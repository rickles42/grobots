// GBMacWindow.cpp
// code for the Mac window class
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include <Windows.h>
#include <ToolUtils.h>
#include "GBMacWindow.h"
#include "GBErrors.h"
#include "GBStringUtilities.h"

const short kMacWindowKind = 190;


GBMacWindow::GBMacWindow(GBView * newView, bool vis)
	: view(newView), window(nil), visible(vis)
{
	view->SetGraphics(&graphics);
}

GBMacWindow::GBMacWindow(GBView * newView, const Point where, bool vis)
	: view(newView),
	window(nil),
	visible(vis)
{
	Str255 s;
	Rect bounds;
	bounds.left = where.h;
	bounds.top = where.v;
	bounds.right = bounds.left + view->PreferredWidth();
	bounds.bottom = bounds.top + view->PreferredHeight();
	ToPascalString(view->Name(), s);
	window = NewCWindow(nil, &bounds, s, vis,
			noGrowDocProc, (WindowPtr)(-1), true, (long)this);
	if ( ! window ) throw GBOutOfMemoryError();
	view->SetBounds(window->portRect);
	((CWindowPeek)window)->windowKind = kMacWindowKind;
	view->SetGraphics(&graphics);
}

GBMacWindow::~GBMacWindow() {
	delete view;
	DisposeWindow(window);
}

void GBMacWindow::SetSize(short width, short height) {
	view->SetSize(width, height);
	SizeWindow(window, view->Width(), view->Height(), true);
}

void GBMacWindow::Update(bool running) {
	GrafPtr savePort;
	GetPort(&savePort);
	SetPort(window);
	if ( view->NeedsResize() )
		ResizeSelf();
	if ( view->NeedsRedraw(running) ) {
		BeginUpdate(window);
		EndUpdate(window);
		view->DoDraw();
	} else {
		BeginUpdate(window);
		view->DoDraw();
		EndUpdate(window);
	}
	SetPort(savePort);
}

void GBMacWindow::DrawChanges(bool running) {
	if ( view->NeedsResize() )
		ResizeSelf();
	if ( visible && view->NeedsRedraw(running) ) {
		GrafPtr savePort;
		GetPort(&savePort);
		SetPort(window);
		view->DoDraw();
		SetPort(savePort);
	}
}

void GBMacWindow::ResizeSelf() {
	SetSize(view->PreferredWidth(), view->PreferredHeight());
}

void GBMacWindow::Resize(Point) {}

void GBMacWindow::ZoomIn() {}
void GBMacWindow::ZoomOut() {}

void GBMacWindow::Close() {
	Hide();
	// may need to vary
}

void GBMacWindow::Select() {
	if ( ! visible && view->NeedsResize() )
		ResizeSelf();
	ShowWindow(window);
	SelectWindow(window);
	visible = true;
}

void GBMacWindow::Show() {
	if ( view->NeedsResize() )
		ResizeSelf();
	ShowWindow(window);
	visible = true;
}

void GBMacWindow::Hide() {
	HideWindow(window);
	visible = false;
}

bool GBMacWindow::Visible() const {
	return visible;
}

void GBMacWindow::AcceptClick(Point where, int clicksBefore) {
	GrafPtr savePort;
	GetPort(&savePort);
	SetPort(window);
	GlobalToLocal(&where);
	view->DoClick(where.h, where.v, clicksBefore);
	SetPort(savePort);
}

void GBMacWindow::AcceptDrag(Point where) {
	GrafPtr savePort;
	GetPort(&savePort);
	SetPort(window);
	GlobalToLocal(&where);
	view->DoDrag(where.h, where.v);
	SetPort(savePort);
}

void GBMacWindow::AcceptUnclick(Point where, int clicksBefore) {
	GrafPtr savePort;
	GetPort(&savePort);
	SetPort(window);
	GlobalToLocal(&where);
	view->DoUnclick(where.h, where.v, clicksBefore);
	SetPort(savePort);
}

void GBMacWindow::AcceptKeystroke(char what) {
	view->AcceptKeystroke(what);
}


bool GBMacWindow::GetFrontClicks() const {
	return view->GetFrontClicks();
}

GBCursor GBMacWindow::Cursor() const {
	return view->Cursor();
}

bool GBMacWindow::IsMacWindow(WindowPtr wind) {
	if ( ! wind ) return false;
	return (((CWindowPeek)wind)->windowKind == kMacWindowKind);
}

GBMacWindow * GBMacWindow::GetFromWindow(WindowPtr wind) {
	if ( ! wind ) return nil;
	return (GBMacWindow *)(((CWindowPeek)wind)->refCon);
}


GBResizableMacWindow::GBResizableMacWindow(GBView * newView, const Point where, bool vis, bool showGrow)
	: GBMacWindow(newView, vis), showGrowBox(showGrow)
{
	Str255 s;
	Rect bounds;
	bounds.left = where.h;
	bounds.top = where.v;
	bounds.right = bounds.left + view->PreferredWidth();
	bounds.bottom = bounds.top + view->PreferredHeight();
	ToPascalString(view->Name(), s);
	window = NewCWindow(nil, &bounds, s, vis,
		zoomDocProc, (WindowPtr)(-1), true, (long)this);
	if ( ! window ) throw GBOutOfMemoryError();
	view->SetBounds(window->portRect);
	((CWindowPeek)window)->windowKind = kMacWindowKind;
}

void GBResizableMacWindow::Update(bool running) {
	GrafPtr savePort;
	GetPort(&savePort);
	SetPort(window);
	if ( view->NeedsResize() )
		ResizeSelf();
	if ( view->NeedsRedraw(running) ) {
		BeginUpdate(window);
		EndUpdate(window);
		if ( showGrowBox ) DrawGrowIcon(window);
		view->DoDraw();
	} else {
		BeginUpdate(window);
			if ( showGrowBox ) DrawGrowIcon(window);
			view->DoDraw();
		EndUpdate(window);
	}
	SetPort(savePort);
}

void GBResizableMacWindow::DrawChanges(bool running) {
	if ( view->NeedsResize() )
		ResizeSelf();
	if ( visible && view->NeedsRedraw(running) ) {
		GrafPtr savePort;
		GetPort(&savePort);
		SetPort(window);
		view->DoDraw();
		SetPort(savePort);
	}
}

void GBResizableMacWindow::Resize(Point where) {
	Rect sizeRect;
	SetRect(&sizeRect, view->MinimumWidth(), view->MinimumHeight(), view->MaximumWidth(), view->MaximumHeight());
	long result = GrowWindow(window, where, &sizeRect);
	if ( result )
		SetSize(LoWord(result), HiWord(result));
}

void GBResizableMacWindow::ZoomIn() {
	ZoomWindow(window, inZoomIn, false);
	//view->SetSize(GetSize());
}

void GBResizableMacWindow::ZoomOut() {
	//ZoomWindow(window, inZoomOut, false);
	ResizeSelf();
}

