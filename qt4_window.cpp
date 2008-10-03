/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */


#include "qt4_window.h"

using namespace Ui;

Qt4Plugin::Qt4Plugin( const QString& title ) {
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
}

void
Qt4Plugin::init_actions() {
	QObject::connect( action_exit, SIGNAL( activated() ), this, SLOT( close() ));
}

bool
Qt4Plugin::is_alive() {
	if ( this->isVisible() )
		return true;
	else
		return false;
}

