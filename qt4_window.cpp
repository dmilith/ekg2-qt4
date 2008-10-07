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
	setupUi( this );
	config_window = NULL;
	set_current_window( 1 ); // 1 -> status window // tabs->currentIndex();
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
	QObject::connect( action_clear_current_window, SIGNAL( activated() ), this, SLOT( clear_current_window() ));
	QObject::connect( action_enable_debug_window, SIGNAL( activated() ), this, SLOT( enable_debug_window() ));

	QObject::connect( action_new_window, SIGNAL( activated() ), this, SLOT( new_window() ));
	QObject::connect( action_kill_window, SIGNAL( activated() ), this, SLOT( kill_window() ));
	QObject::connect( action_previous_window, SIGNAL( activated() ), this, SLOT( previous_window() ));
	QObject::connect( action_next_window, SIGNAL( activated() ), this, SLOT( next_window() ));

	//qt command line
	QObject::connect( qt_entry, SIGNAL( returnPressed() ), this, SLOT( qt_entry_command_exec() ));
//	QObject::connect( qt_entry, SIGNAL( keyPressEvent() ), this, SLOT( qt_entry_command_previous() ));
}

void
Qt4Plugin::set_current_window( int value ) {
	current_window = value;
}

int
Qt4Plugin::get_current_window() {
	return current_window;
}

void
Qt4Plugin::new_window() {
}

void
Qt4Plugin::kill_window() {
}

void
Qt4Plugin::next_window() {
	if ( get_current_window() + 1 < tabs->count() )
		set_current_window( get_current_window() + 1 );
	else
		set_current_window( 1 ); //cause 0 is debug window which we don't want to switch on by default
	tabs->setCurrentIndex( get_current_window() );
}

void
Qt4Plugin::previous_window() {
	if ( get_current_window() - 1 > 0 )
		set_current_window( get_current_window() - 1 );
	else
		set_current_window( tabs->count() - 1 ); 
	tabs->setCurrentIndex( get_current_window() );
}

bool
Qt4Plugin::is_alive() {
	if ( this->isVisible() )
		return true;
	else
		return false;
}

void
Qt4Plugin::enable_debug_window() {
	if ( tabs->isTabEnabled( 0 ) ) {
		tabs->setTabEnabled( 0, false );
	} else {
		tabs->setTabEnabled( 0, true );
	}
}

void
Qt4Plugin::open_config_window() {
	if (! config_window )
		config_window = new Qt4Config( "Ekg2 Core configuration." );
}

void
Qt4Plugin::clear_current_window() {
	//( tabs->currentIndex() )->clear();
}

void
Qt4Plugin::qt_entry_command_exec() {
	QString command = qt_entry->text();
	command_exec( NULL, NULL, command.toUtf8(), 0 );
	qt_entry->clear();
}

/*
void
Qt4Plugin::qt_entry_command_previous() {
//	Qt::Key_Up
	QString command = qt_entry->text();
	command_exec( NULL, NULL, command.toUtf8(), 0 );
	qt_entry->clear();
}
*/

