// GBViewsApplication.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include <Quickdraw.h>
#include <Events.h>
#include <AppleEvents.h>
#include <Menus.h>
#include <Windows.h>
#include <Dialogs.h>
#include <Menus.h>
#include <ToolUtils.h>
#include <Devices.h>
#include "GBMacWindow.h"
#include "GBErrors.h"
#include "GBViewsApplication.h"
#include <Sound.h>


const short kNumberDialogID = 128;

const short kClickRange = 5;

static OSErr HandleAEOpenApplication(AppleEvent * evt, AppleEvent * reply, long refCon);
static OSErr HandleAEOpenDocuments(AppleEvent * evt, AppleEvent * reply, long refCon);
static OSErr HandleAEPrintDocuments(AppleEvent * evt, AppleEvent * reply, long refCon);
static OSErr HandleAEQuitApplication(AppleEvent * evt, AppleEvent * reply, long refCon);


static OSErr HandleAEOpenApplication(AppleEvent * /*evt*/, AppleEvent * /*reply*/, long refCon) {
	((GBViewsApplication *)refCon)->OpenApp();
	return noErr;
}

static OSErr HandleAEOpenDocuments(AppleEvent * evt, AppleEvent * /*reply*/, long refCon) {
	OSErr err;
	AEDescList list;
	long items, dummySize;
	FSSpec spec;
	AEKeyword dummyKey;
	DescType dummyType;
	
	err = AEGetParamDesc(evt, keyDirectObject, typeAEList, &list);
	if ( err ) return err;		
	err = AECountItems(&list, &items);
	if ( err ) {
		AEDisposeDesc(&list);
		return err;
	}
	for ( long i = 1; i <= items; i ++ ) {
		err = AEGetNthPtr(&list, i, typeFSS, &dummyKey, &dummyType, (Ptr)&spec, sizeof(FSSpec), &dummySize);
		if ( err ) {
			AEDisposeDesc(&list);
			return err;
		}
		((GBViewsApplication *)refCon)->OpenFile(spec);
	}
	return noErr;
}

static OSErr HandleAEPrintDocuments(AppleEvent * /*evt*/, AppleEvent * /*reply*/, long refCon) {
	((GBViewsApplication *)refCon)->OpenApp();
	return noErr;
}

static OSErr HandleAEQuitApplication(AppleEvent * /*evt*/, AppleEvent * /*reply*/, long refCon) {
	((GBViewsApplication *)refCon)->Quit();
	return noErr;
}


void GBViewsApplication::InitToolbox() {
	InitGraf(&qd.thePort);
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitMenus();
	InitDialogs(0L);
	MaxApplZone();
}

void GBViewsApplication::SetupAppleEvents() {
	if ( AEInstallEventHandler(kCoreEventClass, kAEOpenApplication, NewAEEventHandlerUPP(&HandleAEOpenApplication), (long)this, false) )
		NonfatalError("couldn't install AppleEvent handler");
	if ( AEInstallEventHandler(kCoreEventClass, kAEOpenDocuments, NewAEEventHandlerUPP(&HandleAEOpenDocuments), (long)this, false) )
		NonfatalError("couldn't install AppleEvent handler");
	if ( AEInstallEventHandler(kCoreEventClass, kAEPrintDocuments, NewAEEventHandlerUPP(&HandleAEPrintDocuments), (long)this, false) )
		NonfatalError("couldn't install AppleEvent handler");
	if ( AEInstallEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(&HandleAEQuitApplication), (long)this, false) )
		NonfatalError("couldn't install AppleEvent handler");
}

void GBViewsApplication::DoEventLoop() {
	EventRecord event;
	do {
		Process();
		Redraw();
		while ( alive && WaitNextEvent(everyEvent, &event, SleepTime(), nil) )
			HandleEvent(&event);
		if ( dragging && Button() )
			dragging->AcceptDrag(event.where);
		AdjustCursor(event.where);
	} while ( alive );
}

void GBViewsApplication::HandleEvent(EventRecord * evt) {
	try {
		switch ( evt->what ) {
			case mouseDown:
				HandleMouseDown(evt);
				break;
			case mouseUp:
				if ( dragging ) {
					ExpireClicks(evt->where);
					dragging->AcceptUnclick(evt->where, clicks);
					dragging = nil;
				}
				break;
			case keyDown:
			case autoKey:
				HandleKeyDown(evt);
				break;
			case updateEvt:
				HandleUpdate(evt);
				break;
			case kHighLevelEvent:
				if ( AEProcessAppleEvent(evt) )
					NonfatalError("couldn't process AppleEvent");
				break;
			case nullEvent:
				break;
			default:
				break;
		}
	} catch ( GBError & err ) {
		NonfatalError("Error processing event: " + err.ToString());
	}
}

void GBViewsApplication::HandleMouseDown(EventRecord * evt) {
	WindowPtr window;
	switch ( FindWindow(evt->where, &window) ) {
		case inMenuBar:
			AdjustMenus();
			{
				long result = MenuSelect(evt->where);
				HandleMenuSelection(HiWord(result), LoWord(result));
			}
			break;
		case inContent:
			ExpireClicks(evt->where);
			clickTime = TickCount();
			clickPlace = evt->where;
			{
				bool isFront = window == FrontWindow();
				if ( ! isFront )
					SelectWindow(window);
				if ( GBMacWindow::IsMacWindow(window) ) {
					GBMacWindow * mw = GBMacWindow::GetFromWindow(window);
					if ( isFront || mw->GetFrontClicks() ) {
						dragging = mw;
						mw->AcceptClick(evt->where, clicks);
					}
				}
			}
			++ clicks;
			break;
		case inDrag:
			if ( ! evt->modifiers & cmdKey )
				SelectWindow(window);
			DragWindow(window, evt->where, &(qd.screenBits.bounds));
			break;
		case inGoAway:
			if ( TrackGoAway(window, evt->where) )
				if ( GBMacWindow::IsMacWindow(window) )
					GBMacWindow::GetFromWindow(window)->Close();
			break;
		case inGrow:
			if ( GBMacWindow::IsMacWindow(window) )
				GBMacWindow::GetFromWindow(window)->Resize(evt->where);
			break;
		case inZoomIn:
			if ( TrackBox(window, evt->where, inZoomIn) )
				if ( GBMacWindow::IsMacWindow(window) )
					GBMacWindow::GetFromWindow(window)->ZoomIn();
		case inZoomOut:
			if ( TrackBox(window, evt->where, inZoomOut) )
				if ( GBMacWindow::IsMacWindow(window) )
					GBMacWindow::GetFromWindow(window)->ZoomOut();
			break;
		default:
			break;
	}
}

void GBViewsApplication::HandleKeyDown(EventRecord * evt) {
	if ( evt->modifiers & cmdKey ) {
		if ( evt->what == keyDown ) {
			AdjustMenus();
			long result = MenuKey(evt->message & charCodeMask);
			HandleMenuSelection(HiWord(result), LoWord(result));
		}
	} else {
		WindowPtr wind = FrontWindow();
		if ( GBMacWindow::IsMacWindow(wind) )
			GBMacWindow::GetFromWindow(wind)->AcceptKeystroke(evt->message & charCodeMask);
	}
}

void GBViewsApplication::HandleUpdate(EventRecord * evt) {
	WindowPtr window = (WindowPtr)(evt->message);
	if ( GBMacWindow::IsMacWindow(window) )
		GBMacWindow::GetFromWindow(window)->Update(false);
}

void GBViewsApplication::AdjustCursor(Point where) {
	WindowPtr window;
	if ( FindWindow(where, &window) == inContent && GBMacWindow::IsMacWindow(window) ) {
		GBMacWindow * mw = GBMacWindow::GetFromWindow(window);
		if ( window == FrontWindow() || mw && mw->GetFrontClicks() )
			SetCursor(mw->Cursor());
		else
			SetCursor(cuArrow);
	} else
		SetCursor(cuArrow);
}

void GBViewsApplication::ExpireClicks(Point where) {
	if ( clicks && (TickCount() > clickTime + GetDblTime()
			|| abs(where.h - clickPlace.h) > kClickRange
			|| abs(where.v - clickPlace.v) > kClickRange) )
		clicks = 0;
}

GBViewsApplication::GBViewsApplication()
	: alive(true),
	clicks(0), clickTime(0),
	dragging(nil)
{
	InitToolbox();
	clickPlace.h = clickPlace.v = 0;
}

GBViewsApplication::~GBViewsApplication() {}

void GBViewsApplication::Run() {
	SetupAppleEvents();
	InitCursor();
	DoEventLoop();
}

void GBViewsApplication::OpenAppleMenuItem(short menu, short item) {
	Str255 name;
	MenuHandle mHandle = GetMenuHandle(menu);
	GetMenuItemText(mHandle, item, name);
	OpenDeskAcc(name);
}

bool GBViewsApplication::DoNumberDialog(ConstStr255Param prompt, long & value, long min, long max) {
	short itemHit, itemType;
	bool done = false;
	ParamText(prompt, nil, nil, nil);
	DialogPtr dlog = GetNewDialog(kNumberDialogID, nil, (WindowPtr)(-1));
	Handle textBox;
	Rect bounds;
	GetDialogItem(dlog, 4, &itemType, &textBox, &bounds);
	Str255 text;
	NumToString(value, text);
	SetDialogItemText(textBox, text);
	SelectDialogItemText(dlog, 4, 0, -1);
	SetDialogDefaultItem(dlog, 1);
	SetDialogCancelItem(dlog, 2);
	do {
		ModalDialog(nil, &itemHit);
		switch ( itemHit ) {
			case 1: { // ok
				long num;
				GetDialogItemText(textBox, text);
				StringToNum(text, &num);
				if ( num >= min && num <= max ) {
					value = num;
					done = true;
				} else {
					SelectDialogItemText(dlog, 4, 0, -1);
					SysBeep(1);
				}
			} break;
			case 2: // cancel
				done = true; break;
			default: break;
		}
	} while ( ! done );
	DisposeDialog(dlog);
	return itemHit == 1;
}

void GBViewsApplication::SetCursor(GBCursor curs) {
	CursHandle cursor = nil;
	switch ( curs ) {
		case cuArrow: default: break;
		case cuHand: cursor = GetCursor(-20709); break; // might be hand cursor, might not
		case cuCross: cursor = GetCursor(crossCursor); break;
		case cuWait: cursor = GetCursor(watchCursor); break;
	}
	if ( cursor ) ::SetCursor(*cursor);
	else InitCursor();
}

long GBViewsApplication::SleepTime() {
	return -1; // forever; override
}

void GBViewsApplication::AdjustMenus() {} // override

void GBViewsApplication::HandleMenuSelection(short /*menu*/, short /*item*/) {} // override

void GBViewsApplication::Process() {} // override

void GBViewsApplication::Redraw() {} // override

void GBViewsApplication::OpenApp() {} // override if you want to do anything

void GBViewsApplication::OpenFile(FSSpec &) {} // override if you want to do anything

void GBViewsApplication::PrintFile(FSSpec &) {} // override if you want to do anything

void GBViewsApplication::Quit() {
	// override if you want to do anything different
	alive = false;
}
