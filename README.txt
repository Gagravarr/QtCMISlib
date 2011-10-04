                  Qt CMIS Library
                  ---------------

This is a Qt Powered Library for talking to a CMIS (Content Management 
Interoperability Services) content repository. 

CMIS is a standard for improving interoperability between ECM systems. 
It specifies a domain model plus a set of services and protocol bindings 
for Web Services (SOAP) and AtomPub. This library attempts to provide a
Qt powered, Qt compatible client implementation for talking to CMIS
repositories.

Currently, only the AtomPub binding is supported. The library is hopefully
designed so that other bindings (such as the JSON "Browser Binding") can
be used in future too.

For more information on CMIS, including links to the specifications,
then http://cmis.alfresco.com/ is an excellent starting point.


Limitations
-----------
This is still very much a work in progress, and it is currently not 
possible to list, read or write. At this point, only connecting to a
repository and returning the information on it is supported.

The intended use case is to be able to connect to a repository, list
folders and files, browse, and retrieve content. Write/Update/Create support
would be nice, but patches would be needed for this!


Requirements
------------
The library requires Qt 4. It has been tested on Qt 4.6, but most recent
4.x versions should work.


Qt Problems for new Qt Users
----------------------------
When working with Qt, you can get some very cryptic error messages
for some apparently minor mistakes... This is a list of a few of the
common ones found when developing this library:

Problem:
   undefined reference to `vtable for .....`
Cause:
   You've changed something to be based on QObject / not based on QObject,
   and the auto-magic moc_xxx.cpp file hasn't been generated for you
Fix:
   run "make clean" and then "qmake -r" on all the projects to have the 
   appropriate moc files produced

Problem:
   qobject.h: In copy constructor ‘QtCMIS...::QtCMIS...(const QtCMIS...&)’:
   qobject.h: error: ‘QObject::QObject(const QObject&)’ is private
Cause:
   QObject based classes can really only be worked with by pointers, and 
   never by reference (despite what tutorials suggest). Attempting to
   work with non-references leads to a value copy somewhere, which
   QObject doesn't allow
Fix:
   Make sure you always work with QObject based classes by referencec, and
   never value, eg "QtCMISlib cmis" becomes "QtCMISlib* cmis"
