// GBPlatform.h
// Platform-specificity definitions, mostly conditional macros.
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBPlatform_h
#define GBPlatform_h

#if defined __MRC__  // MrCpp
	#define MAC 1
	#if CARBON
		#define ACCESSOR_CALLS_ARE_FUNCTIONS 1
		#define OPAQUE_TOOLBOX_STRUCTS 1
		//#define TARGET_API_MAC_CARBON 1
	#endif
#elif defined __unix__
	#ifndef UNIX
		#define UNIX 1
	#endif
#elif defined _MSC_VER // MS Visual C++
	#define WINDOWS 1
	#if defined _MFC_VER // Microsoft Foundation Classes
		#define MFCWIN 1
	#endif
#else
	#warning "Can't tell what compiler is being used."
#endif

#ifndef nil
	#define nil 0 // maybe not needed here
#endif

#endif
