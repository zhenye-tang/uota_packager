#-------------------------------------------------
#
# Project created by QtCreator 2023-12-27T23:01:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ota_tool
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


SOURCES += \
        main.cpp \
        widget.cpp \
    ota.cpp \
    lz4compressor.cpp \
    zlibcompressor.cpp \
    third_party/zlib/adler32.c \
    third_party/zlib/compress.c \
    third_party/zlib/crc32.c \
    third_party/zlib/deflate.c \
    third_party/zlib/infback.c \
    third_party/zlib/inffast.c \
    third_party/zlib/inflate.c \
    third_party/zlib/inftrees.c \
    third_party/zlib/trees.c \
    third_party/zlib/uncompr.c \
    third_party/zlib/zutil.c

HEADERS += \
        widget.h \
    ota.h \
    compressor.h \
    compressfactory.h \
    lz4compressor.h \
    zlibcompressor.h \
    third_party/zlib/crc32.h \
    third_party/zlib/deflate.h \
    third_party/zlib/inffast.h \
    third_party/zlib/inffixed.h \
    third_party/zlib/inflate.h \
    third_party/zlib/inftrees.h \
    third_party/zlib/trees.h \
    third_party/zlib/zconf.h \
    third_party/zlib/zlib.h \
    third_party/zlib/zutil.h


win32: LIBS += -L$$PWD/third_party/lz4/ -lliblz4_static

INCLUDEPATH += $$PWD/third_party/lz4
DEPENDPATH += $$PWD/third_party/lz4
