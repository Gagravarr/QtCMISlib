#include <demo.h>

#include <QtDebug>

Demo::Demo() {}
void Demo::execute()
{
    printf("Hello world!\n");

    // Create our connection
    QtCMISlib* cmis = new QtCMISlib();
    connect(cmis, SIGNAL(networkError(QNetworkReply*,QNetworkReply::NetworkError)),
            this, SLOT(handleNetworkError(QNetworkReply*,QNetworkReply::NetworkError)));
    connect(cmis, SIGNAL(xmlError(QNetworkReply*,QString)),
            this, SLOT(handleXmlError(QNetworkReply*,QString)));

    // Have it connect
    cmis->open();
}

void Demo::handleNetworkError(QNetworkReply* reply,
                              QNetworkReply::NetworkError error)
{
    qDebug() << "Error requesting " << error << ":";
    qDebug() << reply->errorString();

    // TODO Should we always bail out at errors?
    emit exit();
}

void Demo::handleXmlError(QNetworkReply* reply, QString error)
{
    qDebug() << "XML Error " << error;

    // TODO Should we always bail out at errors?
    emit exit();
}

int main(int argc, char *argv[]) {
   QCoreApplication app(argc, argv);

   Demo demo;
   QObject::connect(&demo, SIGNAL(exit()), &app, SLOT(quit()));
   QTimer::singleShot(0, &demo, SLOT(execute()));

   return app.exec();
}
