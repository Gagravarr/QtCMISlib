#include "qtcmislib.h"
#include <iostream>
#include <QEventLoop>
#include <QtDebug>

QtCMISlib::QtCMISlib() : QObject()
{
    // Default is to talk to a local Alfresco install
    init(
        "http://localhost:8080/alfresco/service/api/cmis",
        "admin", "admin"
    );
}
QtCMISlib::QtCMISlib(const QString & repo, const QString & user, 
                     const QString & pass) : QObject()
{
    init(repo, user, pass);
}
void QtCMISlib::init(const QString & repository, const QString & username, 
                     const QString & password)
{
    // Save the connection details
    this->repository = repository;
    this->username = username;
    this->password = password;

    // Create our Network Manager
    nam = new QNetworkAccessManager(this);

    // Wire up the authentication needed handler
    connect(nam,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this,SLOT(authenticationRequired(QNetworkReply*,QAuthenticator*)));

    // Bit of debugging for now
    qDebug("Repo is %s", repository.toStdString().c_str());
}

void QtCMISlib::open()
{
    qDebug("Fetching core repo details");

    QNetworkRequest request;
    request.setUrl(QUrl(repository));
    request.setRawHeader("User-Agent", "QtCMISlib 0.1");

    QNetworkReply *reply = nam->get(request);
    connect(reply, SIGNAL(finished()),
            this, SLOT(getRepositoriesCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(handleError(QNetworkReply::NetworkError)));
}

void QtCMISlib::getRepositories()
{
}

void QtCMISlib::getRepositoriesCompleted()
{
   QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
   qDebug("Got repository info from %s",
          reply->request().url().toString().toStdString().c_str());
   QByteArray data = reply->readAll();

   //QXmlReader* r = new QXmlReader();

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
