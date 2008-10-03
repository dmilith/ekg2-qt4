

#define USINGANANTIQUECOMPILER 1

extern "C" {

	#define new new_ekg
	#include "../../ekg/debug.h"
	#include "../../ekg/plugins.h"
	#undef new
	PLUGIN_DEFINE( qt4, PLUGIN_UI, NULL );

}

#include "qt4_window.h"

using namespace Ui;

int argc;
char** argv;
QApplication lib( argc, argv );
Qt4Plugin *main_obj;

void
res_init() {
		Q_INIT_RESOURCE( ekg2_qt4 );
}

extern "C" {

	extern void ekg_exit();

	int qt4_plugin_init( int prio ) {
   	PLUGIN_CHECK_VER( "qt4" );
		plugin_register( &qt4_plugin, prio );
    	main_obj = new Qt4Plugin( "Ekg2" );
		return lib.exec();
	}

	int qt4_plugin_destroy() {
		if (main_obj) delete main_obj;
    	plugin_unregister( &qt4_plugin );
		ekg_exit();
		// dead code
   	return 0;
	}

}

