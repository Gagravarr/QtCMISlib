#ifndef QTCMISLIB_H
#define QTCMISLIB_H

#include "qtcmislib_global.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include <QObject>

QNetworkAccessManager* nam;

class QTCMISLIBSHARED_EXPORT QtCMISlib : public QObject
{
    Q_OBJECT 

public:
    QtCMISlib();
    QtCMISlib(const QString & repository);
    void open(QNetworkAccessManager* nam);
private:
    void init(const QString & repository);
    QString repository;
    QNetworkAccessManager* nam;
};

#endif // QTCMISLIB_H
