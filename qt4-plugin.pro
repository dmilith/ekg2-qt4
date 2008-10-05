TEMPLATE = lib
CONFIG += plugin debug
HEADERS += qt4_window.h
SOURCES += main.cpp \
				qt4_window.cpp
FORMS += ekg2_qt4.ui
  
RESOURCES += ekg2_qt4.qrc
LIBS +=

QMAKE_CXXFLAGS += -O0 -ggdb -fPIC -shared -I../..
QMAKE_LFLAGS += -Wl,--unresolved-symbols=ignore-all
TARGET = qt
TARGETD = qt
target.path = ../
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
sources.path = .
INSTALLS += target sources
