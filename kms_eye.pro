QT += quick core

CONFIG += c++11

HEADER_PATH = ./hdr
SOURCE_PATH = ./src

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$HEADER_PATH

CONFIG -= qt5

SOURCES += \
        src/DirHelper.cpp \
        src/main.cpp \
        src/QConfig.cpp

HEADERS += hdr/QConfig.h \
           hdr/DirHelper.h

#INCLUDEPATH +=  -I/usr/include \
#                ${OpenCV_INCLUDE_DIRS}

LIBS += -lopencv_highgui -lopencv_core -lopencv_imgproc -lopencv_videoio

RESOURCES += share/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += data/EurocStereoVIO.yaml


