// GBColor.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBColor.h"
#include "GBNumber.h"
#include "GBPlatform.h"
#include <math.h>

#if MAC
#include <Quickdraw.h>
#endif

// some things are inline, and can be found in GBColor.h
const float kContrastRedWeight = 0.35;
const float kContrastGreenWeight = 0.5;
const float kContrastBlueWeight = 0.15;

const float kMinTextContrast = 0.4;


GBColor::GBColor(const float grey)
	: r(Limit(grey)), g(Limit(grey)), b(Limit(grey))
{}

GBColor::GBColor(const float red, const float green, const float blue)
	: r(Limit(red)), g(Limit(green)), b(Limit(blue))
{}

void GBColor::Set(const float red, const float green, const float blue) {
	r = Limit(red);
	g = Limit(green);
	b = Limit(blue);
}

float GBColor::Lightness() const {
	return r * 0.35 + g * 0.4 + b * 0.25;
}

const GBColor GBColor::Mix(const float fraction, const GBColor & other) const {
	return *this * fraction + other * (1.0 - fraction);
}

const float GBColor::Contrast(const GBColor & other) const {
	return sqrt((r - other.r) * (r - other.r) * kContrastRedWeight +
			(g - other.g) * (g - other.g) * kContrastGreenWeight +
			(b - other.b) * (b - other.b) * kContrastBlueWeight);
}


float GBColor::Limit(float val) {
	if ( val > 1.0 ) return 1.0;
	if ( val < 0.0 ) return 0.0;
	return val;
}

// this function may not return a color with Lightness>minimum if minimum is > 1/3!
const GBColor GBColor::EnsureContrastWithBlack(const float minimum) const {
	GBColor result;
	float contrast = Contrast(GBColor(0));
	if ( contrast >= minimum )
		return *this;
	if ( contrast == 0.0 )
		return GBColor(minimum);
	result.r = Limit(r * minimum / contrast);
	result.g = Limit(g * minimum / contrast);
	result.b = Limit(b * minimum / contrast);
	return result;
}

// Returns one of two colors which best contrasts with *this. If the primary color
//  is at least cutoff contrast, secondary is not considered.
const GBColor GBColor::ChooseContrasting(const GBColor & primary, const GBColor & secondary,
		const float cutoff) const {
	float cp, cs;
	cp = Contrast(primary);
	cs = Contrast(secondary);
	if ( cp >= cutoff )
		return primary;
	else
		return cp >= cs ? primary : secondary;
}

const GBColor GBColor::ContrastingTextColor() const {
	return GBColor::white.ChooseContrasting(*this, GBColor::black, kMinTextContrast);
}

const GBColor GBColor::operator *(float multiplier) const {
	return GBColor(r * multiplier, g * multiplier, b * multiplier);
}

const GBColor GBColor::operator /(float divisor) const {
	if ( ! divisor ) throw GBDivideByZeroError();
	return GBColor(r / divisor, g / divisor, b / divisor);
}

const GBColor GBColor::operator +(const GBColor & other) const {
	return GBColor(r + other.r, g + other.g, b + other.b);
}

const GBColor GBColor::red(1, 0, 0);
const GBColor GBColor::green(0, 1, 0);
const GBColor GBColor::blue(0, 0, 1);
const GBColor GBColor::cyan(0, 1, 1);
const GBColor GBColor::magenta(1, 0, 1);
const GBColor GBColor::yellow(1, 1, 0);
const GBColor GBColor::black(0);
const GBColor GBColor::white(1);
const GBColor GBColor::gray(0.5);
const GBColor GBColor::lightGray(0.8);
const GBColor GBColor::darkGray(0.2);
const GBColor GBColor::purple(0.6, 0, 0.8);
const GBColor GBColor::darkGreen(0, 0.5, 0);
const GBColor GBColor::darkRed(0.7, 0, 0);

