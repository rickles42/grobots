// GBMilliseconds.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBMilliseconds.h"
#include "GBPlatform.h"

#if MAC
	#include <Events.h>
#endif

#if MAC
GBMilliseconds Milliseconds() {
	return (GBMilliseconds)(TickCount()) * 1000 / 60;
}
#else
	#warning "Need Milliseconds"
#endif

// This file is small and lonely. Other stuff could go in it too.
