QT += core gui serialbus charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += \
    c++20 \
    sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    modbusrtu.cpp \
    qledindicator.cpp \
    widget.cpp \

HEADERS += \
    modbusrtu.h \
    qledindicator.h \
    widget.h \

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
