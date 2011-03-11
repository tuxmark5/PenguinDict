#-------------------------------------------------
#
# Project created by QtCreator 2011-03-10T20:47:54
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = PenguinDict
TEMPLATE = app


SOURCES += main.cc\
        PMainWindow.cc \
    PSearchDock.cc \
    PDictListModel.cc \
    PDict.cc \
    PDictCodec.cc \
    PTextEdit.cc

HEADERS  += PMainWindow.hh \
    PSearchDock.hh \
    PDictListModel.hh \
    Penguin.hh \
    PDict.hh \
    PDictCodec.hh \
    PTextEdit.hh

FORMS    += PMainWindow.ui \
    PSearchDock.ui
