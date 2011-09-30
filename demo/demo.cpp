#include <demo.h>

Demo::Demo() {}
void Demo::execute()
{
    printf("Hello world!\n");

    // Create our QNetworkAccessManager
    QNetworkAccessManager* nam = new QNetworkAccessManager(this);

    // Create our connection
    QtCMISlib cmis = QtCMISlib::QtCMISlib();
    cmis.open(nam);
}


int main(int argc, char *argv[]) {
   QCoreApplication app(argc, argv);

   Demo demo;
   QTimer::singleShot(0, &demo, SLOT(execute()));

   return app.exec();
}
