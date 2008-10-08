/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */


#include "ekg2_includes.h"
#include "qt4_window.h"

using namespace Ui;
using namespace std;

Qt4Plugin::Qt4Plugin( const QString& title ) {
	setupUi( this );
	config_window = NULL;
	set_current_window( 1 ); // 0 -> debug window, 1 -> status window
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint;
	setWindowFlags( flags );
	setWindowTitle( title );
	init_actions();
	//assign shortcuts
/*	setShortcut( "1", "Alt+1", 0, QApplication::UnicodeUTF8 );
	setShortcut( "2", "Alt+2", 0, QApplication::UnicodeUTF8 );
	setShortcut( "0", "Alt+0", 0, QApplication::UnicodeUTF8 );
*/	show();
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
Qt4Plugin::new_window() { //DEBUG action ofcourse.. it should be done automatically
	session_t *s;
	if (!session_current)
		return;
	s = session_current;
	
	QString window_number =  QString::number( tabs->count() + 1, 10 );
	QString window_name = "win"; // XXX: here should be nick/uid/id of user/chan name
	QString cmd = "/window new " + window_number;
	command_exec( window_number.toLatin1(), s, cmd.toLatin1(), 0 );

	QWidget *window = new QWidget();
	tabs->addTab( window, window_number + ":" + window_name );

	window_t *w = window_find( ( window_number + window_name ).toLatin1() );
	query_emit_id( NULL, UI_WINDOW_NEW, &w );
	
	set_current_window( get_current_window() + 1 );
	tabs->setCurrentIndex( get_current_window() );

	// XXX FIXME This below seems to not work well ;}
	QTextBrowser *window_content = new QTextBrowser( window );
	window_content->setObjectName( window_number );
	window_content->setGeometry(QRect(0, 0, 621, 431));
	window_content->setAutoFillBackground(true);
	window_content->setAutoFormatting(QTextEdit::AutoAll);
	window_content->show();
//	window_content->objectName().append( "New window" );
	

	retranslateUi( this );
	update(); // don't forget to update window!
}

void
Qt4Plugin::kill_window() {
	if ( tabs->currentIndex() > 1 ) {
		tabs->removeTab( get_current_window() );
		set_current_window( tabs->currentIndex() );
	}
}

void
Qt4Plugin::next_window() {
	if ( ! get_current_window() ) {
		set_current_window( 1 );
	} else if ( get_current_window() + 1 < tabs->count() )
		set_current_window( get_current_window() + 1 );
	else
		set_current_window( 1 ); //cause 0 is debug window which we don't want to switch on by default
	tabs->setCurrentIndex( get_current_window() );
}

void
Qt4Plugin::previous_window() {
	if ( ! get_current_window() ) {
		set_current_window( tabs->count() - 1 );
	} else if ( get_current_window() - 1 > 0 )
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
		tabs->setCurrentIndex( 0 );
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

const char*
Qt4Plugin::get_current_window_name() {
	return tabs->tabText( tabs->currentIndex() ).toLatin1();
}

void
Qt4Plugin::qt_entry_command_exec() {
	session_t *s;
	userlist_t *ul;
	if (!session_current)
		return;
	s = session_current;

	for (ul = s->userlist; ul; ul = ul->next) {
		userlist_t *u = ul;
		qt_userlist->addItem( QString( u->uid ) );
	}

	QString command = qt_entry->text();
	if ( command[0] == '/' ) { // command
		command_exec( NULL, NULL, command.toUtf8(), 0 );
	} else {
		const char* temp = get_current_window_name();
		if ( strcmp( temp,"dbg" ) ) return;
		if ( strcmp( temp,"status" ) ) return;
		command_exec( temp, s, command.toUtf8(), 0 );
		// TODO: normal typing
	}
	qt_entry->clear();
}


/*
// TODO: make buffer for last commands
void
Qt4Plugin::qt_entry_command_previous() {
//	Qt::Key_Up
	QString command = qt_entry->text();
	command_exec( NULL, NULL, command.toUtf8(), 0 );
	qt_entry->clear();
}
*/

