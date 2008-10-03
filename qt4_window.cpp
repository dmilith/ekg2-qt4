/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPL2 license
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

extern "C" {

	extern void ekg_exit();

	Qt4Plugin::~Qt4Plugin() {
		ekg_exit();
	}
	
}

void
Qt4Plugin::init_actions() {
	QObject::connect( action_exit, SIGNAL( activated() ), this, SLOT( quit() ));
}

void
Qt4Plugin::quit() {
	this->close();
	this->~Qt4Plugin();
}

