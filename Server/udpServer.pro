QT += core network
QT -= gui

CONFIG += c++11

TARGET = udpServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    udpserver.cpp

HEADERS += \
    udpserver.h
