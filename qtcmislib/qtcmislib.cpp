#include "qtcmislib.h"
#include <iostream>
#include <stdexcept>
#include <QEventLoop>
#include <QtDebug>

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
   xml->setContent(reply);

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
for(int i=0; i<we->childNodes().count(); i++) {
qDebug() << we->childNodes().at(i).localName();
qDebug() << we->childNodes().at(i).namespaceURI();
}

   QDomNodeList infoList = we->elementsByTagNameNS(CMISRA_NS, "repositoryInfo");
   if(infoList.count() != 1)
   {
      QString error = QString("Incorrect number of repositoryInfo elements, should be 1 but found %1").arg(infoList.count());
      throw std::range_error(error.toStdString().c_str());
   }
   QDomElement infoElement = infoList.at(0).toElement();
   info = new QtCMISRepositoryInfo(&infoElement);

   // TODO Build the rest
}


QtCMISRepositoryInfo::QtCMISRepositoryInfo(QDomElement* infoElement)
{
   // TODO
}
