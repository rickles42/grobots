// GBApplication.h
// Top-level interface class.
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBApplication_h
#define GBApplication_h


#include "GBViewsApplication.h"
#include "GBTypes.h"
#include "GBMilliseconds.h"
#include "GBWorld.h"


class GBObject;
class GBRobot;
class GBPortal;
class GBMiniMapView;
class GBMacWindow;
class GBDebuggerView;

class GBApplication : public GBViewsApplication {
	GBWorld world;
	GBMilliseconds speedLimit;
	GBMilliseconds lastFrame;
	GBMilliseconds lastAutofollow;
// the views and windows
	GBPortal * portal;
	GBMiniMapView * minimap;
	GBDebuggerView * debugger;
	GBMacWindow * mainWindow;
	GBMacWindow * minimapWindow;
	GBMacWindow * statusWindow;
	GBMacWindow * rosterWindow;
	GBMacWindow * scoresWindow;
	GBMacWindow * typeWindow;
	GBMacWindow * aboutWindow;
	GBMacWindow * inspectorWindow;	
	GBMacWindow * tournamentWindow;	
	GBMacWindow * debuggerWindow;	
	GBMacWindow * sideDebuggerWindow;
	void SetupMenus();
	void DoLoadSide();
	void DoReloadSide();
	void DoRulesDialog();
public:
	GBApplication();
	~GBApplication();
	
	long SleepTime();
	void AdjustMenus();
	void HandleMenuSelection(short menu, short item);
	void Process();
	void Redraw();
	void OpenFile(FSSpec & file);
};

#endif
