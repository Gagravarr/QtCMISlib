TEMPLATE = subdirs

# Keep things in the right order
CONFIG += ordered
SUBDIRS = qtcmislib demo
CC_OPTS += -g

# Demo depends on the lib
demo.depends = qtcmislib
