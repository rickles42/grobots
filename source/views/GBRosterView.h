// GBRosterView.h
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBRosterView_h
#define GBRosterView_h

#include "GBListView.h"
#include "GBModel.h"

class GBWorld;

class GBRosterView : public GBListView {
	GBWorld & world;
	GBChangeCount worldChanges;
	long sideID;
public:
	explicit GBRosterView(GBWorld & wrld);

	void Draw();
	GBMilliseconds RedrawInterval() const;
	bool InstantChanges() const;
	bool DelayedChanges() const;
	
	short PreferredWidth() const;
	
	const string Name() const;

	long Items() const;
	short ItemHeight() const;
	void DrawItem(long index, const GBRect & box);
	void ItemClicked(long index);
};

#endif
