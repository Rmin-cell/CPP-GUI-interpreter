QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyQtProject
TEMPLATE = app
CONFIG += c++11
SOURCES += main.cpp

#CONFIG += link_pkgconfig
#PKGCONFIG += Qt6Core Qt6Gui Qt6Widgets

#python path va in dastana
INCLUDEPATH += /opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/include/python3.13
INCLUDEPATH += /opt/homebrew/lib/python3.13/site-packages/numpy/core/include

LIBS += -L/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/lib
LIBS += -lpython3.13

# Ensure these paths are correct for your Qt installation
INCLUDEPATH += /opt/homebrew/opt/qt@6/include
#INCLUDEPATH += /opt/homebrew/include
LIBS += -L/opt/homebrew/opt/qt@6/lib -F/opt/homebrew/opt/qt@6/lib \
        -framework QtCore -framework QtGui -framework QtWidgets






