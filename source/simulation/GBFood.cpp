// GBFood.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBFood.h"
#include "GBErrors.h"
#include "GBWorld.h"
#include "GBSide.h"

const GBDistance kFoodMinRadius = 0.1;
const GBRatio kFoodRadiusFactor = 0.01;
const GBRatio kFoodMassPerValue = 0.02;
const GBEnergy kFoodDecayRate = 0.12;

const GBSpeed kFriction = 0.004;
const GBRatio kLinearDragFactor = 0.01;
const GBRatio kQuadraticDragFactor = 0.3;

// GBFood //

void GBFood::Recalculate() {
	radius = value.Sqrt() * kFoodRadiusFactor + kFoodMinRadius;
	mass = value * kFoodMassPerValue;
}

GBFood::GBFood(const GBPosition & where, const GBEnergy val)
	: GBObject(where, val.Sqrt() * kFoodRadiusFactor + kFoodMinRadius, val * kFoodMassPerValue),
	value(val)
{
	if ( val < 0 ) throw GBBadArgumentError();
}

GBFood::GBFood(const GBPosition & where, const GBVelocity & vel, const GBEnergy val)
	: GBObject(where, val.Sqrt() * kFoodRadiusFactor + kFoodMinRadius, vel, val * kFoodMassPerValue),
	value(val)
{
	if ( val < 0 ) throw GBBadArgumentError();
}

GBFood::~GBFood() {}

GBEnergy GBFood::Energy() const {
	return value;
}

GBEnergy GBFood::TakeEnergy(const GBEnergy limit) {
	if ( limit < 0 ) throw GBBadArgumentError();
	if ( value <= limit ) {
		GBEnergy amt = value;
		value = 0;
		Recalculate();
		return amt;
	} else {
		value -= limit;
		if ( value < 0 ) throw GBBadComputedValueError();
		Recalculate();
		return limit;
	}
}

GBEnergy GBFood::MaxTakeEnergy() {
	return value;
}

GBObjectClass GBFood::Class() const {
	if ( value > 0 )
		return ocFood;
	else
		return ocDead;
}

bool GBFood::CollidesWith(GBObjectClass what) const {
	switch ( what ) {
		case ocRobot: case ocSensorShot:
			return true;
		default:
			return false;
	}
}

GBSide * GBFood::Owner() const {
	return nil;
}

void GBFood::Move() {
	GBObject::Move();
	Drag(kFriction, kLinearDragFactor, kQuadraticDragFactor);
}

void GBFood::Act(GBWorld *) {
	value = (value - kFoodDecayRate).Max(0);
	Recalculate();
}

const GBColor GBFood::Color() const {
	return GBColor::white;
}

void GBFood::Draw(GBGraphics & g, const GBRect & where, bool /*detailed*/) const {
	g.DrawSolidRect(where, Color());
}

// GBManna //

GBManna::GBManna(const GBPosition & where, const GBEnergy val)
	: GBFood(where, val)
{}

GBManna::~GBManna() {}

void GBManna::CollectStatistics(GBWorld * world) const {
	world->ReportManna(value);
}

const GBColor GBManna::Color() const {
	return GBColor::green;
}

// GBCorpse //

GBCorpse::GBCorpse(const GBPosition & where, const GBVelocity & vel,
		const GBEnergy val, GBSide * const who, GBSide * const cause)
	: GBFood(where, vel, val),
	owner(who), killer(cause)
{}

GBCorpse::~GBCorpse() {}

GBSide * GBCorpse::Owner() const {
	return owner;
}

GBSide * GBCorpse::Killer() const {
	return killer;
}

void GBCorpse::CollectStatistics(GBWorld * world) const {
	world->ReportCorpse(value);
}

GBNumber GBCorpse::Interest() const {
	return value / 500;
}

const GBColor GBCorpse::Color() const {
	return GBColor::red;
}

void GBCorpse::Draw(GBGraphics & g, const GBRect & where, bool detailed) const {
	GBFood::Draw(g, where, detailed);
	if ( detailed ) {
		g.DrawOpenRect(where, owner->Color());
		if ( killer && where.Width() >= 6 ) {
			GBRect dot(where.CenterX() - 1, where.CenterY() - 1,
				where.CenterX() + 1, where.CenterY() + 1);
			g.DrawSolidRect(dot, killer->Color());
		}
	}
}

