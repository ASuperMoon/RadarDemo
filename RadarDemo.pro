QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

OBJECTS_DIR =$$PWD/Build/obj
MOC_DIR = $$PWD/Build/moc
RCC_DIR = $$PWD/Build/rcc
UI_DIR = $$PWD/Build/ui

SOURCES += \
    Net/udpclass.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Net/udpclass.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc
