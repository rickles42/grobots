// GBDecorations.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBDecorations.h"


const GBDistance kSmokeRadius = 0.4;
const GBFrames kSmokeHalfBrightnessTime = 20;

const GBFrames kBlasterSparkLifetime = 8;
const GBDistance kBlasterSparkMaxRadius = 0.3125;
const GBDistance kBlasterSparkGrowthRate = 0.03125;

const GBDistance kTransmissionMinRadius = 0.2;
const GBDistance kTransmissionMaxRadius = 1.5;
const GBSpeed kTransmissionGrowthRate = 0.1;
const short kTransmissionRingSpacing = 3;

const GBDistance kSparkleRadius = 0.0625;


// GBTimedDecoration //

GBTimedDecoration::GBTimedDecoration(const GBPosition where, const GBDistance r, const GBFrames howLong)
	: GBObject(where, r),
	lifetime(howLong)
{}

GBTimedDecoration::GBTimedDecoration(const GBPosition where, const GBDistance r, const GBVelocity vel,
		const GBFrames howLong)
	: GBObject(where, r, vel),
	lifetime(howLong)
{}

GBObjectClass GBTimedDecoration::Class() const {
	if ( lifetime > 0 ) return ocDecoration;
	else return ocDead;
}

void GBTimedDecoration::Act(GBWorld *) {
	lifetime --;
}

// GBSmoke //

GBSmoke::GBSmoke(const GBPosition where, const GBVelocity vel, const GBFrames life)
	: GBTimedDecoration(where, kSmokeRadius, vel, life)
{}

const GBColor GBSmoke::Color() const {
	float intensity = 0.8 * (float)lifetime / (lifetime + kSmokeHalfBrightnessTime);
	return GBColor(intensity);
}

void GBSmoke::Draw(GBGraphics & g, const GBRect & where, bool /*detailed*/) const {
	g.DrawSolidOval(where, Color());
}

// GBBlasterSpark //

GBBlasterSpark::GBBlasterSpark(const GBPosition where)
	: GBTimedDecoration(where, kBlasterSparkMaxRadius, kBlasterSparkLifetime)
{}

void GBBlasterSpark::Act(GBWorld * world) {
	GBTimedDecoration::Act(world);
	radius = kBlasterSparkMaxRadius - kBlasterSparkGrowthRate * (lifetime - 1);
}

const GBColor GBBlasterSpark::Color() const {
	return GBColor::white;
}

void GBBlasterSpark::Draw(GBGraphics & g, const GBRect & where, bool /*detailed*/) const {
	g.DrawOpenOval(where, Color());
}

// GBTransmission //

GBTransmission::GBTransmission(const GBPosition where, int cnt, bool msg)
	: GBObject(where, kTransmissionMinRadius),
	count(cnt), message(msg)
{}

void GBTransmission::Act(GBWorld * world) {
	GBObject::Act(world);
	if ( radius >= kTransmissionMaxRadius ) {
		count --;
	} else
		radius += kTransmissionGrowthRate;
}

const GBColor GBTransmission::Color() const {
	if ( message ) return GBColor(0.6, 0.5, 1);
	return GBColor(1, 0.8, 0.5);
}

GBObjectClass GBTransmission::Class() const {
	if ( count > 0 ) return ocDecoration;
	else return ocDead;
}

void GBTransmission::Draw(GBGraphics & g, const GBRect & where, bool /*detailed*/) const {
	GBRect box = where;
	for ( int i = count; i > 0 && box.left < box.right; i -- ) {
		g.DrawOpenOval(box, Color());
		box.Shrink(kTransmissionRingSpacing);
	}
}

void GBTransmission::DrawMini(GBGraphics & g, const GBRect & where) const {
	g.DrawOpenOval(where, Color());
}

// GBSparkle //

GBSparkle::GBSparkle(const GBPosition where, const GBVelocity vel,
		const GBColor & col, const GBFrames life)
	: GBTimedDecoration(where, kSparkleRadius, vel, life),
	color(col)
{}

const GBColor GBSparkle::Color() const {
	return color;
}

void GBSparkle::Draw(GBGraphics & g, const GBRect & where, bool /*detailed*/) const {
	g.DrawSolidRect(where, color);
}

void GBSparkle::DrawMini(GBGraphics & g, const GBRect & where) const {
	g.DrawSolidRect(where, color * 0.7);
}

