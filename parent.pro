TEMPLATE = subdirs

# Keep things in the right order
CONFIG += ordered
SUBDIRS = qtcmislib demo

# Demo depends on the lib
demo.depends = qtcmislib
