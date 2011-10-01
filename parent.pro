TEMPLATE = subdirs

# Keep things in the right order
CONFIG += ordered
SUBDIRS = qtcmislib demo

# When debugging
QMAKE_CXXFLAGS = -g

# Demo depends on the lib
demo.depends = qtcmislib
