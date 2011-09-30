#include <QCoreApplication>
#include <QTextStream>

#include <qtcmislib.h>

int main(int argc, char *argv[]) {
   QCoreApplication a(argc, argv);

   printf("Hello world!\n");

   // Create our connection
   QtCMISlib cmis = QtCMISlib::QtCMISlib();

   return 0;
}
