// GBApplication.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBApplication.h"
#include "GBWorld.h"
#include "GBSide.h"
#include "GBMacWindow.h"
#include "GBPortal.h"
#include "GBMiniMap.h"
#include "GBRosterView.h"
#include "GBStatusView.h"
#include "GBAboutBox.h"
#include "GBScoresView.h"
#include "GBRobotTypeView.h"
#include "GBInspectorView.h"
#include "GBDebugger.h"
#include "GBSideDebugger.h"
#include "GBTournamentView.h"
#include <StandardFile.h>
#include "GBSideReader.h"
#include "GBSound.h"
#include <Sound.h>
#include <NumberFormatting.h>
#include "GBStringUtilities.h"
#include <Dialogs.h>
#include <Controls.h>


// constants

enum {
	kAppleMenu = 128,
		miAbout = 1,
	kFileMenu = 129,
		miLoadSide = 1, miDuplicateSide = 2,
		miReloadSide = 3,
		miRemoveSide = 5, miRemoveAllSides = 6,
		miClose = 8,
		miQuit = 10,
	kWindowMenu = 130,
		miRosterView = 1, miMainView, miMinimapView, miStatusView,
		miScoresView, miTypesView,
		miInspectorView, miDebuggerView,
		miTournamentView, miSideDebuggerView,
	kViewMenu = 133,
		miSound = 1,
		miShowSensors = 3, miShowDecorations, miShowMeters,
		miMinimapRobots = 7, miMinimapFood, miMinimapSensors, miMinimapDecorations,
		miMinimapTrails,
		miReportErrors = 13, miReportPrints,
		miRefollow = 16, miFollowRandom, miRandomNear, miAutofollow,
	kSimulationMenu = 131,
		miRun = 1, miSingleFrame, miStep, miPause,
		miSlowSpeed = 6, miNormalSpeed, miFastSpeed, miUnlimitedSpeed,
		miNewRound = 11, miRestart,
		miSeed = 13, miReseed,
		miRules = 16,
		miTournament = 17, miResetScores,
		miStartStopBrain = 20,
	kToolsMenu = 132,
		miScroll = 1,
		miAddManna = 3, miAddRobot, miAddSeed,
		miMove = 7, miPull, miSmite, miBlasts, miErase, miEraseBig
};

const short kRulesDialogID = 129;

const GBMilliseconds kSlowSpeedLimit = 100;
const GBMilliseconds kNormalSpeedLimit = 33;
const GBMilliseconds kFastSpeedLimit = 17;
const GBMilliseconds kNoSpeedLimit = 0;

const GBMilliseconds kMaxEventInterval = 50;
const GBMilliseconds kAutofollowInterval = 3000;

void GBApplication::SetupMenus() {
	MenuHandle currentMenu;
	currentMenu = GetMenu(kAppleMenu);
	AppendResMenu(currentMenu, 'DRVR');
	InsertMenu(currentMenu, 0);
	currentMenu = GetMenu(kFileMenu);
	InsertMenu(currentMenu, 0);
	currentMenu = GetMenu(kWindowMenu);
	InsertMenu(currentMenu, 0);
	currentMenu = GetMenu(kSimulationMenu);
	InsertMenu(currentMenu, 0);
	currentMenu = GetMenu(kViewMenu);
	InsertMenu(currentMenu, 0);
	currentMenu = GetMenu(kToolsMenu);
	InsertMenu(currentMenu, 0);
	// help items...?
	DrawMenuBar();
}

void GBApplication::DoLoadSide() {
// Open... box
	SFTypeList types;
	types[0] = 'TEXT';
	StandardFileReply reply;
	StandardGetFile(nil, 1, types, &reply);
	if ( ! reply.sfGood ) return;
	OpenFile(reply.sfFile);
}

void GBApplication::DoReloadSide() {
	GBSide * oldSide = world.SelectedSide();
	if ( ! oldSide ) return;
	GBSide * newSide = GBSideReader::Load(oldSide->file);
	if ( newSide ) {
		if ( oldSide->Scores().Seeded() ) {
			world.Reset();
			world.running = false;
		}
		world.ReplaceSide(oldSide, newSide);
	}
}

void GBApplication::DoRulesDialog() {
// item IDs
	const short kOKButton = 1;
	const short kCancelButton = 2;
	const short kWorldWidthBox = 3;
	const short kWorldHeightBox = 4;
	const short kMannaSizeBox = 5;
	const short kMannaDensityBox = 6;
	const short kMannaRateBox = 7;
	const short kSeedLimitBox = 8;
	const short kAutoReseedCheckbox = 9;
	const short kSeedValueBox = 10;
	const short kSeedTypePenaltyBox = 11;
	const short kTimeLimitCheckbox = 12;
	const short kTimeLimitBox = 13;
	const short kStopOnEliminationCheckbox = 14;
// put up dialog
	bool done = false;
	short itemHit, itemType;
	Handle item;
	Rect bounds;
	Str255 text;
	DialogPtr dlog = GetNewDialog(kRulesDialogID, nil, (WindowPtr)(-1));
	SetDialogDefaultItem(dlog, kOKButton);
	SetDialogCancelItem(dlog, kCancelButton);
	SetDialogTracksCursor(dlog, true);
// fill in current values
	bool autoReseedCheck = world.autoReseed;
	bool timeLimitCheck = world.timeLimit > 0;
	bool stopOnEliminationCheck = world.stopOnElimination;
	GetDialogItem(dlog, kWorldWidthBox, &itemType, &item, &bounds);
	NumToString(world.BackgroundTilesX(), text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kWorldHeightBox, &itemType, &item, &bounds);
	NumToString(world.BackgroundTilesY(), text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kMannaSizeBox, &itemType, &item, &bounds);
	ToPascalString(ToString(world.mannaSize), text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kMannaDensityBox, &itemType, &item, &bounds);
	ToPascalString(ToString(world.mannaDensity),text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kMannaRateBox, &itemType, &item, &bounds);
	ToPascalString(ToString(world.mannaRate),text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kSeedLimitBox, &itemType, &item, &bounds);
	NumToString(world.seedLimit, text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kAutoReseedCheckbox, &itemType, &item, &bounds);
	SetControlValue((ControlHandle)item, autoReseedCheck);
	GetDialogItem(dlog, kSeedValueBox, &itemType, &item, &bounds);
	ToPascalString(ToString(world.seedValue),text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kSeedTypePenaltyBox, &itemType, &item, &bounds);
	ToPascalString(ToString(world.seedTypePenalty),text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kTimeLimitCheckbox, &itemType, &item, &bounds);
	SetControlValue((ControlHandle)item, timeLimitCheck);
	GetDialogItem(dlog, kTimeLimitBox, &itemType, &item, &bounds);
	NumToString(world.timeLimit > 0 ? world.timeLimit : 18000, text);
	SetDialogItemText(item, text);
	GetDialogItem(dlog, kStopOnEliminationCheckbox, &itemType, &item, &bounds);
	SetControlValue((ControlHandle)item, stopOnEliminationCheck);
// do dialog
	SelectDialogItemText(dlog, kWorldWidthBox, 0, -1);
	do {
		ModalDialog(nil, &itemHit);
		switch ( itemHit ) {
			case kOKButton: {
			// get and validate values:
			// world size
				long worldWidth;
				GetDialogItem(dlog, kWorldWidthBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				StringToNum(text, &worldWidth);
				if ( worldWidth < 2 || worldWidth > 50 ) {
					SelectDialogItemText(dlog, kWorldWidthBox, 0, -1);
					SysBeep(1);
					break;
				}
				long worldHeight;
				GetDialogItem(dlog, kWorldHeightBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				StringToNum(text, &worldHeight);
				if ( worldHeight < 2 || worldHeight > 50 ) {
					SelectDialogItemText(dlog, kWorldHeightBox, 0, -1);
					SysBeep(1);
					break;
				}
			// manna size
				GBNumber mannaSize;
				GetDialogItem(dlog, kMannaSizeBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				if ( ! ParseNumber(FromPascalString(text), mannaSize) || mannaSize < 10 ) {
					SelectDialogItemText(dlog, kMannaSizeBox, 0, -1);
					SysBeep(1);
					break;
				}
			// manna density
				GBNumber mannaDensity;
				GetDialogItem(dlog, kMannaDensityBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				if ( ! ParseNumber(FromPascalString(text), mannaDensity) || mannaDensity < 0 ) {
					SelectDialogItemText(dlog, kMannaDensityBox, 0, -1);
					SysBeep(1);
					break;
				}
			// manna rate
				GBNumber mannaRate;
				GetDialogItem(dlog, kMannaRateBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				if ( ! ParseNumber(FromPascalString(text), mannaRate) || mannaRate < 0 ) {
					SelectDialogItemText(dlog, kMannaRateBox, 0, -1);
					SysBeep(1);
					break;
				}
			// seed limit
				long seedLimit;
				GetDialogItem(dlog, kSeedLimitBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				StringToNum(text, &seedLimit);
				if ( seedLimit < 0 || seedLimit > 100 ) {
					SelectDialogItemText(dlog, kSeedLimitBox, 0, -1);
					SysBeep(1);
					break;
				}
			// seed value
				GBNumber seedValue;
				GetDialogItem(dlog, kSeedValueBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				if ( ! ParseNumber(FromPascalString(text), seedValue) || seedValue <= 0 ) {
					SelectDialogItemText(dlog, kSeedValueBox, 0, -1);
					SysBeep(1);
					break;
				}
			// seed type penalty
				GBNumber seedTypePenalty;
				GetDialogItem(dlog, kSeedTypePenaltyBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				if ( ! ParseNumber(FromPascalString(text), seedTypePenalty) || seedTypePenalty > seedValue ) {
					SelectDialogItemText(dlog, kSeedTypePenaltyBox, 0, -1);
					SysBeep(1);
					break;
				}
			// time limit
				long timeLimit;
				GetDialogItem(dlog, kTimeLimitBox, &itemType, &item, &bounds);
				GetDialogItemText(item, text);
				StringToNum(text, &timeLimit);
				if ( timeLimit < 0 || timeLimit > 1000000 ) {
					SelectDialogItemText(dlog, kTimeLimitBox, 0, -1);
					SysBeep(1);
					break;
				}
			// store values
				world.mannaSize = mannaSize;
				world.mannaDensity = mannaDensity;
				world.mannaRate = mannaRate;
				world.seedLimit = seedLimit;
				world.seedValue = seedValue;
				world.seedTypePenalty = seedTypePenalty;
				world.autoReseed = autoReseedCheck;
				world.timeLimit = timeLimitCheck ? timeLimit : 0;
				world.stopOnElimination = stopOnEliminationCheck;
			// resize is last so manna changes can take effect
				if ( worldWidth != world.BackgroundTilesX()
					|| worldHeight != world.BackgroundTilesY() ) {
					world.Resize(GBFinePoint(kBackgroundTileSize * worldWidth,
						kBackgroundTileSize * worldHeight));
					world.running = false;
					minimapWindow->ZoomOut();
				}
				done = true;
			} break;
			case kCancelButton:
				done = true; break;
			case kAutoReseedCheckbox:
				autoReseedCheck = ! autoReseedCheck;
				GetDialogItem(dlog, kAutoReseedCheckbox, &itemType, &item, &bounds);
				SetControlValue((ControlHandle)item, autoReseedCheck);
				break;
			case kTimeLimitCheckbox:
				timeLimitCheck = ! timeLimitCheck;
				GetDialogItem(dlog, kTimeLimitCheckbox, &itemType, &item, &bounds);
				SetControlValue((ControlHandle)item, timeLimitCheck);
				break;
			case kStopOnEliminationCheckbox:
				stopOnEliminationCheck = ! stopOnEliminationCheck;
				GetDialogItem(dlog, kStopOnEliminationCheckbox, &itemType, &item, &bounds);
				SetControlValue((ControlHandle)item, stopOnEliminationCheck);
				break;
			default: break;
		}
	} while ( ! done );
	DisposeDialog(dlog);
}

GBApplication::GBApplication()
	: GBViewsApplication(),
	speedLimit(kNormalSpeedLimit), lastFrame(Milliseconds()),
	world(),
	lastAutofollow(Milliseconds()),
	mainWindow(nil), minimapWindow(nil),
	statusWindow(nil), rosterWindow(nil),
	scoresWindow(nil), typeWindow(nil),
	aboutWindow(nil), inspectorWindow(nil),
	tournamentWindow(nil), debuggerWindow(nil), sideDebuggerWindow(nil)
{
	SetupSound();
	SetupMenus();
// interface
	Point where;
	where.h = 537; where.v = 384;
	inspectorWindow = new GBMacWindow(new GBInspectorView(world), where, false);
	where.h = 320; where.v = 384;
	debugger = new GBDebuggerView(world);
	debuggerWindow = new GBMacWindow(debugger, where, false);
	where.h = 200; where.v = 400;
	sideDebuggerWindow = new GBMacWindow(new GBSideDebuggerView(world), where, false);
	portal = new GBPortal(world);
	where.h = 291; where.v = 43;
	mainWindow = new GBResizableMacWindow(new GBDoubleBufferedView(portal), where);
	where.h = 7; where.v = qd.screenBits.bounds.bottom - 230;
	minimap = new GBMiniMapView(world, *portal);
	minimapWindow = new GBResizableMacWindow(new GBDoubleBufferedView(minimap),
		where, true, false);
	where.h = 616; where.v = 43;
	statusWindow = new GBMacWindow(new GBStatusView(world), where, false);
	where.h = 7; where.v = 43;
	rosterWindow = new GBMacWindow(new GBRosterView(world), where);
	where.h = 200; where.v = 150;
	aboutWindow = new GBMacWindow(new GBAboutBox(), where, false);
	where.h = 291; where.v = 384;
	scoresWindow = new GBMacWindow(new GBScoresView(world), where, false);
	where.h = 616; where.v = 270;
	typeWindow = new GBMacWindow(new GBRobotTypeView(world), where, false);
	where.h = 100; where.v = 100;
	tournamentWindow = new GBMacWindow(new GBTournamentView(world), where, false);
}

GBApplication::~GBApplication() {
	delete typeWindow;
	delete scoresWindow;
	delete minimapWindow;
	delete mainWindow;
	delete rosterWindow;
	delete statusWindow;
	delete aboutWindow;
	delete inspectorWindow;
	delete tournamentWindow;
	delete debuggerWindow;
	delete sideDebuggerWindow;
	CleanupSound();
}

long GBApplication::SleepTime() {
	if ( world.running || dragging || portal->Following() ) {
		if ( speedLimit <= 0 ) return 0;
		GBMilliseconds delay = speedLimit + lastFrame - Milliseconds();
		return delay < 0 ? 0 : (long)((delay * 60 + 30) / 1000);
	}
	return -1;
}

void GBApplication::AdjustMenus() {
	MenuHandle thisMenu;
// check About item
	thisMenu = GetMenuHandle(kAppleMenu);
	GBMacWindow * wind = GBMacWindow::GetFromWindow(FrontWindow());
	CheckItem(thisMenu, miAbout, wind == aboutWindow);
// side operations in File menu
	thisMenu = GetMenuHandle(kFileMenu);
	if ( world.SelectedSide() ) {
		EnableItem(thisMenu, miDuplicateSide);
		EnableItem(thisMenu, miReloadSide);
		EnableItem(thisMenu, miRemoveSide);
	} else {
		DisableItem(thisMenu, miDuplicateSide);
		DisableItem(thisMenu, miReloadSide);
		DisableItem(thisMenu, miRemoveSide);
	}
	if ( world.Sides() ) {
		EnableItem(thisMenu, miRemoveAllSides);
	} else {
		DisableItem(thisMenu, miRemoveAllSides);
	}
// check windows in Window menu
	thisMenu = GetMenuHandle(kWindowMenu);
	CheckItem(thisMenu, miMainView, wind == mainWindow);
	CheckItem(thisMenu, miRosterView, wind == rosterWindow);
	CheckItem(thisMenu, miMinimapView, wind == minimapWindow);
	CheckItem(thisMenu, miStatusView, wind == statusWindow);
	CheckItem(thisMenu, miScoresView, wind == scoresWindow);
	CheckItem(thisMenu, miTypesView, wind == typeWindow);
	CheckItem(thisMenu, miInspectorView, wind == inspectorWindow);
	CheckItem(thisMenu, miTournamentView, wind == tournamentWindow);
	CheckItem(thisMenu, miDebuggerView, wind == debuggerWindow);
	CheckItem(thisMenu, miSideDebuggerView, wind == sideDebuggerWindow);
// check view options
	thisMenu = GetMenuHandle(kViewMenu);
	CheckItem(thisMenu, miSound, SoundActive());
	CheckItem(thisMenu, miShowSensors, portal->showSensors);
	CheckItem(thisMenu, miShowDecorations, portal->showDecorations);
	CheckItem(thisMenu, miShowMeters, portal->showDetails);
	CheckItem(thisMenu, miMinimapRobots, minimap->showRobots);
	CheckItem(thisMenu, miMinimapFood, minimap->showFood);
	CheckItem(thisMenu, miMinimapSensors, minimap->showSensors);
	CheckItem(thisMenu, miMinimapDecorations, minimap->showDecorations);
	CheckItem(thisMenu, miMinimapTrails, minimap->showTrails);
	CheckItem(thisMenu, miReportErrors, world.reportErrors);
	CheckItem(thisMenu, miReportPrints, world.reportPrints);
	CheckItem(thisMenu, miAutofollow, portal->autofollow);
// enable/disable run/pause in Simulation menu
	thisMenu = GetMenuHandle(kSimulationMenu);
	if ( world.running ) {
		DisableItem(thisMenu, miRun);
		DisableItem(thisMenu, miSingleFrame);
		DisableItem(thisMenu, miStep);
		EnableItem(thisMenu, miPause);
	} else {
		EnableItem(thisMenu, miRun);
		EnableItem(thisMenu, miSingleFrame);
		if ( debugger->Active() ) EnableItem(thisMenu, miStep);
		else DisableItem(thisMenu, miStep);
		DisableItem(thisMenu, miPause);
	}
// check speed limit
	CheckItem(thisMenu, miSlowSpeed, speedLimit == kSlowSpeedLimit);
	CheckItem(thisMenu, miNormalSpeed, speedLimit == kNormalSpeedLimit);
	CheckItem(thisMenu, miFastSpeed, speedLimit == kFastSpeedLimit);
	CheckItem(thisMenu, miUnlimitedSpeed, speedLimit == kNoSpeedLimit);
// check simulation options
	CheckItem(thisMenu, miTournament, world.tournament);
// enable/disable start/stop brain
	if ( debugger->Active() ) {
		EnableItem(thisMenu, miStartStopBrain);
	} else {
		DisableItem(thisMenu, miStartStopBrain);
	}
// check items in Tools menu
	thisMenu = GetMenuHandle(kToolsMenu);
	GBSide * side = world.SelectedSide();
	if ( side ) {
		EnableItem(thisMenu, miAddSeed);
		EnableItem(thisMenu, miAddRobot);
	} else {
		DisableItem(thisMenu, miAddSeed);
		DisableItem(thisMenu, miAddRobot);
		if ( portal->tool == ptAddSeed || portal->tool == ptAddRobot )
			portal->tool = ptScroll;
	}
	CheckItem(thisMenu, miScroll, portal->tool == ptScroll);
	CheckItem(thisMenu, miAddManna, portal->tool == ptAddManna);
	CheckItem(thisMenu, miAddRobot, portal->tool == ptAddRobot);
	CheckItem(thisMenu, miAddSeed, portal->tool == ptAddSeed);
	CheckItem(thisMenu, miMove, portal->tool == ptMove);
	CheckItem(thisMenu, miPull, portal->tool == ptPull);
	CheckItem(thisMenu, miSmite, portal->tool == ptSmite);
	CheckItem(thisMenu, miBlasts, portal->tool == ptBlasts);
	CheckItem(thisMenu, miErase, portal->tool == ptErase);
	CheckItem(thisMenu, miEraseBig, portal->tool == ptEraseBig);
}

void GBApplication::HandleMenuSelection(short menu, short item) {
	try {
		switch ( menu ) {
			case kAppleMenu:
				switch ( item ) {
					case miAbout: aboutWindow->Select(); break;
					default: OpenAppleMenuItem(menu, item); break;
				} break;
			case kFileMenu:
				switch ( item ) {
					case miLoadSide: DoLoadSide(); break;
					case miDuplicateSide:
						if ( world.SelectedSide() )
							world.AddSide(world.SelectedSide()->Copy());
						break;
					case miReloadSide: DoReloadSide(); break;
					case miRemoveSide:
						if ( world.SelectedSide() ) {
							if ( world.SelectedSide()->Scores().Seeded() ) {
								world.Reset();
								world.running = false;
							}
							world.RemoveSide(world.SelectedSide());
						} break;
					case miRemoveAllSides:
						world.Reset();
						world.RemoveAllSides();
						world.running = false;
						break;
					case miClose: {
							WindowPtr wind = FrontWindow();
							if ( GBMacWindow::IsMacWindow(wind) )
								GBMacWindow::GetFromWindow(wind)->Close();
						} break;
					case miQuit:
						if ( ! world.tournament
								|| Confirm("Do you really want to quit during a tournament?", "Quit") )
							Quit();
						break;
					default: break;
				} break;
			case kWindowMenu:
				switch ( item ) {
					case miMainView: mainWindow->Select(); break;
					case miRosterView: rosterWindow->Select(); break;
					case miMinimapView: minimapWindow->Select(); break;
					case miStatusView: statusWindow->Select(); break;
					case miScoresView: scoresWindow->Select(); break;
					case miTypesView: typeWindow->Select(); break;
					case miInspectorView: inspectorWindow->Select(); break;
					case miTournamentView: tournamentWindow->Select(); break;
					case miDebuggerView: debuggerWindow->Select(); break;
					case miSideDebuggerView: sideDebuggerWindow->Select(); break;
					default: break;
				} break;
			case kViewMenu:
				switch ( item ) {
					case miSound: SetSoundActive(! SoundActive()); break;
					case miShowSensors: portal->showSensors = ! portal->showSensors; break;
					case miShowDecorations: portal->showDecorations = ! portal->showDecorations; break;
					case miShowMeters: portal->showDetails = ! portal->showDetails; break;
					case miMinimapRobots: minimap->showRobots = ! minimap->showRobots; break;
					case miMinimapFood: minimap->showFood = ! minimap->showFood; break;
					case miMinimapSensors: minimap->showSensors = ! minimap->showSensors; break;
					case miMinimapDecorations: minimap->showDecorations = ! minimap->showDecorations; break;
					case miMinimapTrails: minimap->showTrails = ! minimap->showTrails; break;
					case miReportErrors: world.reportErrors = ! world.reportErrors; break;
					case miReportPrints: world.reportPrints = ! world.reportPrints; break;
					case miRefollow: portal->Refollow(); break;
					case miFollowRandom: portal->FollowRandom(); break;
					case miRandomNear: portal->FollowRandomNear(); break;
					case miAutofollow: portal->autofollow = ! portal->autofollow; break;
					default: break;
				} break;
			case kSimulationMenu:
				switch ( item ) {
					case miRun:
						world.running = true;
						lastFrame = Milliseconds();
						break;
					case miSingleFrame:
						world.SimulateOneFrame();
						world.running = false;
						break;
					case miStep:
						if ( debugger->Active() && debugger->Step() )
							world.SimulateOneFrame();
						world.running = false;
						break;
					case miPause: world.running = false; break;
					case miSlowSpeed: speedLimit = kSlowSpeedLimit; break;
					case miNormalSpeed: speedLimit = kNormalSpeedLimit; break;
					case miFastSpeed: speedLimit = kFastSpeedLimit; break;
					case miUnlimitedSpeed: speedLimit = kNoSpeedLimit; break;
					case miNewRound:
						world.Reset();
						world.AddSeeds();
						world.running = true;
						break;
					case miRestart:
						world.Reset();
						world.running = false;
						break;
					case miSeed: world.AddSeeds(); break;
					case miReseed: world.ReseedDeadSides(); break;
					case miRules: DoRulesDialog(); break;
					case miTournament:
						if ( world.tournament ) world.tournament = false;
						else if ( DoNumberDialog("\pNumber of rounds:", world.tournamentLength, -1) )
							world.tournament = true;
						break;
					case miResetScores: world.ResetTournamentScores(); break;
					case miStartStopBrain: debugger->StartStopBrain(); break;
					default: break;
				} break;
			case kToolsMenu:
				switch ( item ) {
					case miScroll: portal->tool = ptScroll; break;
					case miAddManna: portal->tool = ptAddManna; break;
					case miAddRobot: portal->tool = ptAddRobot; break;
					case miAddSeed: portal->tool = ptAddSeed; break;
					case miMove: portal->tool = ptMove; break;
					case miPull: portal->tool = ptPull; break;
					case miSmite: portal->tool = ptSmite; break;
					case miBlasts: portal->tool = ptBlasts; break;
					case miErase: portal->tool = ptErase; break;
					case miEraseBig: portal->tool = ptEraseBig; break;
					default: break;
				} break;
			default: break;
		}
	} catch ( GBAbort & ) {}
	HiliteMenu(0);
}

void GBApplication::Process() {
	try {
		if ( world.running ) {
			lastFrame = Milliseconds();
			do {
				world.SimulateOneFrame();
			} while ( ! speedLimit && world.running && Milliseconds() <= lastFrame + kMaxEventInterval );
		}
	} catch ( GBError & err ) {
		NonfatalError("Error simulating: " + err.ToString());
	} catch ( GBAbort & ) {
		world.running = false;
	}
	if ( portal->autofollow && Milliseconds() >= lastAutofollow + kAutofollowInterval ) {
		portal->FollowRandom();
		lastAutofollow = Milliseconds();
	}
}

void GBApplication::Redraw() {
	try {
		mainWindow->DrawChanges(world.running);
		minimapWindow->DrawChanges(world.running);
		statusWindow->DrawChanges(world.running || dragging);
		rosterWindow->DrawChanges(world.running || dragging);
		scoresWindow->DrawChanges(world.running || dragging);
		typeWindow->DrawChanges(world.running || dragging);
		inspectorWindow->DrawChanges(world.running || dragging);
		tournamentWindow->DrawChanges(world.running || dragging);
		debuggerWindow->DrawChanges(world.running || dragging);
		sideDebuggerWindow->DrawChanges(world.running || dragging);
	} catch ( GBError & err ) {
		NonfatalError("Error drawing: " + err.ToString());
	}
}

void GBApplication::OpenFile(FSSpec & file) {
	SetCursor(cuWait);
	GBSide * side = GBSideReader::Load(file);
	if ( side ) world.AddSide(side);
}

