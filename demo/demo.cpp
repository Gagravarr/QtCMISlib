#
#      Licensed to the Apache Software Foundation (ASF) under one
#      or more contributor license agreements.  See the NOTICE file
#      distributed with this work for additional information
#      regarding copyright ownership.  The ASF licenses this file
#      to you under the Apache License, Version 2.0 (the
#      "License"); you may not use this file except in compliance
#      with the License.  You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#      Unless required by applicable law or agreed to in writing,
#      software distributed under the License is distributed on an
#      "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#      KIND, either express or implied.  See the License for the
#      specific language governing permissions and limitations
#      under the License.
#

#include <demo.h>

#include <QtDebug>
#include <QEventLoop>
#include <QTimer>

Demo::Demo() {}
void Demo::execute()
{
    QTextStream qout(stdout);
    qout << "Hello world!" << endl;

    // Create our connection
    QtCMISlib* cmis = new QtCMISlib();
    connect(cmis, SIGNAL(networkError(QNetworkReply*,QNetworkReply::NetworkError)),
            this, SLOT(handleNetworkError(QNetworkReply*,QNetworkReply::NetworkError)));
    connect(cmis, SIGNAL(xmlError(QNetworkReply*,QString)),
            this, SLOT(handleXmlError(QNetworkReply*,QString)));

    // Have it connect
    cmis->open();

    // Wait for it to load
    QEventLoop wait;
    connect(cmis, SIGNAL(openCompleted()), &wait, SLOT(quit()));
    wait.exec();

    // Report what we found
    QList<QtCMISRepository*> repos = cmis->getRepositories();

    qout << endl;
    qout << "Found " << repos.count() << " Repositories" << endl;
    for(int i=0; i<repos.count(); i++)
    {
       QtCMISRepository* repo = repos.at(i);
       qout << " Repository " << (i+1) << endl;
       qout << "   ID: " << repo->info->repositoryId << endl;
       qout << "   Name: " << repo->info->repositoryName << endl;
       qout << "   Vendor: " << repo->info->vendorName << endl;
       qout << "   Product: " << repo->info->productName << endl;
       qout << "   Version: " << repo->info->productVersion << endl;
    }

    // All done
    emit exit();
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
