// GBStatusView.h
// Displays the status of a World
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBStatusView_h
#define GBStatusView_h

#include "GBView.h"
#include "GBWorld.h"
#include "GBMilliseconds.h"


class GBStatusView : public GBView {
	GBWorld & world;
	GBChangeCount worldChanges;
// framecounter
	GBFrames lastFrame;
	GBMilliseconds lastTime;
public:
	explicit GBStatusView(GBWorld & targ);

	void Draw();

	GBMilliseconds RedrawInterval() const;
	bool DelayedChanges() const;
	
	short PreferredWidth() const;
	short PreferredHeight() const;
	
	const string Name() const;
};

#endif
