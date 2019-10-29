#-------------------------------------------------
#
# Project created by QtCreator 2019-09-28T15:48:53
#
#-------------------------------------------------

QT       += core gui
QT       += sql network
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Market
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        IMLoginCtrl.cpp \
        IMMainCtrl.cpp \
        IMRegisterCtrl.cpp \
        IMTcpSocket.cpp \
        commoditybutton.cpp \
        dialog.cpp \
        main.cpp \
        myinform.cpp \
        onlydiscount.cpp \
        register.cpp \
        sell.cpp \
        updatediscount.cpp

HEADERS += \
        IMConstant.h \
        IMLoginCtrl.h \
        IMMainCtrl.h \
        IMRegisterCtrl.h \
        IMTcpSocket.h \
        commoditybutton.h \
        dialog.h \
        myinform.h \
        onlydiscount.h \
        register.h \
        sell.h \
        updatediscount.h

FORMS += \
        dialog.ui \
        myinform.ui \
        onlydiscount.ui \
        register.ui \
        sell.ui \
        updatediscount.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
