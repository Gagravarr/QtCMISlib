TEMPLATE = app
CONFIG -= moc
CONFIG += qt console
QT -= gui
QT += network xml

# Input
SOURCES += demo.cpp

# We depend on QtCMISLib
INCLUDEPATH += .
INCLUDEPATH += ../qtcmislib/

PRE_TARGETDEPS = ../qtcmislib/libqtcmislib.so
LIBS += ../qtcmislib/libqtcmislib.so

QtCMISLib.target = ../qtcmislib/libqtcmislib.so
QtCMISLib.command = cd ../qtcmislib/ && make
QtCMISLib.depends = ../qtcmislib/Makefile

QMAKE_EXTRA_TARGETS += QtCMISLib
