/*
 * author: Daniel (dmilith) Dettlaff
 * email: dmilith at drakor.eu
 * released under GPLv2 license
 * © 2oo8
 */

#define USINGANANTIQUECOMPILER 1
#define QT_NO_PRINTER // XXX: print()

extern "C" {

	#define new new_ekg
	#include "ekg2-config.h"
	#include <ekg/debug.h>
	#include <ekg/plugins.h>
	#include <ekg/queries.h>
	#include <ekg/sessions.h>
	#include <ekg/events.h>
	#include <ekg/configfile.h>
	#include <ekg/dynstuff.h>
	#include <ekg/log.h>
	#include <ekg/metacontacts.h>
	#include <ekg/msgqueue.h>
	#include <ekg/protocol.h>
	#include <ekg/stuff.h>
	#include <ekg/themes.h>
	#include <ekg/vars.h>
	#include <ekg/userlist.h>
	#include <ekg/scripts.h>
	#include <ekg/windows.h>
	#include <ekg/xmalloc.h>
	#include <ekg/net.h>
	#include <ekg/queries.h>
	#include <ekg/dynstuff_inline.h>
	#include <ekg/bindings.h>
	#include <ekg/plugins.h>
	#include <ekg/commands.h>
	#undef new
	PLUGIN_DEFINE( qt, PLUGIN_UI, NULL );
}

#include "qt4_window.h"

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

	static QUERY( qt_ui_is_initialized ) {
		command_exec( NULL, NULL, "/session -a ircnet", 0 );
		command_exec( NULL, NULL, "/session nickname qtdmil", 0 );
		command_exec( NULL, NULL, "/session server warszawa.irc.pl", 0 );
		command_exec( NULL, NULL, "/connect", 0 );
		return 0;
	}

	int qt_plugin_init( int prio ) {
		PLUGIN_CHECK_VER( "qt" );
		plugin_register( &qt_plugin, prio );

		query_connect_id( &qt_plugin, UI_IS_INITIALIZED, qt_ui_is_initialized, NULL );
		query_connect_id( &qt_plugin, UI_LOOP, qt_plugin_loop, NULL );
		
		lib = new QApplication( argc, argv );
		main_obj = new Qt4Plugin( "Ekg2" );

		return 0;
	}

	int qt_plugin_destroy() {
		if ( main_obj ) delete main_obj;
		if ( lib ) delete lib;
    	plugin_unregister( &qt_plugin );
		#ifdef QT_DEBUG
			cout << "\nDEBUG: main_obj deleted\nTrying to quit..\n" << flush;
		#endif
		ekg_exit();
   	return 0;
	}

}

