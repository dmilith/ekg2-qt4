

#define USINGANANTIQUECOMPILER 1

extern "C" {

	#define new new_ekg
	#include "../../ekg/debug.h"
	#include "../../ekg/plugins.h"
	#include "../../ekg/queries.h"
	#undef new
	PLUGIN_DEFINE( qt, PLUGIN_UI, NULL );

}

#include "qt4_window.h"

using namespace Ui;

int argc;
char** argv;
QApplication lib( argc, argv );
Qt4Plugin *main_obj = NULL;

extern "C" {

	extern void ekg_exit();
	extern void ekg_loop();

	static QUERY( qt_plugin_loop ) {
		while ( main_obj ) {
			printf(".");
			fflush( stdout );
			ekg_loop();
			lib.processEvents();
		}
		return 0;
	}

	static QUERY( qt_ui_is_initialized ) {
		if (! main_obj) main_obj = new Qt4Plugin( "Ekg2" );
		return 0;
	}

	int qt_plugin_init( int prio ) {
		PLUGIN_CHECK_VER( "qt" );
		plugin_register( &qt_plugin, prio );

		query_connect_id( &qt_plugin, UI_IS_INITIALIZED, qt_ui_is_initialized, NULL );
		query_connect_id( &qt_plugin, UI_LOOP, qt_plugin_loop, NULL );

		return 0;
	}

	int qt_plugin_destroy() {
		//if (main_obj) delete main_obj;
    	plugin_unregister( &qt_plugin );
		// dead code
   	return 0;
	}

}

