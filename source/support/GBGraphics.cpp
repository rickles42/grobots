// GBGraphics.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBGraphics.h"
#include "GBStringUtilities.h"

// GBRect //

GBRect::GBRect() : left(0), top(0), right(0), bottom(0) {}

GBRect::GBRect(short l, short t, short r, short b)
	: left(l), top(t), right(r), bottom(b) {}

short GBRect::Width() const { return right - left; }
short GBRect::Height() const { return bottom - top; }
short GBRect::CenterX() const { return (right + left) >> 1; }
short GBRect::CenterY() const { return (top + bottom) >> 1; }

void GBRect::Shrink(short step) {
	left += step;
	top += step;
	right -= step;
	bottom -= step;
}

#if MAC
void GBRect::ToRect(Rect & r) const {
	r.top = top;
	r.left = left;
	r.right = right;
	r.bottom = bottom;
}

GBRect::GBRect(Rect & r)
	: left(r.left), top(r.top), right(r.right), bottom(r.bottom) {}
#endif

// GBGraphics //

#if MAC
void GBGraphics::UseColor(const GBColor & c) {
	RGBColor color;
	color.red = 0xFFFF * c.Red();
	color.green = 0xFFFF * c.Green();
	color.blue = 0xFFFF * c.Blue();
	RGBForeColor(&color);
}

GBGraphics::GBGraphics() {}
GBGraphics::~GBGraphics() {}

void GBGraphics::DrawLine(short x1, short y1, short x2, short y2,
		const GBColor & color, short thickness) {
	UseColor(color);
	PenSize(thickness, thickness);
	MoveTo(x1, y1);
	LineTo(x2, y2);
}

void GBGraphics::DrawSolidRect(const GBRect & where, const GBColor & color) {
	Rect r;
	where.ToRect(r);
	UseColor(color);
	PaintRect(&r);
}

void GBGraphics::DrawOpenRect(const GBRect & where, const GBColor & color, short thickness) {
	Rect r;
	where.ToRect(r);
	UseColor(color);
	PenSize(thickness, thickness);
	FrameRect(&r);
}

void GBGraphics::DrawSolidOval(const GBRect & where, const GBColor & color) {
	Rect r;
	where.ToRect(r);
	UseColor(color);
	PaintOval(&r);
}

void GBGraphics::DrawOpenOval(const GBRect & where, const GBColor & color, short thickness) {
	Rect r;
	where.ToRect(r);
	UseColor(color);
	PenSize(thickness, thickness);
	FrameOval(&r);
}

//startAngle: degrees clockwise from up
//length: degrees
void GBGraphics::DrawArc(const GBRect & where, short startAngle, short length,
		const GBColor & color, short thickness) {
	Rect r;
	where.ToRect(r);
	UseColor(color);
	PenSize(thickness, thickness);
	FrameArc(&r, startAngle, length);
}

void GBGraphics::DrawStringLeft(const string & str, short x, short y,
		short size, const GBColor & color, bool useBold) {
	Str255 s;
	ToPascalString(str, s);
	UseColor(color);
	TextSize(size);
	TextFace(useBold ? bold : normal);
	MoveTo(x, y);
	DrawString(s);
}

void GBGraphics::DrawStringRight(const string & str, short x, short y,
		short size, const GBColor & color, bool useBold) {
	Str255 s;
	ToPascalString(str, s);
	UseColor(color);
	TextSize(size);
	TextFace(useBold ? bold : normal);
	MoveTo(x - StringWidth(s), y);
	DrawString(s);
}

void GBGraphics::DrawStringCentered(const string & str, short x, short y,
		short size, const GBColor & color, bool useBold) {
	Str255 s;
	ToPascalString(str, s);
	UseColor(color);
	TextSize(size);
	TextFace(useBold ? bold : normal);
	MoveTo(x - StringWidth(s) / 2, y);
	DrawString(s);
}

void GBGraphics::DrawStringPair(const string & str1, const string & str2,
		short left, short right, short y, short size, const GBColor & color, bool useBold) {
	DrawStringLeft(str1, left, y, size, color, useBold);
	DrawStringRight(str2, right, y, size, color, useBold);
}


void GBGraphics::Blit(const GBBitmap & src, const GBRect & srcRect, const GBRect & destRect) {
	Rect r1, r2;
	srcRect.ToRect(r1);
	destRect.ToRect(r2);
	GrafPtr port;
	GetPort(&port);
	ForeColor(blackColor);
	CopyBits(src.Bits(), &(port->portBits), &r1, &r2, srcCopy, nil);
}
#else
	#warning "Need implementation of GBGraphics."
#endif

// GBBitmap //

const GBRect & GBBitmap::Bounds() const {
	return bounds;}

GBGraphics & GBBitmap::Graphics() {
	return graphics;}

#if MAC
GBBitmap::GBBitmap(short width, short height)
	: bounds(0, 0, width, height),
	world(nil),
	savePort(nil), saveDevice(nil),
	graphics()
{
	Rect r;
	bounds.ToRect(r);
	if ( NewGWorld(&world, 0, &r, nil, nil, 0) )
		throw GBOutOfMemoryError();
}

GBBitmap::~GBBitmap() {
	if ( world ) {
		DisposeGWorld(world);
		world = nil;
	}
}

BitMapPtr GBBitmap::Bits() const {
	return *(BitMapHandle)GetGWorldPixMap(world);
}

void GBBitmap::StartDrawing() {
	GetGWorld(&savePort, &saveDevice);
	SetGWorld(world, nil);
}

void GBBitmap::StopDrawing() {
	SetGWorld(savePort, saveDevice);
}
#else
	#warning "Need implementation of GBBitmap."
#endif

