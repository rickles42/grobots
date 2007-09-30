// GBMiniMap.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBMiniMap.h"

const float kTrailIntensity = 0.4f;

GBMiniMapView::GBMiniMapView(const GBWorld & targ, GBPortal & port)
	: GBView(),
	world(targ), portal(port),
	worldChanges(-1), portalChanges(-1), frameLastDrawn(0),
	trails(nil),
	showRobots(true), showFood(true), showSensors(false), showDecorations(false),
	showTrails(false)
{}

GBMiniMapView::~GBMiniMapView() {
	delete trails;
}

void GBMiniMapView::DrawLayer(const GBObjectClass layer, const short minSize) const {
	for ( int ty = world.ForegroundTilesY() - 1; ty >= 0; ty -- )
		for ( int tx = world.ForegroundTilesX() - 1; tx >= 0; tx -- )
			DrawObjectList(world.GetObjects(tx, ty, layer), minSize);
	DrawObjectList(world.GetLargeObjects(layer), minSize);
}

void GBMiniMapView::DrawLayerFixed(const GBObjectClass layer, const short size) const {
	for ( int ty = world.ForegroundTilesY() - 1; ty >= 0; ty -- )
		for ( int tx = world.ForegroundTilesX() - 1; tx >= 0; tx -- )
			DrawObjectListFixed(world.GetObjects(tx, ty, layer), size);
	DrawObjectListFixed(world.GetLargeObjects(layer), size);
}

void GBMiniMapView::DrawObjectList(const GBObject * list, const short minSize) const {
	for ( const GBObject * cur = list; cur != nil; cur = cur->next ) {
		GBRect where;
		where.left = ToScreenX(cur->Left());
		where.top = ToScreenY(cur->Top());
		where.right = where.left + (cur->Radius() * 2 * scalex).Max(minSize).Round();
		where.bottom = where.top + (cur->Radius() * 2 * scaley).Max(minSize).Round();
		cur->DrawMini(Graphics(), CalcExternalRect(where));
	}
}

void GBMiniMapView::DrawObjectListFixed(const GBObject * list, const short size) const {
	for ( const GBObject * cur = list; cur != nil; cur = cur->next ) {
		GBRect where;
		where.left = ToScreenX(cur->Left());
		where.top = ToScreenY(cur->Top());
		where.right = where.left + size;
		where.bottom = where.top + size;
		cur->DrawMini(Graphics(), CalcExternalRect(where));
	}
}

void GBMiniMapView::DrawObjectListTrails(const GBObject * list, const short minSize) const {
	trails->StartDrawing();
	for ( const GBObject * cur = list; cur != nil; cur = cur->next ) {
		GBRect where;
		where.left = ToScreenX(cur->Left());
		where.top = ToScreenY(cur->Top());
		where.right = where.left + (cur->Radius() * 2 * scalex).Max(minSize).Round();
		where.bottom = where.top + (cur->Radius() * 2 * scaley).Max(minSize).Round();
		trails->Graphics().DrawSolidRect(CalcExternalRect(where),
			cur->Color() * kTrailIntensity);
	}
	trails->StopDrawing();
}

void GBMiniMapView::RecalculateScales() {
	scalex = GBNumber(Width()) / world.Size().x;
	scaley = GBNumber(Height()) / world.Size().y;
}

short GBMiniMapView::ToScreenX(const GBCoordinate x) const {
	return (x * scalex).Floor();
}

short GBMiniMapView::ToScreenY(const GBCoordinate y) const {
	return Height() - (y * scaley).Floor();
}

GBCoordinate GBMiniMapView::FromScreenX(const short h) const {
	return GBNumber(h) / scalex;
}

GBCoordinate GBMiniMapView::FromScreenY(const short v) const {
	return GBNumber(Height() - v) / scaley;
}

GBFinePoint GBMiniMapView::FromScreen(short x, short y) const {
	return GBFinePoint(FromScreenX(x), FromScreenY(y));
}

void GBMiniMapView::Draw() {
	RecalculateScales();
//draw trails
	bool newTrails = false;
	if ( ! trails ) {
		trails = new GBBitmap(Width(), Height(), Graphics());
		newTrails = true;
	}
	if ( newTrails || frameLastDrawn > world.CurrentFrame() ) {
		trails->StartDrawing();
		trails->Graphics().DrawSolidRect(trails->Bounds(), GBColor::black);
		trails->StopDrawing();
	}
// draw background and grid
	if ( showTrails ) BlitAll(*trails, trails->Bounds());
	else DrawBackground(GBColor::black);
	long bTilesX = world.BackgroundTilesX();
	long bTilesY = world.BackgroundTilesY();
	long tx, ty;
	for ( tx = 0; tx < bTilesX; tx ++ ) {
		short x = ToScreenX(kBackgroundTileSize * tx);
		DrawLine(x, 0, x, Height(), GBColor::gray);
	}
	for ( ty = 1; ty <= bTilesY; ty ++ ) {
		short y = ToScreenY(kBackgroundTileSize * ty);
		DrawLine(0, y, Width(), y, GBColor::gray);
	}
// draw objects
	if ( showFood ) DrawLayerFixed(ocFood, 1);
	DrawLayer(ocArea, 2);
	//robots are special because they leave trails
	for ( ty = world.ForegroundTilesY() - 1; ty >= 0; ty -- )
		for ( tx = world.ForegroundTilesX() - 1; tx >= 0; tx -- ) {
			if ( showRobots ) DrawObjectList(world.GetObjects(tx, ty, ocRobot), 2);
			DrawObjectListTrails(world.GetObjects(tx, ty, ocRobot), 2);
		}
	if ( showRobots ) DrawObjectList(world.GetLargeObjects(ocRobot), 2);
	DrawObjectListTrails(world.GetLargeObjects(ocRobot), 2);
	DrawLayer(ocShot, 1);
	if ( showSensors ) DrawLayer(ocSensorShot, 4);
	if ( showDecorations ) DrawLayer(ocDecoration, 1);
// draw viewing area
	GBRect box;
	box.left = ToScreenX(portal.ViewLeft());
	box.top = ToScreenY(portal.ViewTop());
	box.right = ToScreenX(portal.ViewRight());
	box.bottom = ToScreenY(portal.ViewBottom());
	DrawOpenRect(box, GBColor::white);
// record
	portalChanges = portal.ChangeCount();
	worldChanges = world.ChangeCount();
	frameLastDrawn = world.CurrentFrame();
}

bool GBMiniMapView::InstantChanges() const {
	return world.CurrentFrame() >= frameLastDrawn + 2
		|| world.CurrentFrame() < frameLastDrawn
		|| portalChanges != portal.ChangeCount();
}

bool GBMiniMapView::DelayedChanges() const {
	return worldChanges != world.ChangeCount();
}

bool GBMiniMapView::GetFrontClicks() const {
	return true;
}

bool GBMiniMapView::Resizable() const {
	return true;
}

short GBMiniMapView::MinimumWidth() const {
	return world.Size().x.Floor();
}

short GBMiniMapView::MinimumHeight() const {
	return world.Size().y.Floor();
}

short GBMiniMapView::MaximumWidth() const {
	return world.Size().x.Floor() * 8;
}

short GBMiniMapView::MaximumHeight() const {
	return world.Size().y.Floor() * 8;
}

short GBMiniMapView::PreferredWidth() const {
	return world.Size().x.Floor() * 2;
}

short GBMiniMapView::PreferredHeight() const {
	return world.Size().y.Floor() * 2;
}

void GBMiniMapView::SetSize(short width, short height) {
	scalex = scaley = (GBNumber(width) / world.Size().x + GBNumber(height) / world.Size().y) / 2;
	GBView::SetSize((scalex * world.Size().x).Floor(), (scaley * world.Size().y).Floor());
	worldChanges = -1;
	if ( trails ) {
		delete trails;
		trails = nil;
	}
}

void GBMiniMapView::SetBounds(const GBRect & newbounds) {
	GBView::SetBounds(newbounds);
	if ( trails ) {
		delete trails;
		trails = nil;
	}
}

void GBMiniMapView::AcceptClick(short x, short y, int /*clicks*/) {
	AcceptDrag(x, y);
}

void GBMiniMapView::AcceptDrag(short x, short y) {
	RecalculateScales();
	portal.Unfollow();
	portal.ScrollTo(FromScreen(x, y));
}

void GBMiniMapView::AcceptKeystroke(const char what) {
	switch ( what ) {
		case 'r': case 'R': showRobots = ! showRobots; break;
		case 'f': case 'F': showFood = ! showFood; break;
		case 's': case 'S': showSensors = ! showSensors; break;
		case 'd': case 'D': showDecorations = ! showDecorations; break;
		case 't': case 'T': showTrails = ! showTrails; break;
		case '\n': case '\r': portal.FollowRandom(); break;
		case '\t': portal.FollowRandomNear(); break;
		case '`': portal.Refollow(); break;
		case '-': portal.Zoom(-1); break;
		case '+': case '=': portal.Zoom(1); break;
		case '0': portal.ResetZoom(); break;
		default: break;
	}
	worldChanges = -1;
}

const string GBMiniMapView::Name() const {
	return "Minimap";
}

GBCursor GBMiniMapView::Cursor() const {
	return cuCross;
}

