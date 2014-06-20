include(global.pri)
TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = lib app test
app.depends = lib
test.depends = lib
