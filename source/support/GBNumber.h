// GBNumber.h
// Fixed-point number class, used everywhere.
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#ifndef GBNumber_h
#define GBNumber_h

#include "GBErrors.h"

class GBNumber {
	long data;
public:
// constructors
	GBNumber();
	GBNumber(const GBNumber & src);
	GBNumber(const double f);
	GBNumber(const long l);
	GBNumber(const int i);
	~GBNumber();
// conversions
	// no implicit conversions
	double ToDouble() const;
	int operator !() const;
// assignment operators
	GBNumber operator=(const GBNumber newvalue);
	GBNumber operator+=(const GBNumber addend);
	GBNumber operator-=(const GBNumber subtrahend);
	GBNumber operator*=(const GBNumber factor);
	GBNumber operator/=(const GBNumber divisor);
	GBNumber operator*=(const int factor);
	GBNumber operator/=(const int divisor);
	GBNumber operator*=(const long factor);
	GBNumber operator/=(const long divisor);
	GBNumber operator*=(const double factor);
	GBNumber operator/=(const double divisor);
// arithmetic
	GBNumber operator+(const GBNumber addend) const;
	GBNumber operator-(const GBNumber subtrahend) const;
	GBNumber operator-() const;
	GBNumber operator*(const GBNumber factor) const;
	GBNumber operator/(const GBNumber divisor) const;
	GBNumber operator*(const int factor) const;
	GBNumber operator/(const int divisor) const;
	GBNumber operator*(const long factor) const;
	GBNumber operator/(const long divisor) const;
	GBNumber operator*(const double factor) const;
	GBNumber operator/(const double divisor) const;
	GBNumber Mod(const GBNumber divisor) const;
	GBNumber Rem(const GBNumber divisor) const;
// comparisons
	bool operator==(const GBNumber other) const;
	bool operator!=(const GBNumber other) const;
	bool operator<(const GBNumber other) const;
	bool operator>(const GBNumber other) const;
	bool operator<=(const GBNumber other) const;
	bool operator>=(const GBNumber other) const;
	bool operator==(const int other) const;
	bool operator!=(const int other) const;
	bool operator<(const int other) const;
	bool operator>(const int other) const;
	bool operator<=(const int other) const;
	bool operator>=(const int other) const;
	bool operator==(const double other) const;
	bool operator!=(const double other) const;
	bool operator<(const double other) const;
	bool operator>(const double other) const;
	bool operator<=(const double other) const;
	bool operator>=(const double other) const;
// misc
	GBNumber Square() const;
	GBNumber Sqrt() const;
	GBNumber Exponent(const GBNumber ex) const;
	bool IsInteger() const;
	bool Nonzero() const;
	bool Zero() const;
	long Floor() const;
	long Ceiling() const;
	long Round() const;
	long Truncate() const;
	GBNumber FractionalPart() const;
	GBNumber Max(const GBNumber other) const;
	GBNumber Min(const GBNumber other) const;
	GBNumber Abs() const;
	GBNumber Signum() const;
// trig
	GBNumber Reorient() const;
	GBNumber Cos() const;
	GBNumber Sin() const;
	GBNumber Tan() const;
	GBNumber ArcCos() const;
	GBNumber ArcSin() const;
	GBNumber ArcTan() const;
	GBNumber ArcTan2(const GBNumber other) const;
// raw access
	static GBNumber MakeRaw(const long raw);
	long GetRaw() const;
	void SetRaw(const long raw);
// constants
	static const GBNumber epsilon;
	static const GBNumber infinity;
	static const GBNumber pi;
	static const GBNumber e;
};

typedef GBNumber GBAngle;
typedef GBNumber GBCoordinate;

// errors //

class GBArithmeticError : public GBError {
public:
	GBArithmeticError();
	~GBArithmeticError();
	string ToString() const;
};

class GBDivideByZeroError : public GBArithmeticError {
public:
	string ToString() const;
};

class GBOverflowError : public GBArithmeticError {
public:
	string ToString() const;
};

class GBImaginaryError : public GBArithmeticError {
public:
	string ToString() const;
};

// implementation //

const int kNumFractionBits = 12;
const long kIntegerPartMask = -1 << kNumFractionBits;
const long kFractionalPartMask = ~ kIntegerPartMask;
const long kMaxValueRaw = 0x7FFFFFFF;
const long kMaxValue = kMaxValueRaw >> kNumFractionBits;


inline GBNumber::GBNumber()
	: data(0)
{}

inline GBNumber::GBNumber(const GBNumber & src)
	: data(src.data)
{}

inline GBNumber::GBNumber(const int i)
	: data(i << kNumFractionBits)
{
	if ( i > kMaxValue || i < - kMaxValue )
		throw GBOverflowError();
}

inline GBNumber::GBNumber(const long l)
	: data(l << kNumFractionBits)
{
	if ( l > kMaxValue || l < - kMaxValue )
		throw GBOverflowError();
}

inline GBNumber::~GBNumber() {}

inline int GBNumber::operator!() const {
	return ! data;
}

inline GBNumber GBNumber::operator=(const GBNumber newvalue) {
	data = newvalue.data;
	return *this;
}

inline GBNumber GBNumber::operator+=(const GBNumber addend) {
	data += addend.data;
	return *this;
}

inline GBNumber GBNumber::operator-=(const GBNumber subtrahend) {
	data -= subtrahend.data;
	return *this;
}

inline GBNumber GBNumber::operator*=(const int factor){
	data *= factor;
	return *this;
}

inline GBNumber GBNumber::operator*=(const long factor){
	data *= factor;
	return *this;
}

inline GBNumber GBNumber::operator+(const GBNumber addend) const {
	return MakeRaw(data + addend.data);
}

inline GBNumber GBNumber::operator-(const GBNumber subtrahend) const {
	return MakeRaw(data - subtrahend.data);
}

inline GBNumber GBNumber::operator-() const {
	return MakeRaw(- data);
}

inline GBNumber GBNumber::operator*(const int factor) const {
	return MakeRaw(data * factor);
}

inline GBNumber GBNumber::operator*(const long factor) const {
	return MakeRaw(data * factor);
}

inline bool GBNumber::operator==(const GBNumber other) const {
	return (data == other.data);}

inline bool GBNumber::operator!=(const GBNumber other) const {
	return (data != other.data);}

inline bool GBNumber::operator<(const GBNumber other) const {
	return (data < other.data);}

inline bool GBNumber::operator>(const GBNumber other) const {
	return (data > other.data);}

inline bool GBNumber::operator<=(const GBNumber other) const {
	return (data <= other.data);}

inline bool GBNumber::operator>=(const GBNumber other) const {
	return (data >= other.data);}

inline bool GBNumber::operator==(const int other) const {
	return (data == other << kNumFractionBits);}

inline bool GBNumber::operator!=(const int other) const {
	return (data != other << kNumFractionBits);}

inline bool GBNumber::operator<(const int other) const {
	return (data < other << kNumFractionBits);}

inline bool GBNumber::operator>(const int other) const {
	return (data > other << kNumFractionBits);}

inline bool GBNumber::operator<=(const int other) const {
	return (data <= other << kNumFractionBits);}

inline bool GBNumber::operator>=(const int other) const {
	return (data >= other << kNumFractionBits);}

inline bool GBNumber::IsInteger() const {
	return ! (data & kFractionalPartMask);
}

inline bool GBNumber::Nonzero() const {
	return data != 0;
}

inline bool GBNumber::Zero() const {
	return data == 0;
}

inline long GBNumber::Floor() const {
	return data >> kNumFractionBits;
}

inline GBNumber GBNumber::Max(const GBNumber other) const {
	if ( *this >= other )
		return *this;
	else
		return other;
}

inline GBNumber GBNumber::Min(const GBNumber other) const {
	if ( *this <= other )
		return *this;
	else
		return other;
}

inline GBNumber GBNumber::Abs() const {
	if ( data < 0 )
		return - *this;
	else
		return *this;
}

inline GBNumber GBNumber::Signum() const {
	if ( data < 0 )
		return -1;
	else if ( data > 0 )
		return 1;
	else
		return 0;
}

inline GBNumber GBNumber::MakeRaw(const long raw) {
	GBNumber rval;
	rval.data = raw;
	return rval;
}

inline long GBNumber::GetRaw() const {
	return data;
}

inline void GBNumber::SetRaw(const long raw) {
	data = raw;
}


#endif
