// GBInspectorView.h
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBInspectorView_h
#define GBInspectorView_h

#include "GBView.h"
#include "GBModel.h"
#include "GBDeletionReporter.h"

class GBObject;
class GBWorld;

class GBInspectorView : public GBView, public GBDeletionListener {
	const GBObject * viewee;
	const GBWorld & world;
	GBChangeCount worldChangeCount;
	bool redrawAnyway;
public:
	explicit GBInspectorView(const GBWorld & newWorld);
	~GBInspectorView();
// drawing
	void Draw();
	GBMilliseconds RedrawInterval() const;
	bool InstantChanges() const;
	bool DelayedChanges() const;

	short PreferredWidth() const;
	short PreferredHeight() const;
	
	void ReportDeletion(const GBDeletionReporter * deletee);
	const string Name() const;
};

#endif
