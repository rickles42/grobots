// GBInspectorView.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBInspectorView.h"
#include "GBObject.h"
#include "GBFood.h"
#include "GBShot.h"
#include "GBSensorShot.h"
#include "GBRobot.h"
#include "GBRobotType.h"
#include "GBWorld.h"
#include "GBSide.h"
#include "GBStringUtilities.h"


const short divOne = 120;
const short divTwo = 200;
const short width = 350;
const short half = width / 2;
const short lineOne = 20;
const short lineTwo = 40;
const short lineThree = 60;
const short lineFour = 80;


GBInspectorView::GBInspectorView(const GBWorld & newWorld)
	: GBView(), world(newWorld), worldChangeCount(-1), redrawAnyway(true),
	viewee(nil)
{}

GBInspectorView::~GBInspectorView() {
	if ( viewee ) viewee->RemoveDeletionListener(this);
}

void GBInspectorView::Draw() {
	if ( viewee != world.Followed() ) {
		if ( viewee ) viewee->RemoveDeletionListener(this);
		viewee = world.Followed();
		if ( viewee ) viewee->AddDeletionListener(this);
	}
	if ( ! viewee ) {
		DrawBackground();
		DrawStringLeft("Nothing selected", 4, 20, 12);
	} else {
		DrawBackground(GBColor::white);
		string className = "Unknown object class";
		GBObjectClass oc = viewee->Class();
		switch ( oc ) {
		case ocRobot: {
			className = "Robot";
			GBRobot * bot = dynamic_cast<GBRobot *>(viewee);
			if ( ! bot ) break;
			DrawStringLongPair("ID:", bot->ID(), divOne + 2, divTwo - 2, lineTwo, 12);
			DrawStringPair("Type:", bot->Type()->Name(), divTwo + 2, width - 5, lineTwo,
				12, bot->Type()->Color().ContrastingTextColor());
			DrawStringPair("Energy:", ToString(bot->Energy(), 2), 5, divOne - 2, lineTwo, 12, GBColor::darkGreen);
			DrawStringPair("Armor:", ToString(bot->hardware.Armor(), 2), 5, divOne - 2, lineFour, 12, GBColor::darkGray);
			if ( bot->hardware.MaxShield().Nonzero() )
				DrawStringPair("Shield:", ToString(bot->hardware.ActualShield(), 2) + " (" + ToPercentString(bot->ShieldFraction(), 1) + ')',
					divOne + 2, width - 5, lineFour, 12, GBColor::blue);
			} break;
		case ocFood:
			className = "Food";
			DrawStringPair("Value:", ToString(viewee->Energy(), 1),
				2, divOne - 2, lineTwo, 12, GBColor::darkGreen);
			{
				GBCorpse * corpse = dynamic_cast<GBCorpse *>(viewee);
				if ( corpse )
					DrawStringPair("Killed by:", corpse->Killer() ? corpse->Killer()->Name() : string("accident"),
						divOne + 2, width - 5, lineTwo, 12,
						corpse->Killer() ? corpse->Killer()->Color().ContrastingTextColor() : GBColor::black);
			} break;
		case ocShot:
			className = "Shot";
			DrawStringPair("Power:", ToString(((const GBShot *)viewee)->Power(), 1), 2, divOne - 2, lineTwo, 12);
			break;
		case ocArea:
			className = "Area Shot";
			DrawStringPair("Power:", ToString(((const GBShot *)viewee)->Power(), 1), 2, divOne - 2, lineTwo, 12);
			break;
		case ocSensorShot: {
			className = "Sensor";
				string tempStr;
				switch ( ((GBSensorShot *)viewee)->Seen() ) {
					case ocFood: tempStr = "Food"; break;
					case ocRobot: tempStr = "Robots"; break;
					case ocShot: tempStr = "Shots"; break;
					default: tempStr = "unknown"; break;
				}
				DrawStringPair("Sees:", tempStr, 2, divOne - 2, lineTwo, 12);
			DrawStringLongPair("Firer ID:", ((GBSensorShot *)viewee)->Firer()->ID(), divTwo + 2, width - 5, lineTwo, 12);
			} break;
		case ocDecoration:
			className = "Decoration";
			break;
		case ocDead:
			className = "Dead";
			break;
		default:
			break;
		}
	// common stuff
		DrawStringPair("Class:", className, 5, divOne - 2, lineOne, 12);
		DrawStringPair("Mass:", ToString(viewee->Mass(), 2), divOne + 2, divTwo - 2, lineOne, 12);
		DrawStringPair("Side:", viewee->Owner() ? viewee->Owner()->Name() : string("none"), divTwo + 2, width - 5, lineOne,
			12, viewee->Owner() ? viewee->Owner()->Color().ContrastingTextColor() : GBColor::black);
		DrawStringPair("Position:", ToString(viewee->Position(), 2), 5, half - 2, lineThree, 12);
		DrawStringPair("Velocity:", ToString(viewee->Velocity()), half + 2, width - 5, lineThree, 12);
	}
// record
	worldChangeCount = world.ChangeCount();
	redrawAnyway = false;
}

void GBInspectorView::ReportDeletion(const GBDeletionReporter * deletee) {
	if ( deletee == (const GBDeletionReporter *)viewee ) {
		viewee = nil;
		redrawAnyway = true;
	} else
		throw GBGenericError("Deletion reported on object GBInspectorView isn't viewing.");
}

GBMilliseconds GBInspectorView::RedrawInterval() const {
	return 250;
}

bool GBInspectorView::InstantChanges() const {
	return redrawAnyway || viewee != world.Followed();
}

bool GBInspectorView::DelayedChanges() const {
	return viewee && world.ChangeCount() != worldChangeCount;
}

short GBInspectorView::PreferredWidth() const {
	return width;
}

short GBInspectorView::PreferredHeight() const {
	return 100;
}

const string GBInspectorView::Name() const {
	return "Inspector";
}

