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
	current_command = 0; // index of first element on list
	//current_window_number = "1";
	command_buffer << ""; // to have one empty element
	set_current_window( 1 ); // 0 -> debug window, 1 -> status window
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	Qt::WindowFlags flags;
	flags = Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint;
	setWindowFlags( flags );
	setWindowTitle( title );
	qt_entry->setFocus();
	init_actions();

	// TODO: assign shortcuts
/*	setShortcut( "1", "Alt+1", 0, QApplication::UnicodeUTF8 );
	setShortcut( "2", "Alt+2", 0, QApplication::UnicodeUTF8 );
	setShortcut( "0", "Alt+0", 0, QApplication::UnicodeUTF8 );
*/	
	auto_resize(); // resizes widgets in window for main window size.
	show();
}

Qt4Plugin::~Qt4Plugin() {
	if (config_window) delete config_window;
}

void // resizeEvent is executed when window will be resized
Qt4Plugin::resizeEvent( QResizeEvent * event ) {
	event = NULL;
	auto_resize();
}

/*
bool
Qt4Plugin::event( QEvent *event ) {
	if ( event->type() == QEvent::Enter ) {
	}
}*/

void
Qt4Plugin::keyPressEvent( QKeyEvent * event ) {
	if ( !command_buffer.empty() ) {
		
		// previous command:
		if ( event->key() == Qt::Key_Up ) {
			if ( current_command < command_buffer.count() - 1 ) {
				current_command++;
				qt_entry->setText( command_buffer[ current_command ] );
			} else {
				current_command = 0; //command_buffer.count() - 1;
				qt_entry->setText( command_buffer[ command_buffer.count() - 1 ] );
			}
		}

		// next command:
		if ( event->key() == Qt::Key_Down ) {
			if ( current_command > 0 ) {
				current_command--;
				qt_entry->setText( command_buffer[ current_command ] );
			} else {
				current_command = command_buffer.count();
				qt_entry->setText( command_buffer[ 0 ] );
			}
		}
	}
		if ( event->key() == Qt::Key_Escape ) {
			qt_entry->clear();
		}

		if ( event->key() == Qt::Key_Tab ) { // this is not working, tab is used for different action that's why it's C-Tab not Tab
			qt_entry->setText( "/" );
		}
}

void // auto_resize will resize all window widgets for actual MainWindow size.
Qt4Plugin::auto_resize() {
	int qt_userlist_size = 190;
	qt_userlist->setGeometry( this->width() - qt_userlist_size - 10, 10, qt_userlist_size, this->height() - 35 );
	qt_topic->setGeometry( 10, 10, this->width() - qt_userlist->width() - 30, 30 );
	qt_entry->setGeometry( 10, this->height() - qt_entry->height() - 15, this->width() - qt_userlist->width() - 35, 35 );
	qt_status->setGeometry( 10, this->height() - qt_entry->height() - 50, this->width() - qt_userlist->width() - 35, 25 );
	tabs->setGeometry( 10, 40, this->width() - qt_userlist->width() - 35, 
										this->height() - qt_entry->height() - qt_status->height() - 80 );
	QList<QTextBrowser *> all_browsers = this->findChildren<QTextBrowser *>();
	while ( !all_browsers.empty() ) { // resize all text browsers (windows) to specified size:
		( (QTextBrowser*)all_browsers.takeLast() )->setGeometry( 10, 10, tabs->width() - 20, tabs->height() - 50 );
	}
	
}

void // initiate interface actions
Qt4Plugin::init_actions() {
	QObject::connect( action_exit, SIGNAL( activated() ), this, SLOT( close() ));
	QObject::connect( action_core_settings, SIGNAL( activated() ), this, SLOT( open_config_window() ));
	QObject::connect( action_clear_current_window, SIGNAL( activated() ), this, SLOT( clear_current_window() ));
	QObject::connect( action_enable_debug_window, SIGNAL( activated() ), this, SLOT( enable_debug_window() ));

//	QObject::connect( action_new_window, SIGNAL( activated() ), this, SLOT( new_window() ));
	QObject::connect( action_kill_window, SIGNAL( activated() ), this, SLOT( kill_window() ));
	QObject::connect( action_previous_window, SIGNAL( activated() ), this, SLOT( previous_window() ));
	QObject::connect( action_next_window, SIGNAL( activated() ), this, SLOT( next_window() ));

	//qt command line
	QObject::connect( qt_entry, SIGNAL( returnPressed() ), this, SLOT( qt_entry_command_exec() ));
	
	//qt userlist
	QObject::connect( qt_userlist, SIGNAL( itemSelectionChanged() ), this, SLOT( qt_userlist_selected() ));
}

void
Qt4Plugin::qt_userlist_selected() {
	qt_debug_window->append( "Ui: Selected user: " + ( qt_userlist->item( qt_userlist->currentRow() ) )->text() );
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
Qt4Plugin::new_window( const QString& target ) { //DEBUG action ofcourse.. it should be done automatically
	session_t *s;
	window_t *w;
	if (!session_current)
		return;
	s = session_current;
	
	current_window_number = QString::number( tabs->count(), 10 );
	current_window_name = target; // XXX: here should be nick/uid/id of user/chan name

		// looking for tabs with such name
		for ( int i = 0; i < tabs->count() - 1; i++ ) {
			if ( tabs->tabText( i ).section( ':', 1 ) == target ) {
				// a contact has been found on window "i"
				set_current_window( ( tabs->tabText( i ).section( ':', 0 ) ).toInt() );
				tabs->setCurrentIndex( get_current_window() );
				return;
			}
		}
	
		QWidget *window = new QWidget();
		tabs->addTab( window, current_window_number + ":" + current_window_name );

		w = window_find( ( current_window_number + ":" + current_window_name ).toLatin1() );
		query_emit_id( NULL, UI_WINDOW_NEW, &w );
		
		set_current_window( get_current_window() + 1 );
		tabs->setCurrentIndex( get_current_window() );

		// XXX FIXME This below seems to not work well ;}
		QTextBrowser *window_content = new QTextBrowser( window );
		window_content->setObjectName( current_window_number );
		window_content->setAutoFillBackground(true);
		window_content->setAutoFormatting(QTextEdit::AutoAll);
		window_content->show();

	retranslateUi( this );
	auto_resize();
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
	// TODO: ( tabs->currentIndex() )->clear();
}

QString
Qt4Plugin::get_current_tab_name() {
	return tabs->tabText( tabs->currentIndex() );
}

void
Qt4Plugin::qt_entry_command_exec() {
	session_t *s;
	if ( !session_current )
		return;
	s = session_current;
	QString command = qt_entry->text();
	command_buffer.append( command ); // add command/ text to command buffer
	QString temp = get_current_tab_name();
	if ( command[0] == '/' ) { // command
		command_exec( temp.toLatin1(), s, command.toUtf8(), 0 );
	} else {
		if ( ( temp == (QString)"dbg" ) || ( temp == (QString)"status" ) ) return;
		// XXX FIXME TODO: should check session here (?):
		command.insert( 0, QString( "/query " ) + temp.section( ':', 1 ) + " " );
		command_exec( temp.toLatin1(), s, command.toUtf8(), 0 );
	}
	qt_entry->clear();
}

