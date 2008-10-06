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

		main_obj->qt_main_window->append("dzień dobry\n");
		return 0;
	}

	static QUERY( qt_setvar_default ) {

		return 0;
	}

	static QUERY( qt_ui_window_switch ) {
		window_t *w = *(va_arg(ap, window_t **));
		return 0;
	}

	static QUERY( qt_ui_window_print ) {
		
		return 0;
	}

	static QUERY( qt_ui_window_new ) {

		return 0;
	}

	static QUERY( qt_ui_window_kill ) {

		return 0;
	}
	
	static QUERY( qt_ui_window_target_changed ) {

		return 0;
	}
	
	static QUERY( qt_ui_window_act_changed ) {
		return 0;
	}
	
	static QUERY( qt_ui_window_refresh ) {

		return 0;
	}

	static QUERY( qt_ui_window_clear ) {

		return 0;
	}
	
	static QUERY( qt_ui_window_lastlog ) {

		return 0;
	}

	static QUERY( qt_ui_refresh ) {

		return 0;
	}

	static QUERY( qt_password_input ) {

		return 0;
	}

	static QUERY( qt_statusbar_query ) {
		window_t *w = *(va_arg(ap, window_t **));
		main_obj->qt_main_window->append( "Session created" );

		return 0;
	}

	static QUERY( qt_binding_set_query ) {

		return 0;
	}

	static QUERY( qt_binding_adddelete_query ) {

		return 0;
	}

	static QUERY( qt_binding_default ) {

		return 0;
	}

	static QUERY( qt_variable_changed ) {

		return 0;
	}

	static QUERY( qt_conference_renamed ) {

		return 0;
	}

	int qt_plugin_init( int prio ) {
		PLUGIN_CHECK_VER( "qt" );
		plugin_register( &qt_plugin, prio );

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
		
		lib = new QApplication( argc, argv );
		main_obj = new Qt4Plugin( "Ekg2" );
	
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

