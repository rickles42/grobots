// GBScoresView.h
// scores and statistics
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBScoresView_h
#define GBScoresView_h

#include "GBListView.h"
#include "GBWorld.h"


class GBScoresView : public GBListView {
	GBWorld & world;
	GBChangeCount lastDrawnWorld;
	long sideID;
	
	void DrawIncome(const GBIncomeStatistics & income, short left, short right, short top);
	void DrawExpenditures(const GBExpenditureStatistics & spent, short left, short right, short top);
	void DrawDeaths(const GBScores & scores, short left, short right, short top);
public:
	explicit GBScoresView(GBWorld & rost);

	void Draw();
	GBMilliseconds RedrawInterval() const;
	bool InstantChanges() const;
	bool DelayedChanges() const;
	
	short PreferredWidth() const;
	
	const string Name() const;

	long Items() const;
	short HeaderHeight() const;
	short ItemHeight() const;
	void DrawHeader(const GBRect & box);
	void DrawItem(long index, const GBRect & box);
	void ItemSelected(long index);
};

#endif
