// GBMain.cpp
// main for Grobots with GBApplication.
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBApplication.h"
#include "GBErrors.h"
#include <exception>

#if MAC
	QDGlobals qd;
#endif

void main() {
	try {
		GBApplication app;
		
		app.Run();
	} catch ( GBError & err ) {
		FatalError("Uncaught GBError: " + err.ToString());
	} catch ( GBRestart & r ) {
		FatalError("Uncaught GBRestart: " + r.ToString());
	} catch ( std::exception & e ) {
		FatalError("Uncaught std::exception: " + string(e.what()));
	} catch ( ... ) {
		FatalError("Uncaught mystery exception.");
	}
}

// that was short.
