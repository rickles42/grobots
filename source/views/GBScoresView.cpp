// GBSideView.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBScoresView.h"
#include "GBSide.h"
#include "GBWorld.h"
#include "GBRobotType.h"
#include "GBStringUtilities.h"

const short kInfoBoxHeight = 184;
const short kTypeStatsWidth = 100;

void GBScoresView::DrawIncome(const GBIncomeStatistics & income,
		short left, short right, short top) {
	long total = income.Total();
	if ( ! total ) return;
	DrawStringPair("Solar:", ToPercentString(income.Autotrophy(), total),
		left, right, top + 10, 9, GBColor::darkGreen);
	DrawStringPair("Manna:", ToPercentString(income.Theotrophy(), total),
		left, right, top + 20, 9, GBColor::darkGreen);
	DrawStringPair("Enemies:", ToPercentString(income.Heterotrophy(), total),
		left, right, top + 30, 9, GBColor::purple);
	DrawStringPair("Stolen:", ToPercentString(income.Kleptotrophy(), total),
		left, right, top + 40, 9, GBColor(0.4, 0.6, 0));
	DrawStringPair("Cannibal:", ToPercentString(income.Cannibalism(), total),
		left, right, top + 50, 9, GBColor::darkRed);
}

void GBScoresView::DrawExpenditures(const GBExpenditureStatistics & spent,
		short left, short right, short top) {
	long total = spent.Total();
	if ( ! total ) return;
	DrawStringPair("Growth:", ToPercentString(spent.Construction(), total),
		left, right, top + 10, 9, GBColor::darkGreen);
	DrawStringPair("Engine:", ToPercentString(spent.Engine(), total),
		left, right, top + 20, 9, GBColor::black);
	DrawStringPair("Sensors:", ToPercentString(spent.Sensors(), total),
		left, right, top + 30, 9, GBColor::blue);
	DrawStringPair("Weapons:", ToPercentString(spent.Weapons(), total),
		left, right, top + 40, 9, GBColor::purple);
	DrawStringPair("Force:", ToPercentString(spent.ForceField(), total),
		left, right, top + 50, 9, GBColor::blue);
	DrawStringPair("Shield:", ToPercentString(spent.Shield(), total),
		left, right, top + 60, 9, GBColor::blue);
	DrawStringPair("Repairs:", ToPercentString(spent.Repairs(), total),
		left, right, top + 70, 9, GBColor::black);
	DrawStringPair("Misc:", ToPercentString(spent.Misc(), total),
		left, right, top + 80, 9, GBColor::black);
	DrawStringPair("Stolen:", ToPercentString(spent.Stolen(), total),
		left, right, top + 90, 9, GBColor(0.4, 0.6, 0));
	DrawStringPair("Overflow:", ToPercentString(spent.Wasted(), total),
		left, right, top + 100, 9, GBColor::darkRed);
}

void GBScoresView::DrawDeaths(const GBScores & scores,
		short left, short right, short top) {
	DrawStringPair("Kills:", ToPercentString(scores.KilledFraction()),
		left, right, top + 10, 9, GBColor::purple);
	if ( scores.Survived() )
		DrawStringPair("Relative:", ToPercentString(scores.KillRate(), 0),
			left, right, top + 20, 9, GBColor::black);
	DrawStringLongPair("Kills:", scores.Killed(), left, right, top + 30, 9, GBColor::purple);
	DrawStringLongPair("Dead:", scores.Dead(), left, right, top + 40, 9, GBColor::black);
	DrawStringLongPair("Suicide:", scores.Suicide(), left, right, top + 50, 9, GBColor::darkRed);
}

void GBScoresView::DrawHeader(const GBRect & box) {
	const GBSide * side = world.SelectedSide();
	DrawBox(box);
	DrawStringLeft(side ? side->Name() : std::string("Overall statistics"),
		box.left + 3, box.top + 13, 12, GBColor::black);
// draw stats...
	short c1 = box.left + 3;
	short c2 = (box.left * 3 + box.right) / 4 + 2;
	short c3 = box.CenterX() + 2;
	short c4 = (box.left + box.right * 3) / 4 + 2;
	const GBScores & scores = side ? side->Scores() : world.RoundScores();
// basics
	DrawStringLongPair("Biomass:", scores.Biomass(), c1, c2 - 4, box.top + 25, 9, GBColor::darkGreen);
	DrawStringLongPair("Population:", scores.Population(), c1, c2 - 4, box.top + 35, 9, GBColor::blue);
	DrawStringLongPair("Ever:", scores.PopulationEver(), c1 + 10, c2 - 4, box.top + 45, 9, GBColor::blue);
	if ( side ) {
		if ( side->Scores().Sterile() && side->Scores().SterileTime() != side->Scores().ExtinctTime() )
			DrawStringLongPair("Sterile:", side->Scores().SterileTime(),
				c1, c2 - 4, box.top + 60, 9, GBColor::purple);
		if ( ! side->Scores().Population() )
			DrawStringLongPair("Extinct:", side->Scores().ExtinctTime(),
				c1, c2 - 4, box.top + 70, 9, GBColor::red);
	}
// income
	DrawStringLongPair("Income:", scores.Income().Total(), c1, c2 - 4, box.top + 95, 9, GBColor::black);
	DrawIncome(scores.Income(), c1, c2 - 4, box.top + 95);
	DrawStringLongPair("Seed:", scores.Seeded(), c1, c2 - 4, box.top + 155, 9, GBColor::black);
	if ( scores.Efficiency() > 0 )
		DrawStringPair("Efficiency:", ToPercentString(scores.Efficiency(), 0),
			c1, c2 - 4, box.top + 170, 9, GBColor::black);
	if ( scores.Doubletime(world.CurrentFrame()) )
		DrawStringLongPair("Double:", scores.Doubletime(world.CurrentFrame()),
			c1, c2 - 4, box.top + 180, 9, GBColor::black);
// expenditures
	DrawStringLongPair("Spent:", scores.Expenditure().Total(), c2, c3 - 4, box.top + 25, 9, GBColor::black);
	DrawExpenditures(scores.Expenditure(), c2, c3 - 4, box.top + 25);
// death
	DrawDeaths(scores, c2, c3 - 4, box.top + 130);
// tournament stats:
	const GBScores & tscores = side ? side->TournamentScores() : world.TournamentScores();
	DrawStringLongPair("Rounds:", tscores.Rounds(), c3, c4 - 4, box.top + 10, 9, GBColor::black);
	DrawStringPair("Biomass:", ToPercentString(tscores.BiomassFraction()),
		c3, c4 - 4, box.top + 25, 9, GBColor::darkGreen);
	DrawStringPair("Early:", ToPercentString(tscores.EarlyBiomassFraction()),
		c3 + 10, c4 - 4, box.top + 35, 9, GBColor::darkGreen);
	DrawStringPair("Survival:", ToPercentString(tscores.Survival()),
		c3, c4 - 4, box.top + 50, 9, GBColor::black);
	DrawStringPair("Early d:", ToPercentString(tscores.EarlyDeathRate()),
		c3, c4 - 4, box.top + 60, 9, GBColor::black);
	DrawStringPair("Late d:", ToPercentString(tscores.LateDeathRate()),
		c3, c4 - 4, box.top + 70, 9, GBColor::black);
// income
	DrawStringLongPair("Avg income:", tscores.Income().Total() / tscores.Rounds(),
		c3, c4 - 4, box.top + 95, 9, GBColor::darkGreen);
	DrawIncome(tscores.Income(), c3, c4 - 4, box.top + 95);
	DrawStringLongPair("Avg seed:", tscores.Seeded(),
		c3, c4 - 4, box.top + 155, 9, GBColor::black);
	if ( tscores.Efficiency() > 0 )
		DrawStringPair("Efficiency:", ToPercentString(tscores.Efficiency(), 0),
			c3, c4 - 4, box.top + 170, 9, GBColor::black);
// expenditures
	DrawStringLongPair("Avg spent:", tscores.Expenditure().Total() / tscores.Rounds(),
		c4, box.right - 4, box.top + 25, 9, GBColor::black);
	DrawExpenditures(tscores.Expenditure(), c4, box.right - 4, box.top + 25);
// death
	DrawDeaths(tscores, c4, box.right - 4, box.top + 130);
}

void GBScoresView::DrawItem(long index, const GBRect & box) {
	const GBSide * side = world.SelectedSide();
	if ( ! side ) return;
	const GBRobotType * type = side->GetType(index);
	if ( ! type ) return;
	DrawBox(box);
// draw ID and name and color
	DrawStringLeft(ToString(type->ID()) + '.', box.left + 3, box.top + 13, 12, type->Color());
	DrawStringLeft(type->Name(), box.left + 20, box.top + 13,
		12, GBColor::black);
// numbers
	DrawStringLongPair("Population:", type->Population(),
		box.right - kTypeStatsWidth * 2 + 5, box.right - kTypeStatsWidth, box.top + 13,
		10, GBColor::blue);
	DrawStringLongPair("Biomass:", type->Biomass(),
		box.right - kTypeStatsWidth + 5, box.right - 2, box.top + 13,
		10, GBColor::darkGreen);
}

GBScoresView::GBScoresView(GBWorld & w)
	: GBListView(),
	world(w),
	lastDrawnWorld(-1), sideID(-1)
{}

void GBScoresView::Draw() {
	GBListView::Draw();
// record
	lastDrawnWorld = world.ChangeCount();
	sideID = world.SelectedSideID();
}

GBMilliseconds GBScoresView::RedrawInterval() const {
	return 2000;
}

bool GBScoresView::InstantChanges() const {
	return sideID != world.SelectedSideID();
}

bool GBScoresView::DelayedChanges() const {
	return lastDrawnWorld != world.ChangeCount();
}

short GBScoresView::PreferredWidth() const {
	return 311;
}

const string GBScoresView::Name() const {
	return "Scores";
}

long GBScoresView::Items() const {
	const GBSide * side = world.SelectedSide();
	if ( ! side ) return 0;
	return side->CountTypes();
}

short GBScoresView::HeaderHeight() const {
	return kInfoBoxHeight;
}

short GBScoresView::ItemHeight() const {
	return 17;
}

