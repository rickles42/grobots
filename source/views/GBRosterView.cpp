// GBRosterView.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBRosterView.h"
#include "GBSide.h"
#include "GBWorld.h"
#include "GBStringUtilities.h"

const short kSideBoxHeight = 17;
const short kPopulationWidth = 30;


GBRosterView::GBRosterView(GBWorld & wrld)
	: GBListView(),
	world(wrld),
	worldChanges(-1), sideID(-1)
{}

void GBRosterView::Draw() {
	GBListView::Draw();
// record
	worldChanges = world.ChangeCount();
	sideID = world.SelectedSideID();
}

GBMilliseconds GBRosterView::RedrawInterval() const {
	return 1500;
}

bool GBRosterView::InstantChanges() const {
	return sideID != world.SelectedSideID(); // FIXME: when sides added or deleted or reloaded
}

bool GBRosterView::DelayedChanges() const {
	return worldChanges != world.ChangeCount();
}

short GBRosterView::PreferredWidth() const {
	return 270;
}

const string GBRosterView::Name() const {
	return "Roster";
}

long GBRosterView::Items() const {
	return world.CountSides();
}

short GBRosterView::ItemHeight() const {
	return kSideBoxHeight;
}

void GBRosterView::DrawItem(long index, const GBRect & box) {
	const GBSide * side = world.GetSide(index);
	bool selected = side == world.SelectedSide();
	DrawBox(box, selected);
	if ( ! side ) return;
// draw ID and name
	DrawStringRight(ToString(side->ID()) + '.', box.left + 25, box.top + 13, 12, side->Color());
	DrawStringLeft(side->Name(), box.left + 30, box.top + 13, 12, selected ? GBColor::white : GBColor::black);
// draw numbers
	if ( side->Scores().Seeded() ) {
		if ( side->Scores().Population() ) {
			if ( side->Scores().Sterile() )
				DrawStringRight(string("Sterile at ") + ToString(side->Scores().SterileTime()),
					box.right - 4, box.top + 12, 10, GBColor::purple);
			else {
				DrawStringRight(ToPercentString(side->Scores().BiomassFraction()), box.right - kPopulationWidth, box.top + 13,
					12, (selected ? GBColor::white : GBColor::black));
				DrawStringRight(ToString(side->Scores().Population()),
					box.right - 4, box.top + 12, 10, GBColor::blue);
			}
		} else {
			DrawStringRight(string("Extinct at ") + ToString(side->Scores().ExtinctTime()),
				box.right - 4, box.top + 12, 10, GBColor::red);
		}
	}
}

void GBRosterView::ItemClicked(long index) {
	world.SelectSide(index ? world.GetSide(index) : nil);
}

