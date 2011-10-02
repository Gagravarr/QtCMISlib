#ifndef CMIS_DEMO_H
#define CMIS_DEMO_H

#include <QCoreApplication>
#include <QTextStream>
#include <QTimer>

#include <qtcmislib.h>
#include <stdio.h>

class Demo : public QObject
{
    Q_OBJECT

public:
    Demo();
public slots:
    void execute();
    void handleNetworkError(QNetworkReply* reply,
                            QNetworkReply::NetworkError error);
};

#endif // CMIS_DEMO_H
