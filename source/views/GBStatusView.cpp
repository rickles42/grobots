// GBStatusView.cpp
// miscellaneous info on a world
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBStatusView.h"
#include "GBErrors.h"
#include "GBColor.h"
#include "GBWorld.h"


const short kBoxEdgeSpace = 2;
const short kFrameBoxHeight = GBWORLD_PROFILING ? 93 : 28;
const short kObjectCountBoxHeight = 58;
const short kValueBoxHeight = 38;

GBStatusView::GBStatusView(GBWorld & targ)
	: GBView(),
	world(targ),
	worldChanges(-1),
	lastFrame(0), lastTime(-1)
{}

void GBStatusView::Draw() {
	DrawBackground();
// frame box
	GBRect box;
	box.left = kBoxEdgeSpace;
	box.top = kBoxEdgeSpace;
	box.right = Width() - kBoxEdgeSpace;
	box.bottom = box.top + kFrameBoxHeight;
	DrawBox(box);
// framenumber
	DrawStringLongPair("Frame:", world.CurrentFrame(), 7, Width() - 7, box.top + 13, 10);
// framerate
	if ( lastTime >= 0 && world.CurrentFrame() > lastFrame ) {
		DrawStringLongPair("Framerate:", 
			((world.CurrentFrame() - lastFrame) * 1000 + 500) / (long)(Milliseconds() - lastTime),
			7, Width() - 7, box.top + 23, 10);
	}
	lastFrame = world.CurrentFrame();
	lastTime = Milliseconds();
#if GBWORLD_PROFILING
	DrawStringLongPair("Total time:", world.TotalTime(), 7, Width() - 7, box.top + 38, 10);
	DrawStringLongPair("Simulation:", world.SimulationTime(), 7, Width() - 7, box.top + 48, 10);
	DrawStringLongPair("Move:", world.MoveTime(), 7, Width() - 7, box.top + 58, 10);
	DrawStringLongPair("Act:", world.ActTime(), 7, Width() - 7, box.top + 68, 10);
	DrawStringLongPair("Collide:", world.CollideTime(), 7, Width() - 7, box.top + 78, 10);
	DrawStringLongPair("Think:", world.ThinkTime(), 7, Width() - 7, box.top + 88, 10);
	world.ResetTimes();
#endif
// object counts
	box.top = box.bottom + kBoxEdgeSpace;
	box.bottom = box.top + kObjectCountBoxHeight;
	DrawBox(box);
	DrawStringLongPair("Robots:", world.CountObjects(ocRobot), 7, Width() - 7, box.top + 13, 10);
	DrawStringLongPair("Foods:", world.CountObjects(ocFood), 7, Width() - 7, box.top + 23, 10);
	DrawStringLongPair("Shots:", world.CountObjects(ocShot) + world.CountObjects(ocArea), 7, Width() - 7, box.top + 33, 10);
	DrawStringLongPair("Sensors:", world.CountObjects(ocSensorShot), 7, Width() - 7, box.top + 43, 10);
	DrawStringLongPair("Decorations:", world.CountObjects(ocDecoration), 7, Width() - 7, box.top + 53, 10);
// values
	box.top = box.bottom + kBoxEdgeSpace;
	box.bottom = box.top + kValueBoxHeight;
	DrawBox(box);
	DrawStringLongPair("Manna:", world.MannaValue(), 7, Width() - 7, box.top + 13, 10, GBColor::darkGreen);
	DrawStringLongPair("Corpses:", world.CorpseValue(), 7, Width() - 7, box.top + 23, 10, GBColor::red);
	DrawStringLongPair("Robots:", world.RobotValue(), 7, Width() - 7, box.top + 33, 10, GBColor::blue);
//
	worldChanges = world.ChangeCount();
}

GBMilliseconds GBStatusView::RedrawInterval() const {
	return 500;
}

bool GBStatusView::DelayedChanges() const {
	return worldChanges != world.ChangeCount();
}

short GBStatusView::PreferredWidth() const {
	return 110;
}

short GBStatusView::PreferredHeight() const {
	return kFrameBoxHeight + kObjectCountBoxHeight + kValueBoxHeight + kBoxEdgeSpace * 4;
}

const string GBStatusView::Name() const {
	return "Status";
}


