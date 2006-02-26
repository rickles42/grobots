// GBAboutBox.cpp
// Grobots (c) 2002-2006 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBAboutBox.h"
#include "GBColor.h"


GBAboutBox::GBAboutBox()
	: GBView()
{}

GBAboutBox::~GBAboutBox() {}

void GBAboutBox::Draw() {
	DrawBackground(GBColor::black);
	DrawStringCentered("Grobots", Width() / 2, 50, 40, GBColor::green, true);
	DrawStringCentered("by Devon and Warren Schudy", Width() / 2, 75, 12, GBColor::white);
	DrawStringCentered("built " __DATE__
		#if CARBON
			" for Carbon"
		#elif MAC
			" for Mac OS Classic"
		#elif WINDOWS
			" for Win32"
		#endif
		, Width() / 2, 95, 10, GBColor::magenta);
	DrawStringLeft("Additional contributors:", 15, 115, 10, GBColor::white);
	DrawStringLeft("Tilendor", 35, 128, 10, GBColor::white);
	DrawStringLeft("Daniel von Fange", 35, 138, 10, GBColor::white);
	DrawStringCentered("http://grobots.sourceforge.net/", Width() / 2, 165, 10, GBColor(0, 0.7f, 1));
	DrawStringLeft("Grobots comes with ABSOLUTELY NO WARRANTY.", 10, 188, 10, GBColor::white);
    DrawStringLeft("This is free software, and you are welcome to", 10, 200, 10, GBColor::white);
	DrawStringLeft("redistribute it under the GNU General Public License.", 10, 212, 10, GBColor::white);
}

short GBAboutBox::PreferredWidth() const {
	return 260;
}

short GBAboutBox::PreferredHeight() const {
	return 225;
}

const string GBAboutBox::Name() const {
	return "About Grobots";
}

