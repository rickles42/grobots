// GBGraphics.h
// portable graphics layer
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef _GBGraphics_h
#define _GBGraphics_h

#include "GBPlatform.h"

#if MAC
#include <Quickdraw.h>
#include <QDOffscreen.h>
#endif

#include "GBColor.h"
#include <string>
using std::string;

class GBRect {
public:
	short left, top, right, bottom;
	GBRect();
	GBRect(short l, short t, short r, short b);
	short Width() const;
	short Height() const;
	short CenterX() const;
	short CenterY() const;
	void Shrink(short step);
#if MAC
	void ToRect(Rect & r) const;
	GBRect(Rect & r);
#endif
};

class GBBitmap;

class GBGraphics {
#if MAC
	// no data members
	void UseColor(const GBColor & c);
#endif
public:
	GBGraphics();
	~GBGraphics();
// lines
	void DrawLine(short x1, short y1, short x2, short y2,
		const GBColor & color, short thickness = 1);
// rectangles
	void DrawSolidRect(const GBRect & where, const GBColor & color);
	void DrawOpenRect(const GBRect & where, const GBColor & color, short thickness = 1);
// ovals
	void DrawSolidOval(const GBRect & where, const GBColor & color);
	void DrawOpenOval(const GBRect & where, const GBColor & color, short thickness = 1);
	void DrawArc(const GBRect & where, short startAngle, short length,
		const GBColor & color, short thickness = 1);
// strings
	void DrawStringLeft(const string & str, short x, short y,
		short size, const GBColor & color = GBColor::black, bool bold = false);
	void DrawStringRight(const string & str, short x, short y,
		short size, const GBColor & color = GBColor::black, bool bold = false);
	void DrawStringCentered(const string & str, short x, short y,
		short size, const GBColor & color = GBColor::black, bool bold = false);
	void DrawStringPair(const string & str1, const string & str2,
		short left, short right, short y, short size, const GBColor & color, bool useBold);

// blitter
	void Blit(const GBBitmap & src, const GBRect & srcRect, const GBRect & destRect);
};

class GBBitmap {
private:
	GBRect bounds;
#if MAC
	GWorldPtr world;
	CGrafPtr savePort;
	GDHandle saveDevice;
#else
	#warning "Need GBBitmap."
#endif
	GBGraphics graphics;
public:
	GBBitmap(short width, short height);
	~GBBitmap();
	const GBRect & Bounds() const;
	GBGraphics & Graphics();
#if MAC
	BitMapPtr Bits() const;
#endif
// must call StartDrawing and StopDrawing around any drawing to the bitmap,
//  to allow saving and restoring state :(
	void StartDrawing();
	void StopDrawing();
};

#endif // GBGraphics_h
