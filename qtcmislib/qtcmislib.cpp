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
void QtCMISlib::init(const QString & repo, const QString & user, 
                     const QString & pass)
{
    repository = repo;
    // TODO Stash user details somewhere helpful

    // Create our Network Manager
    nam = new QNetworkAccessManager(this);

    // Bit of debugging for now
    std::cout << "Repo is " + repository.toStdString() + "\n";
}

void QtCMISlib::open()
{
//    nam = manager;
std::cout << "NAM is ";
std::cout << nam;
std::cout << "\n";
std::cout << "Repo is " + repository.toStdString() + "\n";

    QNetworkRequest request;
    request.setUrl(QUrl(repository));
    request.setRawHeader("User-Agent", "QtCMISlib 0.1");

    QNetworkReply *reply = nam->get(request);
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(getRepositoriesCompleted()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(handleError(QNetworkReply::NetworkError)));


QEventLoop loop;
QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
loop.exec();
std::cout << "Done waiting\n";
}

void QtCMISlib::getRepositories()
{
}

void QtCMISlib::getRepositoriesCompleted()
{
   QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
   std::cout << "Got details back\n";
   QByteArray data = reply->readAll();

}

void QtCMISlib::authenticationRequired(QNetworkReply* reply, 
                                       QAuthenticator* authenticator)
{
   authenticator->setUser("admin"); // TODO Fix
   authenticator->setPassword("admin"); // TODO Fix
}

void QtCMISlib::handleError(QNetworkReply::NetworkError error)
{
   // Grab the request the triggered it
   QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

   // Tell our clients that something broke...
   emit networkError(reply, error);

   // Debugging just in case
   qDebug("Hit error on request: %d:", error);
}
