/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "qtcmislib.h"
#include <iostream>
#include <stdexcept>
#include <QEventLoop>
#include <QtDebug>

QString getValueOrNull(QDomElement* parent, QString ns, QString tag)
{
   QDomNodeList tagList = parent->elementsByTagNameNS(ns, tag);
   if(tagList.count() == 0) {
      return NULL;
   }
   if(tagList.count() > 1) {
      QString error = QString("Incorrect number of %1 elements, should be 1 but found %2").
                          arg(tag).arg(tagList.count());
      throw std::range_error(error.toStdString().c_str());
   }

   QDomElement element = tagList.at(0).toElement();
   return element.text();
}

QtCMISlib::QtCMISlib() : QObject()
{
    // Default is to talk to a local Alfresco install
    init(
        "http://localhost:8080/alfresco/s/cmis",
        "admin", "admin"
    );
}
QtCMISlib::QtCMISlib(const QString & repositoryUrl, const QString & user, 
                     const QString & pass) : QObject()
{
    init(repositoryUrl, user, pass);
}
void QtCMISlib::init(const QString & repositoryUrl, const QString & username, 
                     const QString & password)
{
    // Save the connection details
    this->repositoryUrl = repositoryUrl;
    this->username = username;
    this->password = password;

    // Create our Network Manager
    nam = new QNetworkAccessManager(this);

    // Wire up the authentication needed handler
    connect(nam,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this,SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));

    // Bit of debugging for now
    qDebug("Repo is %s", repositoryUrl.toStdString().c_str());
}

void QtCMISlib::open()
{
    qDebug("Fetching core repo details");

    QNetworkRequest request;
    request.setUrl(QUrl(repositoryUrl));
    request.setRawHeader("User-Agent", "QtCMISlib 0.1");

    QNetworkReply *reply = nam->get(request);
    connect(reply, SIGNAL(finished()),
            this, SLOT(getRepositoriesCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(handleError(QNetworkReply::NetworkError)));
}

QList<QtCMISRepository*> QtCMISlib::getRepositories()
{
   return repositories;
}
QtCMISRepository* QtCMISlib::getRepository(QString repositoryId)
{
   foreach (QtCMISRepository* repo, repositories)
   {
      if(repo->info->repositoryId == repositoryId) 
          return repo;
   }
   return NULL;
}

void QtCMISlib::getRepositoriesCompleted()
{
   QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
   qDebug("Got repository info from %s",
          reply->request().url().toString().toStdString().c_str());

   // Parse our XML
   QDomDocument* xml = new QDomDocument();
   xml->setContent(reply, true);

   QDomElement svc = xml->documentElement();
   if(svc.tagName() != "service")
   {
      QString error = QString("Invalid XML received, got ");
      error.append(svc.tagName());
      emit xmlError(reply, error);
      return;
   }

   // Build up the list of Repositories in the reply
   QDomNodeList workspaces = svc.elementsByTagNameNS(APP_NS, "workspace");
   if(workspaces.size() == 0) 
   {
       workspaces = svc.elementsByTagName("workspace");
   }
   if(workspaces.size() == 0) 
   {
      emit xmlError(reply, QString("No workspaces found"));
   }
   for(int i=0; i<workspaces.count(); i++)
   {
      QDomElement workspace = workspaces.at(i).toElement();
      try
      {
         QtCMISRepository* repo = new QtCMISRepository(&workspace);
         repositories.append(repo);
      }
      catch(std::runtime_error e)
      {
         emit xmlError(reply, QString(e.what()));
      }
   }

   // Indicate we're done
   emit openCompleted();

   // Tidy up
   reply->deleteLater();
}

void QtCMISlib::authenticationRequired(QNetworkReply* reply, 
                                       QAuthenticator* authenticator)
{
   authenticator->setUser(username);
   authenticator->setPassword(password);
}

void QtCMISlib::handleError(QNetworkReply::NetworkError error)
{
   // Grab the request the triggered it
   QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

   // Tell our clients that something broke...
   emit networkError(reply, error);

   // Debugging just in case
   qDebug("Hit error on request: %d:", error);

   // Tidy up
   reply->deleteLater();
}


QtCMISRepository::QtCMISRepository(QDomElement* we)
{
   // Check
   if(we == NULL)
   {
     throw std::runtime_error("<workspace> element must be supplied");
   }

   // Build the Info object
   QDomNodeList infoList = we->elementsByTagNameNS(CMISRA_NS, "repositoryInfo");
   if(infoList.count() != 1) {
      QString error = QString("Incorrect number of repositoryInfo elements, should be 1 but found %1").
                          arg(infoList.count());
      throw std::range_error(error.toStdString().c_str());
   }
   QDomElement infoElement = infoList.at(0).toElement();
   info = new QtCMISRepositoryInfo(&infoElement);

   // Build the Root Folder object
   rootFolder = new QtCMISFolder(info->rootFolderId, QString("root"), QString("root"), NULL);

   // TODO Build the rest
}


QtCMISRepositoryInfo::QtCMISRepositoryInfo(QDomElement* infoElement)
{
   // Grab the common parts
   repositoryId = getValueOrNull(infoElement, CMIS_NS, "repositoryId");
   repositoryName = getValueOrNull(infoElement, CMIS_NS, "repositoryName");
   vendorName = getValueOrNull(infoElement, CMIS_NS, "vendorName");
   productName = getValueOrNull(infoElement, CMIS_NS, "productName");
   productVersion = getValueOrNull(infoElement, CMIS_NS, "productVersion");
   rootFolderId = getValueOrNull(infoElement, CMIS_NS, "rootFolderId");
   specificationTitle = getValueOrNull(infoElement, ALFRESCO_NS, "cmisSpecificationTitle");

   QString versionSupportedS = getValueOrNull(infoElement, CMIS_NS, "cmisVersionSupported");
   if(versionSupportedS == NULL) {
     versionSupported = 0.0;
   } else {
     versionSupported = versionSupportedS.toDouble();
   }

   // TODO Build the rest
}

QtCMISFolder::QtCMISFolder(QString objectId, QString name, QString title, QString childrenLink)
    : QtCMISEntry(objectId, name, title)
{
   this->getChildrenLink = childrenLink;

   // TODO
}

QtCMISEntry::QtCMISEntry(QString objectId, QString name, QString title)
{
   this->objectId = objectId;
   this->name = name;
   this->title = title;
}
