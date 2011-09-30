#include "qtcmislib.h"
#include <iostream>
#include <QEventLoop>

QtCMISlib::QtCMISlib() : QObject()
{
    // Default is to talk to a local Alfresco install
    QtCMISlib::QtCMISlib( 
        "http://localhost:8080/alfresco/service/api/cmis",
        "admin", "admin"
    );
}
QtCMISlib::QtCMISlib(const QString & repo, const QString & user, 
                     const QString & pass) : QObject()
{
    repository = repo;
    // TODO Stash user details somewhere helpful
    nam = new QNetworkAccessManager(this);

    std::cout << "Repo is " + repository.toStdString() + "\n";
}

void QtCMISlib::open(QNetworkAccessManager* manager)
{
//    nam = manager;

    QNetworkRequest request;
    request.setUrl(QUrl(repository));

    QNetworkReply *reply = nam->get(request);
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(getRepositoriesCompleted()));

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
