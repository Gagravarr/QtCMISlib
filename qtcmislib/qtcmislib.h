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

#ifndef QTCMISLIB_H
#define QTCMISLIB_H

#include "qtcmislib_global.h"

#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QDomDocument>
#include <QDomElement>

#include <QObject>
#include <QList>

// Namespaces
const QString ATOM_NS = "http://www.w3.org/2005/Atom";
const QString APP_NS  = "http://www.w3.org/2007/app";
const QString CMISRA_NS = "http://docs.oasis-open.org/ns/cmis/restatom/200908/";
const QString CMIS_NS   = "http://docs.oasis-open.org/ns/cmis/core/200908/";
const QString ALFRESCO_NS = "http://www.alfresco.org";

// Content Types
const QString ATOM_XML_TYPE = "application/atom+xml";
const QString ATOM_XML_ENTRY_TYPE = "application/atom+xml;type=entry";
const QString CMIS_TREE_TYPE = "application/cmistree+xml";
const QString CMIS_QUERY_TYPE = "application/cmisquery+xml";
const QString CMIS_ACL_TYPE = "application/cmisacl+xml";


// Info on a Repository
class QTCMISLIBSHARED_EXPORT QtCMISRepositoryInfo : protected QObject
{
    Q_OBJECT 

public:
    QString repositoryId;
    QString repositoryName;
    QString vendorName;
    QString productName;
    QString productVersion;
    QString rootFolderId;
    // TODO Capabilities
    // TODO ACL Capabilities
    QString specificationTitle;
    double versionSupported;

private:
    QtCMISRepositoryInfo(QDomElement* infoElement);
    friend class QtCMISRepository;
};

// An Object (Folder or Document) in a Repository
class QTCMISLIBSHARED_EXPORT QtCMISEntry : protected QObject
{
    Q_OBJECT

protected:
    QtCMISEntry(QString objectId, QString name, QString title);
    friend class QtCMISRepository;

public:
    QString objectId;
    QString name;
    QString title;
};

// A Folder in a Repository
class QTCMISLIBSHARED_EXPORT QtCMISFolder : public QtCMISEntry
{
    Q_OBJECT

protected:
    QtCMISFolder(QString objectId, QString name, QString title, QString childrenLink);
    friend class QtCMISRepository;
public:
    void createFolder(QString name);
    void createDocument(QString name, QByteArray data);
    void getChildren();
    void getDescendents();
    void getTree();

signals:
    void childrenAvailable(QString type, QList<QtCMISEntry*> children);

private:
    QString getChildrenLink;
};

// A Document in a Repository
class QTCMISLIBSHARED_EXPORT QtCMISDocument : public QtCMISEntry
{
    Q_OBJECT
};

// A Repository
class QTCMISLIBSHARED_EXPORT QtCMISRepository : protected QObject
{
    Q_OBJECT 

public:
    void getFolder(QString folderId);
    void getObject(QString objectId);
    void getObjectByPath(QString objectPath);

    QtCMISFolder* rootFolder;
    QtCMISRepositoryInfo* info;

signals:
    void objectAvailable(QString objectId, QtCMISEntry* object);

private:
    QtCMISRepository(QDomElement* workspaceElement);
    friend class QtCMISlib;
};

// The Main Library class
class QTCMISLIBSHARED_EXPORT QtCMISlib : public QObject
{
    Q_OBJECT 

public:
    QtCMISlib();
    QtCMISlib(const QString & repositoryUrl, const QString & username,
              const QString & password);
    void open();

    QList<QtCMISRepository*> getRepositories();
    QtCMISRepository* getRepository(QString repositoryId);

signals:
    void networkError(QNetworkReply* reply,
                      QNetworkReply::NetworkError error);
    void xmlError(QNetworkReply* reply, QString error);
    void openCompleted();

private slots:
    void getRepositoriesCompleted();

    void authenticationRequired(QNetworkReply* reply, 
                                QAuthenticator* authenticator);
    void handleError(QNetworkReply::NetworkError error);

private:
    void init(const QString & repositoryUrl, const QString & username,
              const QString & password);

    QString repositoryUrl;
    QString username;
    QString password;
    QNetworkAccessManager* nam;

    QList<QtCMISRepository*> repositories;
};

#endif // QTCMISLIB_H
