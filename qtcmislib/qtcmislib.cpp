#include "qtcmislib.h"

QString repository;

QtCMISlib::QtCMISlib()
{
    // Default is to talk to a local Alfresco install
    ::repository = "http://localhost:8080/alfresco/service/api/cmis";
}
QtCMISlib::QtCMISlib(const QString & repo)
{
    ::repository = repo;
}

