QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyQtProject
TEMPLATE = app
CONFIG += c++11
SOURCES += main.cpp

#CONFIG += link_pkgconfig
#PKGCONFIG += Qt6Core Qt6Gui Qt6Widgets

# Ensure these paths are correct for your Qt installation
INCLUDEPATH += /opt/homebrew/opt/qt@6/include
#INCLUDEPATH += /opt/homebrew/include
LIBS += -L/opt/homebrew/opt/qt@6/lib -F/opt/homebrew/opt/qt@6/lib \
        -framework QtCore -framework QtGui -framework QtWidgets




