QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/types/actionmanager.cpp \
    src/types/explorermanager.cpp \
    src/types/explorersplitter.cpp \
    src/types/explorersplitterhandle.cpp \
    src/types/opendircommand.cpp \
    src/types/treeviewmanager.cpp \
    src/ui/aboutdialog.cpp \
    src/ui/mainwindow.cpp

HEADERS += \
    src/types/actionmanager.h \
    src/types/explorermanager.h \
    src/types/explorersplitter.h \
    src/types/explorersplitterhandle.h \
    src/types/opendircommand.h \
    src/types/treeviewmanager.h \
    src/ui/aboutdialog.h \
    src/ui/mainwindow.h

FORMS += \
    src/ui/aboutdialog.ui \
    src/ui/mainwindow.ui

TRANSLATIONS += \
    lang/Xplorer_af_ZA.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/res.qrc
