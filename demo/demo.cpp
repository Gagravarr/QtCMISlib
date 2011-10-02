#include <demo.h>

// While testing
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>


Demo::Demo() {}
void Demo::execute()
{
    printf("Hello world!\n");

    // Create our connection
    QtCMISlib cmis;
    cmis.open();

//QNetworkAccessManager* nam = new QNetworkAccessManager(this);
//QNetworkRequest request;
//request.setUrl(QUrl("http://localhost:8080/alfresco/service/api/cmis"));
//
//QNetworkReply *reply = nam->get(request);
//
//QEventLoop loop;
//QObject::connect(reply, SIGNAL(readyRead()), &loop, SLOT(quit()));
//loop.exec();
//printf("Done waiting\n");
}


int main(int argc, char *argv[]) {
   QCoreApplication app(argc, argv);

   Demo demo;
   QTimer::singleShot(0, &demo, SLOT(execute()));

   return app.exec();
}
