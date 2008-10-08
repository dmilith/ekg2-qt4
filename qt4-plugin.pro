message("Qt4 plugin for ekg2, by Daniel (dmilith) Dettlaff")
VERSION = 0.0.4
TEMPLATE = lib
CONFIG *= plugin debug qt thread shared warn_off
HEADERS *= qt4_window.h \
			qt4_config_window.h
SOURCES *= qt4_window.cpp \
			qt4_config_window.cpp \
			main.cpp
FORMS *= ekg2_qt4.ui \
			ekg2_qt4_config.ui
RESOURCES *= ekg2_qt4.qrc
DEFINES *= USE_UNICODE
LIBS *= 

linux-g++ {
	message(Host OS - Linux)
	message(Qt version: $$[QT_VERSION])
	message(Qt is installed in $$[QT_INSTALL_PREFIX])
	message(Qt resources can be found in the following locations:)
	message(Documentation: $$[QT_INSTALL_DOCS])
	message(Header files: $$[QT_INSTALL_HEADERS])
	message(Libraries: $$[QT_INSTALL_LIBS])
	message(Binary files (executables): $$[QT_INSTALL_BINS])
	message(Plugins: $$[QT_INSTALL_PLUGINS])
	message(Data files: $$[QT_INSTALL_DATA])
	message(Translation files: $$[QT_INSTALL_TRANSLATIONS])
}

defineTest(allFiles) {
	files = $$SOURCES + $$HEADERS + $$FORMS + $$RESOURCES
		for(file, files) {
			!exists($$file) {
				return(false)
			}
		}
	return(true)
}

!exists("/usr/bin/ccache") {
CMAKE_CXX = g++	
} else {
QMAKE_CXX = ccache g++
}
INCLUDEPATH *= ../..
QMAKE_CXXFLAGS *= -ggdb -O0
# not needed? QMAKE_LFLAGS *= -Wl,--unresolved-symbols=ignore-all
TARGET = qt
target.path = .libs/
# sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS *.pro
# sources.path = ./
INSTALLS *= target
QMAKE_CLEAN *= Makefile

