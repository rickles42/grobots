// GBObjectWorld.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBObjectWorld.h"
#include "GBRobot.h"
#include "GBSensorShot.h"


// world size must be a multiple of kBackgroundTileSize, but need not
//  be a multiple of kForegroundTileSize
const GBCoordinate kWorldWidth = kBackgroundTileSize * 10;
const GBCoordinate kWorldHeight = kBackgroundTileSize * 10;

const GBDistance kLargeRadius = kForegroundTileSize / 2;
// large objects are stored in the last tile, which is special.

const GBRatio kWallRestitution = 0.9;
const GBRatio kRobotRestitution = 0.9;

const GBDamage kEraseDamage = 10000;


#define FOR_EACH_TILE(i, body) \
	for ( long i = 0; i <= tilesX * tilesY; i ++ ) \
		body

#define FOR_EACH_OBJECT_CLASS(cl, body) \
	for ( GBObjectClass cl = ocRobot; cl < kNumObjectClasses; cl ++ ) \
		body

#define FOR_EACH_OBJECT_LIST(i, cl, body) \
	FOR_EACH_TILE(i, { FOR_EACH_OBJECT_CLASS(cl, body) })

#define FOR_EACH_OBJECT_IN_LIST(list, ob, body) \
	for ( GBObject * ob = (list); ob != nil; ob = ob->next ) \
		body

#define FOR_EACH_OBJECT_IN_LIST_SAFE(list, ob, temp, body) \
	{ \
		GBObject * temp; \
		for ( GBObject * ob = (list); ob != nil; ob = temp ) { \
			temp = ob->next; \
			body \
		} \
	}

#define FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, body) \
	FOR_EACH_OBJECT_LIST(i, cl, { \
		FOR_EACH_OBJECT_IN_LIST(objects[i][cl], ob, body) \
	})

#define FOR_EACH_OBJECT_IN_WORLD_SAFE(i, cl, ob, temp, body) \
	FOR_EACH_OBJECT_LIST(i, cl, { \
		FOR_EACH_OBJECT_IN_LIST_SAFE(objects[i][cl], ob, temp, body) \
	})


void GBObjectWorld::ClearLists() {
// clean up objects
	FOR_EACH_OBJECT_LIST(i, curClass, {
		FOR_EACH_OBJECT_IN_LIST_SAFE(objects[i][curClass], cur, temp, { delete cur; })
		objects[i][curClass] = nil;
	})
// clean up news
	FOR_EACH_OBJECT_IN_LIST_SAFE(news, cur, temp, { delete cur; })
	news = nil;
}

// Puts objects in the appropriate class and tile, and deletes dead ones.
void GBObjectWorld::ResortObjects() {
	try {
		GBObjectTile * old = objects;
		objects = MakeTiles();
		FOR_EACH_OBJECT_LIST(i, cl, {
			FOR_EACH_OBJECT_IN_LIST_SAFE(old[i][cl], cur, temp, { AddObjectDirectly(cur); })
		})
		delete[] old;
		AddNewObjects();
	} catch ( GBError & err ) {
		NonfatalError(string("Error resorting objects: ") + err.ToString());
	}
}

void GBObjectWorld::AddNewObjects() {
	FOR_EACH_OBJECT_IN_LIST_SAFE(news, cur, temp, { AddObjectDirectly(cur); })
	news = nil;
}

void GBObjectWorld::CollideObjectWithWalls(GBObject * ob) {
	GBObjectClass cl = ob->Class();
	if ( ob->Left() < Left() ) {
		GBVelocity vel = ob->Velocity();
		ob->SetVelocity(vel.x.Abs() * kWallRestitution, vel.y);
		if ( cl == ocRobot || cl == ocFood )
			ob->MoveBy(Left() - ob->Left(), 0);
		ob->CollideWithWall();
	} else if ( ob->Right() > Right() ) {
		GBVelocity vel = ob->Velocity();
		ob->SetVelocity(vel.x.Abs() * - kWallRestitution, vel.y);
		if ( cl == ocRobot || cl == ocFood )
			ob->MoveBy(Right() - ob->Right(), 0);
		ob->CollideWithWall();
	}
	if ( ob->Bottom() < Bottom() ) {
		GBVelocity vel = ob->Velocity();
		ob->SetVelocity(vel.x, vel.y.Abs() * kWallRestitution);
		if ( cl == ocRobot || cl == ocFood )
			ob->MoveBy(0, Bottom() - ob->Bottom());
		ob->CollideWithWall();
	} else if ( ob->Top() > Top() ) {
		GBVelocity vel = ob->Velocity();
		ob->SetVelocity(vel.x, vel.y.Abs() * - kWallRestitution);
		if ( cl == ocRobot || cl == ocFood )
			ob->MoveBy(0, Top() - ob->Top());
		ob->CollideWithWall();
	}
}

void GBObjectWorld::MoveAllObjects() {
	try {
		FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, {
			ob->Move();
			CollideObjectWithWalls(ob);
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error moving objects: ") + err.ToString());
	}
}

void GBObjectWorld::CollideAllObjects() {
	for ( long tx = 0; tx < tilesX; tx ++ )
		for ( long ty = 0; ty < tilesY; ty ++ ) {
			long t = ty * tilesX + tx;
			CollideSameTile(t);
		// collide with adjacent tiles
			if ( tx < tilesX - 1 ) {
				CollideTwoTiles(t, t + 1);
				if ( ty < tilesY - 1 )
					CollideTwoTiles(t, t + tilesX + 1);
			}
			if ( ty < tilesY - 1 ) {
				CollideTwoTiles(t, t + tilesX);
				if ( tx > 0 )
					CollideTwoTiles(t, t + tilesX - 1);
			}
		// collide with large-object tile
			CollideTwoTiles(t, tilesX * tilesY);
		}
// intercollide large objects, in case that ever matters
	CollideSameTile(tilesX * tilesY);
}

void GBObjectWorld::CollideSameTile(long t) {
	FOR_EACH_OBJECT_IN_LIST(objects[t][ocRobot], bot, {
		try {
			FOR_EACH_OBJECT_IN_LIST(bot->next, bot2, { bot->SolidCollide(bot2, kRobotRestitution); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robots: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t][ocFood], food, { bot->BasicCollide(food); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robot and food: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t][ocShot], shot, { bot->BasicCollide(shot); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robot and shot: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t][ocArea], area, { bot->BasicCollide(area); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robot and area: ") + err.ToString());
		}
	})
	try {
		FOR_EACH_OBJECT_IN_LIST(objects[t][ocArea], area, {
			FOR_EACH_OBJECT_IN_LIST(objects[t][ocFood], food, { area->BasicCollide(food); })
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error colliding area and food: ") + err.ToString());
	}
	CollideSensors(t, t);
}

void GBObjectWorld::CollideTwoTiles(long t1, long t2) {
	FOR_EACH_OBJECT_IN_LIST(objects[t1][ocRobot], bot, {
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocRobot], bot2, { bot->SolidCollide(bot2, kRobotRestitution); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robots: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocFood], food, { bot->BasicCollide(food); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robot and food: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocShot], shot, { bot->BasicCollide(shot); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robot and shot: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocArea], area, { bot->BasicCollide(area); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding robot and area: ") + err.ToString());
		}
	})
	FOR_EACH_OBJECT_IN_LIST(objects[t1][ocFood], food, {
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocRobot], bot, { food->BasicCollide(bot); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding food and robot: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocArea], area, { food->BasicCollide(area); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding food and area: ") + err.ToString());
		}
	})
	try {
		FOR_EACH_OBJECT_IN_LIST(objects[t1][ocShot], shot, {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocRobot], bot, { shot->BasicCollide(bot); })
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error colliding shot and robot: ") + err.ToString());
	}
	FOR_EACH_OBJECT_IN_LIST(objects[t1][ocArea], area, {
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocRobot], bot, { area->BasicCollide(bot); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding area and robot: ") + err.ToString());
		}
		try {
			FOR_EACH_OBJECT_IN_LIST(objects[t2][ocFood], food, { area->BasicCollide(food); })
		} catch ( GBError & err ) {
			NonfatalError(string("Error colliding area and food: ") + err.ToString());
		}
	})
	CollideSensors(t1, t2);
	CollideSensors(t2, t1);
}

void GBObjectWorld::CollideSensors(long sensorTile, long otherTile) {
	try {
		FOR_EACH_OBJECT_IN_LIST(objects[sensorTile][ocSensorShot], sensor, {
			GBObjectClass seen = ((GBSensorShot *)sensor)->Seen();
			CheckObjectClass(seen);
			if ( sensor->CollidesWith(seen) ) {
				FOR_EACH_OBJECT_IN_LIST(objects[otherTile][seen], ob, {
					if ( sensor->Intersects(ob) )
						sensor->CollideWith(ob);
						// note one-directional collision, since ob mustn't care it's been sensed
				})
			}
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error colliding sensor-shot with other object: ") + err.ToString());
	}
}

void GBObjectWorld::CheckObjectClass(const GBObjectClass cl) const {
	if ( cl < ocRobot || cl >= kNumObjectClasses )
		throw GBBadObjectClassError();
}

long GBObjectWorld::FindTile(const GBPosition where) const {
	long tx = (where.x / kForegroundTileSize).Max(0).Min(tilesX - 1).Floor();
	long ty = (where.y / kForegroundTileSize).Max(0).Min(tilesY - 1).Floor();
	return ty * tilesX + tx;
}

GBObjectTile * GBObjectWorld::MakeTiles() const {
	GBObjectTile * tiles = new GBObjectTile[tilesX * tilesY + 1];
	if ( ! tiles ) throw GBOutOfMemoryError();
	FOR_EACH_OBJECT_LIST(i, cl, { tiles[i][cl] = nil; })
	return tiles;
}

GBObjectWorld::GBObjectWorld()
	: size(kWorldWidth, kWorldHeight),
	tilesX((kWorldWidth / kForegroundTileSize).Ceiling()),
	tilesY((kWorldHeight / kForegroundTileSize).Ceiling()),
	objects(nil),
	news(nil)
{
	objects = MakeTiles();
}

GBObjectWorld::~GBObjectWorld() {
	ClearLists();
	delete[] objects;
}

void GBObjectWorld::EraseAt(const GBPosition & where, const GBDistance radius) {
// modified from ResortObjects. Could be replaced with some dead-marking system
	try {
		GBObjectTile * old = objects;
		objects = MakeTiles();
		FOR_EACH_OBJECT_LIST(i, cl, {
			FOR_EACH_OBJECT_IN_LIST_SAFE(old[i][cl], cur, temp, {
				if ( cur->Position().InRange(where, cur->Radius() + radius) )
					if ( cur->Class() == ocRobot ) { //must stick around a frame for sensors
							((GBRobot *)cur)->Die(nil);
							AddObjectNew(cur); // new so it'll be deleted next resort
						}
					else
						delete cur;
				else
					AddObjectDirectly(cur);
			})
		})
		delete[] old;
	} catch ( GBError & err ) {
		NonfatalError(string("Error erasing objects: ") + err.ToString());
	}
}

void GBObjectWorld::AddObjectNew(GBObject * newOb) {
	if ( ! newOb ) throw GBNilPointerError();
	newOb->next = news;
	news = newOb;
}

void GBObjectWorld::AddObjectDirectly(GBObject * ob) {
	if ( ! ob ) throw GBNilPointerError();
	GBObjectClass dest = ob->Class();
	if ( dest == ocDead )
		delete ob;
	else {
		long tile = ob->Radius() * 2 >= kForegroundTileSize ? tilesX * tilesY : FindTile(ob->Position());
		ob->next = objects[tile][dest];
		objects[tile][dest] = ob;
	}
}

void GBObjectWorld::Resize(const GBFinePoint & newsize) {
	long oldLastTile = tilesX * tilesY;
	size = newsize;
	tilesX = (size.x / kForegroundTileSize).Ceiling();
	tilesY = (size.y / kForegroundTileSize).Ceiling();
// fix tiles
	GBObjectTile * old = objects;
	objects = MakeTiles();
	for ( long i = 0; i <= oldLastTile; i ++ )
		FOR_EACH_OBJECT_CLASS(cl,{
			FOR_EACH_OBJECT_IN_LIST_SAFE(old[i][cl], cur, temp, { AddObjectDirectly(cur); })
		})
	delete[] old;
}

GBFinePoint GBObjectWorld::Size() const {
	return size;
}

GBCoordinate GBObjectWorld::Left() const {
	return 0;
}

GBCoordinate GBObjectWorld::Top() const {
	return size.y;
}

GBCoordinate GBObjectWorld::Right() const {
	return size.x;
}

GBCoordinate GBObjectWorld::Bottom() const {
	return 0;
}

// eventually replace calculation with getting from Rules.
long GBObjectWorld::BackgroundTilesX() const {
	return (size.x / kBackgroundTileSize).Ceiling();
}

long GBObjectWorld::BackgroundTilesY() const {
	return (size.y / kBackgroundTileSize).Ceiling();
}

long GBObjectWorld::ForegroundTilesX() const {
	return tilesX;
}

long GBObjectWorld::ForegroundTilesY() const {
	return tilesY;
}

GBObject * GBObjectWorld::ObjectNear(const GBPosition where, bool hitSensors) const {
	GBObject * best = nil;
	GBDistance dist = 5; // never see objects farther than this
	try {
		FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, {
			if ( (ob->Class() != ocSensorShot || hitSensors)
					&& ob->Class() != ocDecoration
					&& ob->Position().InRange(where, dist) ) {
				best = ob;
				dist = (best->Position() - where).Norm();
			}
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error in GBObjectWorld::ObjectNear: ") + err.ToString());
	}
	return best;
}

GBObject * GBObjectWorld::GetObjects(const long tilex, const long tiley, const GBObjectClass which) const {
	CheckObjectClass(which);
	if ( tilex < 0 || tilex >= tilesX ||
			tiley < 0 || tiley >= tilesY )
		throw GBIndexOutOfRangeError();
	return objects[tilex + tiley * tilesX][which];
}

GBObject * GBObjectWorld::GetLargeObjects(const GBObjectClass which) const {
	CheckObjectClass(which);
	return objects[tilesX * tilesY][which];
}

long GBObjectWorld::CountObjects(const GBObjectClass cl) const {
	CheckObjectClass(cl);
	long count = 0;
	for ( long i = 0; i <= tilesX * tilesY; i ++ )
		for ( GBObject * cur = objects[i][cl]; cur != nil; cur = cur->next )
			count ++;
	return count;
}

GBObject * GBObjectWorld::RandomInterestingObject() const {
	try {
		GBNumber totalInterest = 0;
		FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, {
			totalInterest += ob->Interest();
		})
		if ( totalInterest.Zero() ) return nil;
		FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, {
			GBNumber interest = ob->Interest();
			if ( gRandoms.Boolean(interest / totalInterest) )
				return ob;
			totalInterest -= interest;
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error in GBObjectWorld::RandomInterestingObject: ") + err.ToString());
	}
	return nil;
}

GBObject * GBObjectWorld::RandomInterestingObjectNear(const GBPosition where, GBDistance radius) const {
	try {
		GBNumber totalInterest = 0;
		FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, {
			if ( ob->Position().InRange(where, radius) )
				totalInterest += ob->Interest();
		})
		if ( totalInterest.Zero() ) return nil;
		FOR_EACH_OBJECT_IN_WORLD(i, cl, ob, {
			if ( ob->Position().InRange(where, radius) ) {
				GBNumber interest = ob->Interest();
				if ( gRandoms.Boolean(interest / totalInterest) )
					return ob;
				totalInterest -= interest;
			}
		})
	} catch ( GBError & err ) {
		NonfatalError(string("Error in GBObjectWorld::RandomInterestingObjectNear: ") + err.ToString());
	}
	return nil;
}

