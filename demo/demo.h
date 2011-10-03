#ifndef CMIS_DEMO_H
#define CMIS_DEMO_H

#include <QCoreApplication>

#include <qtcmislib.h>
#include <stdio.h>

class Demo : public QObject
{
    Q_OBJECT

public:
    Demo();
signals:
    void exit();
public slots:
    void execute();
    void handleNetworkError(QNetworkReply* reply,
                            QNetworkReply::NetworkError error);
    void handleXmlError(QNetworkReply* reply, QString error);
};

#endif // CMIS_DEMO_H
