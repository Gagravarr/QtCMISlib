#include <demo.h>

Demo::Demo() {}
void Demo::execute()
{
    printf("Hello world!\n");

    // Create our QNetworkAccessManager
//    QNetworkAccessManager* nm = new QNetworkAccessManager(this);

    // Create our connection
    QtCMISlib cmis;
    cmis.open(0);
}


int main(int argc, char *argv[]) {
   QCoreApplication app(argc, argv);

   Demo demo;
   QTimer::singleShot(0, &demo, SLOT(execute()));

   return app.exec();
}
