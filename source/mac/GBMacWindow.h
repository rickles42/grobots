// GBMacWindow.h
// window class for interface
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBMacWindow_h
#define GBMacWindow_h

#include <Windows.h>
#include "GBView.h"

// everything possible is delegated to the window's view.

class GBMacWindow {
protected:
	WindowPtr window;
	GBGraphics graphics;
	GBView * view;
	bool visible;
// internals
	virtual void SetSize(short width, short height);
private:
	GBMacWindow();
public:
// constructor
	GBMacWindow(GBView * newView, bool visible);
	GBMacWindow(GBView * newView, const Point where, bool visible = true);
	virtual ~GBMacWindow();
// window operations
	virtual void Update(bool running);
	virtual void DrawChanges(bool moreChancesSoon);
	void ResizeSelf();
	virtual void Resize(Point where); // handle grow box
	virtual void ZoomIn(); // go to user state
	virtual void ZoomOut(); // go to calculated state
	virtual void Close(); // close box or cmd-W
	virtual void Select();
	virtual void Show();
	virtual void Hide();
	bool Visible() const;
	virtual void AcceptClick(Point where, int clicksBefore);
	virtual void AcceptDrag(Point where);
	virtual void AcceptUnclick(Point where, int clicksBefore);
	virtual void AcceptKeystroke(char what);
	bool GetFrontClicks() const;
	GBCursor Cursor() const;

// statics
public:
	static bool IsMacWindow(WindowPtr wind);
	static GBMacWindow * GetFromWindow(WindowPtr wind);
	// some sort of list of extant windows? (for Window menu)
};


class GBResizableMacWindow : public GBMacWindow {
	bool showGrowBox;
public:
// constructors
	GBResizableMacWindow(GBView * newView, const Point where, bool visible = true, bool showGrow = true);
// window operations
	void Update(bool running);
	void DrawChanges(bool moreChancesSoon);
	void Resize(Point where);
	void ZoomIn(); // go to user state
	void ZoomOut(); // go to calculated state
};

#endif
