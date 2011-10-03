#ifndef QTCMISLIB_H
#define QTCMISLIB_H

#include "qtcmislib_global.h"

#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QXmlReader>

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
class QTCMISLIBSHARED_EXPORT QtCMISlib : public QObject
{
    Q_OBJECT 

public:
    QtCMISlib();
    QtCMISlib(const QString & repository, const QString & username,
              const QString & password);
    void open();

    void getRepositories();

signals:
    void networkError(QNetworkReply* reply,
                      QNetworkReply::NetworkError error);

private slots:
    void getRepositoriesCompleted();

    void authenticationRequired(QNetworkReply* reply, 
                                QAuthenticator* authenticator);
    void handleError(QNetworkReply::NetworkError error);

private:
    void init(const QString & repository, const QString & username,
              const QString & password);

    QString repository;
    QString username;
    QString password;
    QNetworkAccessManager* nam;
};

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
    QtCMISRepositoryInfo();
};

class QTCMISLIBSHARED_EXPORT QtCMISRepository : protected QObject
{
    Q_OBJECT 

public:
    QtCMISRepositoryInfo info;

private:
    QtCMISRepository();
};

#endif // QTCMISLIB_H
