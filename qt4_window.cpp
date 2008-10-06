/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */


#include "ekg2_includes.h"
#include "qt4_window.h"

using namespace Ui;

Qt4Plugin::Qt4Plugin( const QString& title ) {
	config_window = NULL;
	setupUi( this );
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint;
	setWindowFlags( flags );
	setWindowTitle( title );
	init_actions();
	show();
}

Qt4Plugin::~Qt4Plugin() {
	if (config_window) delete config_window;
}

void
Qt4Plugin::init_actions() {
	QObject::connect( action_exit, SIGNAL( activated() ), this, SLOT( close() ));
	QObject::connect( action_core_settings, SIGNAL( activated() ), this, SLOT( open_config_window() ));
}

bool
Qt4Plugin::is_alive() {
	if ( this->isVisible() )
		return true;
	else
		return false;
}

void
Qt4Plugin::open_config_window() {
	config_window = new Qt4Config( "Ekg2 Core configuration." );
}
 
