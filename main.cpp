/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */

#include <iostream>

#include "ekg2_includes.h"
#include "qtplugin_config.h"
#include "qt4_window.h"
#include "qt4_config_window.h"

extern "C" {
	PLUGIN_DEFINE( qt, PLUGIN_UI, NULL );
}

using namespace Ui;
using namespace std;

int argc;
char** argv;
QApplication *lib = NULL;
static Qt4Plugin *main_obj = NULL;

extern "C" {
	extern void ekg_exit();
	extern void ekg_loop();

	static QUERY( qt_plugin_loop ) {
		while ( main_obj->is_alive() ) {
			lib->processEvents();
			ekg_loop();
		}
		qt_plugin_destroy();
		return 0;
	}

	static QUERY( qt_beep ) {
		lib->beep();
		return 0;
	}

	static QUERY( qt_ui_is_initialized ) {
		main_obj->qt_debug_window->append("Ui: Initialized.");
		return -1;
	}

	static QUERY( qt_setvar_default ) {
		main_obj->qt_debug_window->append("Ui: Default variables set.");
		return 0;
	}

	static QUERY( qt_ui_window_switch ) {
		window_t *w = *(va_arg(ap, window_t **));
		main_obj->qt_debug_window->append("Ui: Window switched.");
/*		main_obj->tabs->addTab( new QWidget, QString("new window") );
		main_obj->set_current_window( main_obj->get_current_window() + 1 );
		main_obj->tabs->setCurrentIndex( main_obj->get_current_window() );*/
		main_obj->qt_debug_window->append("Ui: Current window: " + QString::number( main_obj->tabs->currentIndex(), 10 ) );
		return 0;
	}

	static QUERY( qt_ui_window_print ) {
		window_t *w = *(va_arg(ap, window_t **));
		fstring_t *line = *(va_arg(ap, fstring_t **));
		wchar_t* z = line->str.w;
		main_obj->qt_debug_window->append( "Ui: Window print." );
		main_obj->tabs->setCurrentIndex( main_obj->tabs->currentIndex() );
		main_obj->current_window_number = QString::number( main_obj->tabs->currentIndex() );

		QList<QTextBrowser *> all_browsers = main_obj->tabs->findChildren<QTextBrowser *>();
		while ( !all_browsers.empty() ) {	// find all text browsers and look for one with specified name 
														// ( here window name name => number )
			if ( ( (QTextBrowser*)all_browsers[ all_browsers.count() - 1 ] )->objectName() == main_obj->current_window_number ) {
				( ( QTextBrowser*)all_browsers.takeLast() )->append( QString::fromUtf8( (char*)z ).toUtf8() ); // XXX
					break;
			} else {
				main_obj->qt_status_window->append( QString::fromUtf8( (char*)z ).toUtf8() );
				all_browsers.takeLast();
			}
		}
		return 0;
	}

	static QUERY( qt_ui_window_new ) {
		window_t **w = va_arg(ap, window_t **);
		// TODO: should open new window here with *w
		main_obj->qt_debug_window->append("Ui: New window." );
		return 0;
	}

	static QUERY( qt_ui_window_kill ) {
		window_t **w = va_arg(ap, window_t **);
		// TODO: should kill actual window *w
		// FIXME: this should be done after /window kill ;f but it is not. why?
		main_obj->qt_debug_window->append("Ui: Killed window."); 
		return 0;
	}
	
	static QUERY( qt_ui_window_target_changed ) {
		window_t **w = va_arg(ap, window_t **);
		main_obj->qt_debug_window->append("Ui: Window target changed.");
		return 0;
	}
	
	static QUERY( qt_ui_window_act_changed ) { // called when somebody will talk to us.
		window_t **w = va_arg(ap, window_t **);
		window_t *win = *w;
		QString target_name = QString( win->target );
		main_obj->qt_debug_window->append("Ui: Actual window changed.");
		main_obj->new_window( target_name );
		return 0;
	}
	
	static QUERY( qt_ui_window_refresh ) {
		window_t **w = va_arg(ap, window_t **);
		main_obj->qt_debug_window->append("Ui: Window refresh.");
		return 0;
	}

	static QUERY( qt_ui_window_clear ) {
		window_t **w = va_arg(ap, window_t **);
		main_obj->qt_debug_window->clear();
		main_obj->qt_debug_window->append("Ui: Window cleared.");
		return 0;
	}
	
	static QUERY( qt_ui_window_lastlog ) {
		main_obj->qt_debug_window->append("Ui: Window lastlog.");
		return 0;
	}

	static QUERY( qt_ui_refresh ) {
		main_obj->qt_debug_window->append("Ui: Refresh.");
		main_obj->update();
		return 0;
	}

	static QUERY( qt_password_input ) {
		main_obj->qt_debug_window->append("Ui: Password input.");
		return 0;
	}

	static QUERY( qt_statusbar_query ) {
		window_t *w = *(va_arg(ap, window_t **));
		main_obj->qt_debug_window->append( "Ui: Statusbar query." );
		return 0;
	}

	static QUERY( qt_binding_set_query ) {
		main_obj->qt_debug_window->append("Ui: Binding query set (?).");
		return 0;
	}

	static QUERY( qt_binding_adddelete_query ) {
		main_obj->qt_debug_window->append("Ui: Binding add/delete (?).");
		return 0;
	}

	static QUERY( qt_binding_default ) {
		main_obj->qt_debug_window->append("Ui: Default binding (?).");
		return 0;
	}

	static QUERY( qt_variable_changed ) {
		main_obj->qt_debug_window->append("Ui: Variable changed.");
		return 0;
	}

	static QUERY( qt_conference_renamed ) {
		main_obj->qt_debug_window->append("Ui: Conference renamed.");
		return 0;
	}

	static QUERY( qt_all_contacts_changed ) {
		main_obj->qt_debug_window->append("Ui: Userlist changed.");
		session_t *s;
		userlist_t *ul;
		if ( !session_current )
			return 0;
		s = session_current;
		main_obj->qt_userlist->clear(); // will prevent from multiple loading on list
		for ( ul = s->userlist; ul; ul = ul->next ) {
			userlist_t *u = ul;
				if ( QString( u->descr ) != QString( "" ) ) { // got descr?
					main_obj->qt_userlist->addItem( " " +  QString( u->nickname ) + "\n (" + QString( u->descr ) + ") " );
				} else { // no descr
					main_obj->qt_userlist->addItem( " " +  QString( u->nickname ) );
				}
		}
		return 0;
	}

	int qt_plugin_init( int prio ) {
		PLUGIN_CHECK_VER( "qt" );
		plugin_register( &qt_plugin, prio );
		lib = new QApplication( argc, argv );
		main_obj = new Qt4Plugin( "Ekg2" );
		// two tabs are static: qt_debug_window and qt_status_window. They cannot be closed (so far).
		main_obj->tabs->setTabEnabled( 0, false ); // tab[0] will always be debug window
		query_connect_id( &qt_plugin, UI_IS_INITIALIZED, qt_ui_is_initialized, NULL );
		query_connect_id( &qt_plugin, UI_LOOP, qt_plugin_loop, NULL );
		query_connect_id( &qt_plugin, SET_VARS_DEFAULT, qt_setvar_default, NULL );
		query_connect_id( &qt_plugin, UI_BEEP, qt_beep, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_SWITCH, qt_ui_window_switch, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_NEW, qt_ui_window_new, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_PRINT, qt_ui_window_print, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_KILL, qt_ui_window_kill, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_TARGET_CHANGED, qt_ui_window_target_changed, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_ACT_CHANGED, qt_ui_window_act_changed, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_REFRESH, qt_ui_window_refresh, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_CLEAR, qt_ui_window_clear, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_UPDATE_LASTLOG, qt_ui_window_lastlog, NULL );
		query_connect_id( &qt_plugin, UI_REFRESH, qt_ui_refresh, NULL );
		query_connect_id( &qt_plugin, UI_PASSWORD_INPUT, qt_password_input, NULL );
		query_connect_id( &qt_plugin, SESSION_ADDED, qt_statusbar_query, NULL );
		query_connect_id( &qt_plugin, SESSION_REMOVED, qt_statusbar_query, NULL );
		query_connect_id( &qt_plugin, SESSION_EVENT, qt_statusbar_query, NULL );
		query_connect_id( &qt_plugin, SESSION_RENAMED, qt_statusbar_query, NULL );
		query_connect_id( &qt_plugin, BINDING_SET, qt_binding_set_query, NULL );
		query_connect_id( &qt_plugin, BINDING_COMMAND, qt_binding_adddelete_query, NULL );
		query_connect_id( &qt_plugin, BINDING_DEFAULT, qt_binding_default, NULL );
		query_connect_id( &qt_plugin, VARIABLE_CHANGED, qt_variable_changed, NULL );
		query_connect_id( &qt_plugin, CONFERENCE_RENAMED, qt_conference_renamed, NULL );
		
		// userlist
		query_connect_id( &qt_plugin, UI_REFRESH, qt_all_contacts_changed, (void *) 1 );
		query_connect_id( &qt_plugin, USERLIST_REFRESH, qt_all_contacts_changed, NULL /* ? */ );
		query_connect_id( &qt_plugin, SESSION_CHANGED, qt_all_contacts_changed, (void *) 1 );
		query_connect_id( &qt_plugin, SESSION_EVENT, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, METACONTACT_ADDED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, METACONTACT_REMOVED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, METACONTACT_ITEM_ADDED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, METACONTACT_ITEM_REMOVED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, USERLIST_CHANGED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, USERLIST_ADDED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, USERLIST_REMOVED, qt_all_contacts_changed, NULL );
		query_connect_id( &qt_plugin, USERLIST_RENAMED, qt_all_contacts_changed, NULL );
		return 0;
	}

	int qt_plugin_destroy() {
		if ( main_obj ) delete main_obj;
		if ( lib ) delete lib;
    	plugin_unregister( &qt_plugin );
		#ifdef QT_DEBUG
			cout << "DEBUG: main_obj deleted.\n" << flush;
		#endif
		ekg_exit(); // XXX: shouldn't be here, It should be done automagically, but without it ekg2 segvs
   	return 0;
	}

}

