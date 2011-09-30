#ifndef QTCMISLIB_H
#define QTCMISLIB_H

#include "qtcmislib_global.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QObject>

class QTCMISLIBSHARED_EXPORT QtCMISlib : protected QObject
{
    Q_OBJECT 

public:
    QtCMISlib();
    QtCMISlib(const QString & repository);
    void open(QNetworkAccessManager* nam);

public slots:
    void requestFinishedSlot(QNetworkReply* reply);

private:
    QString repository;
    QNetworkAccessManager* nam;
};

#endif // QTCMISLIB_H
