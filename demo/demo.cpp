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

    // Have it connect
    cmis->open();
}

void Demo::handleNetworkError(QNetworkReply* reply,
                              QNetworkReply::NetworkError error)
{
     qDebug("Error requesting %d:", error);
     qDebug() << reply->errorString();
}


int main(int argc, char *argv[]) {
   QCoreApplication app(argc, argv);

   Demo demo;
   QTimer::singleShot(0, &demo, SLOT(execute()));

   return app.exec();
}
