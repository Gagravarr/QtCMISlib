#ifndef QTCMISLIB_H
#define QTCMISLIB_H

#include "qtcmislib_global.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QObject>

// Namespaces
const QString ATOM_NS = "http://www.w3.org/2005/Atom";
const QString APP_NS  = "http://www.w3.org/2007/app";
const QString CMISRA_NS = "http://docs.oasis-open.org/ns/cmis/restatom/200908/";
const QString CMIS_NS   = "http://docs.oasis-open.org/ns/cmis/core/200908/";

// Content Types
const QString ATOM_XML_TYPE = "application/atom+xml";
const QString ATOM_XML_ENTRY_TYPE = "application/atom+xml;type=entry";
const QString CMIS_TREE_TYPE = "application/cmistree+xml";
const QString CMIS_QUERY_TYPE = "application/cmisquery+xml";
const QString CMIS_ACL_TYPE = "application/cmisacl+xml";


// Main class
class QTCMISLIBSHARED_EXPORT QtCMISlib : protected QObject
{
    Q_OBJECT 

public:
    QtCMISlib();
    QtCMISlib(const QString & repository, const QString & username,
              const QString & password);
    void open(QNetworkAccessManager* nam);

    void getRepositories();

private slots:
    void getRepositoriesCompleted();

private:
    QString repository;
    QNetworkAccessManager* nam;
};

//class QTCMISLIBSHARED_EXPORT QtCMISRepo : protected QObject
//{
//    Q_OBJECT 
//
//public:
//    QtCMISRepo();
//}

#endif // QTCMISLIB_H
