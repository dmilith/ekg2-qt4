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
			#ifdef QT_DEBUG
				cout << "." << flush;
			#endif	
			lib->processEvents();
			ekg_loop();
		}
		qt_plugin_destroy();
		return 0;
	}

	static QUERY( qt_beep ) {
		
		return 0;
	}

	static QUERY( qt_ui_is_initialized ) {
		main_obj->qt_debug_window->append("Ui: Initialized.\n");

		return 0;
	}

	static QUERY( qt_setvar_default ) {
		main_obj->qt_debug_window->append("Ui: Default variables set.\n");

		return 0;
	}

	static QUERY( qt_ui_window_switch ) {
		main_obj->qt_debug_window->append("Ui: Window switched.\n");
		window_t *w = *(va_arg(ap, window_t **));
		return 0;
	}

	static QUERY( qt_ui_window_print ) {
		main_obj->qt_debug_window->append("Ui: Window print.\n");
		
		return 0;
	}

	static QUERY( qt_ui_window_new ) {
		main_obj->qt_debug_window->append("Ui: New window.\n");

		return 0;
	}

	static QUERY( qt_ui_window_kill ) {
		main_obj->qt_debug_window->append("Ui: Killed window.\n");

		return 0;
	}
	
	static QUERY( qt_ui_window_target_changed ) {
		main_obj->qt_debug_window->append("Ui: Window target changed.\n");

		return 0;
	}
	
	static QUERY( qt_ui_window_act_changed ) {
		main_obj->qt_debug_window->append("Ui: Actual window changed.\n");
		return 0;
	}
	
	static QUERY( qt_ui_window_refresh ) {
		main_obj->qt_debug_window->append("Ui: Window refresh.\n");

		return 0;
	}

	static QUERY( qt_ui_window_clear ) {
		main_obj->qt_debug_window->append("Ui: Window clear.\n");

		return 0;
	}
	
	static QUERY( qt_ui_window_lastlog ) {
		main_obj->qt_debug_window->append("Ui: Window lastlog.\n");

		return 0;
	}

	static QUERY( qt_ui_refresh ) {
		main_obj->qt_debug_window->append("Ui: Refresh.\n");

		return 0;
	}

	static QUERY( qt_password_input ) {
		main_obj->qt_debug_window->append("Ui: Password input.\n");

		return 0;
	}

	static QUERY( qt_statusbar_query ) {
		window_t *w = *(va_arg(ap, window_t **));
		main_obj->qt_debug_window->append( "Ui: Statusbar query.\n" );

		return 0;
	}

	static QUERY( qt_binding_set_query ) {
		main_obj->qt_debug_window->append("Ui: Binding query set (?).\n");

		return 0;
	}

	static QUERY( qt_binding_adddelete_query ) {
		main_obj->qt_debug_window->append("Ui: Binding add/delete (?).\n");

		return 0;
	}

	static QUERY( qt_binding_default ) {
		main_obj->qt_debug_window->append("Ui: Default binding (?).\n");

		return 0;
	}

	static QUERY( qt_variable_changed ) {
		main_obj->qt_debug_window->append("Ui: Variable changed.\n");

		return 0;
	}

	static QUERY( qt_conference_renamed ) {
		main_obj->qt_debug_window->append("Ui: Conference renamed.\n");

		return 0;
	}

	int qt_plugin_init( int prio ) {
		PLUGIN_CHECK_VER( "qt" );
		plugin_register( &qt_plugin, prio );
		lib = new QApplication( argc, argv );
		main_obj = new Qt4Plugin( "Ekg2" );
		
		main_obj->qt_debug_window->append("Ui: Plugin initialized.\n");

		query_connect_id( &qt_plugin, UI_IS_INITIALIZED, qt_ui_is_initialized, NULL );
		query_connect_id( &qt_plugin, UI_LOOP, qt_plugin_loop, NULL );
		query_connect_id( &qt_plugin, SET_VARS_DEFAULT, qt_setvar_default, NULL );
		query_connect_id( &qt_plugin, UI_BEEP, qt_beep, NULL );
		query_connect_id( &qt_plugin, UI_WINDOW_SWITCH, qt_ui_window_switch, NULL );
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
		
	
		#ifdef QT_DEBUG
			command_exec( NULL, NULL, "/session -a ircnet", 0 );
			command_exec( NULL, NULL, "/session nickname qtdmil", 0 );
			command_exec( NULL, NULL, "/session server warszawa.irc.pl", 0 );
			command_exec( NULL, NULL, "/connect", 0 );
			command_exec( NULL, NULL, "/set quit_save 0", 0 );
		#endif


		return 0;
	}

	int qt_plugin_destroy() {
		if ( main_obj ) delete main_obj;
		if ( lib ) delete lib;
    	plugin_unregister( &qt_plugin );
		#ifdef QT_DEBUG
			cout << "\nDEBUG: main_obj deleted\nTrying to quit..\n" << flush;
		#endif
		ekg_exit(); // XXX: shouldn't be here, It should be done automagically, but without it ekg2 segvs
   	return 0;
	}

}

