#ifndef QTCMISLIB_H
#define QTCMISLIB_H

#include "qtcmislib_global.h"

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

QNetworkAccessManager* nam;

class QTCMISLIBSHARED_EXPORT QtCMISlib {
public:
    QtCMISlib();
    QtCMISlib(const QString & repository);
};

#endif // QTCMISLIB_H
