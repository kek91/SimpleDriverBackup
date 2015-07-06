#-------------------------------------------------
#
# Project created by QtCreator 2015-05-06T11:01:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SimpleDriverBackup
TEMPLATE = app


SOURCES += main.cpp\
        widget_simpledriverbackup.cpp \
    aboutdialog.cpp

HEADERS  += widget_simpledriverbackup.h \
    aboutdialog.h

FORMS    += widget_simpledriverbackup.ui \
    aboutdialog.ui

win32:RC_ICONS += sdb_icon.ico
