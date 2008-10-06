extern "C" {

	#define USINGANANTIQUECOMPILER 1
	#define QT_NO_PRINTER // XXX: Qt4 API conflict with print()

	// XXX: C => C++ & ekg2 core compatibility issues:
	#define strcasestr strcasestr_ekg
	#define new new_ekg
	#define class class_ekg
	#define private private_ekg

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
	#undef class
	#undef private
	#undef strcasestr
}

