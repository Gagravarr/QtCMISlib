#include "qtcmislib.h"
#include <iostream>

QtCMISlib::QtCMISlib() : QObject()
{
    // Default is to talk to a local Alfresco install
    init( "http://localhost:8080/alfresco/service/api/cmis" );
}
QtCMISlib::QtCMISlib(const QString & repo) : QObject()
{
    init(repo);
}

void QtCMISlib::init(const QString & repo)
{
    repository = repo;
    std::cout << "Repo is " + repository.toStdString() + "\n";
}

void QtCMISlib::open(QNetworkAccessManager* manager)
{
    nam = manager;

//    QtCMISlib::connect(nam, SIGNAL(finished(QNetworkReply*)),
//             this, SLOT(requestFinishedSlot(QNetworkReply*)));
}

void requestFinishedSlot(QNetworkReply* reply)
{

}
