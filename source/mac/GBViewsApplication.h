// GBViewsApplication.h
// Abstract top-level interface class.
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef ViewsApplication_h
#define ViewsApplication_h

#include <Events.h>
#include "GBView.h"

class GBMacWindow;
class FSSpec;

class GBViewsApplication {
protected:
	bool alive;
	int clicks; // how many mousedowns recently
	long clickTime;
	Point clickPlace;
	GBMacWindow * dragging;
private:
// initialization
	void InitToolbox();
	void SetupAppleEvents();
// events
	void DoEventLoop();
	void HandleEvent(EventRecord * evt);
	void HandleMouseDown(EventRecord * evt);
	void HandleMouseUpOrDrag(EventRecord * evt);
	void HandleKeyDown(EventRecord * evt);
	void HandleUpdate(EventRecord * evt);
	void AdjustCursor(Point where);
	void ExpireClicks(Point where);
public:
	GBViewsApplication();
	virtual ~GBViewsApplication();
// operation
	void Run();
	void DrawAll();
// useful
	static void OpenAppleMenuItem(short menu, short item);
	static bool DoNumberDialog(ConstStr255Param prompt, long & value,
		long min = -1000000000, long max = 1000000000);
	static void SetCursor(GBCursor curs);
protected: // to override
	virtual long SleepTime();
	virtual void AdjustMenus();
	virtual void HandleMenuSelection(short menu, short item);
	virtual void Process(); // do periodic processing
	virtual void Redraw();
public: // more to override
	virtual void OpenApp();
	virtual void OpenFile(FSSpec & file);
	virtual void PrintFile(FSSpec & file);
	virtual void Quit();
};

#endif
