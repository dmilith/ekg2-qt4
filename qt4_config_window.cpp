/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */


#include "ekg2_includes.h"
#include "qtplugin_config.h"
#include "qt4_config_window.h"

using namespace Ui;

Qt4Config::Qt4Config( const QString& title ) {
	setupUi( this );
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint;
	setWindowFlags( flags );
	setWindowTitle( title );
	init_actions();
	show();
}

Qt4Config::~Qt4Config() {
}

void
Qt4Config::init_actions() {
//	QObject::connect( action_, SIGNAL( activated() ), this, SLOT( close() ));
}

