// GBNumber.cpp
// Grobots (c) 2002-2004 Devon and Warren Schudy
// Distributed under the GNU General Public License.

#include "GBNumber.h"
#include <math.h>
#include "GBStringUtilities.h"

// many methods are inline, and found in GBNumber.h

GBNumber::GBNumber(const double f)
	: data(f * (1 << kNumFractionBits))
{
	if ( f > (double)kMaxValue || f < - (double)kMaxValue )
		throw GBOverflowError();
}

double GBNumber::ToDouble() const {
	return (double)data / (double)(1 << kNumFractionBits);
}

// ToString is no longer a method, and is now in GBStringUtilities.

GBNumber GBNumber::operator*=(const GBNumber factor) {
	GBLongLong temp = ((GBLongLong)data * (GBLongLong)factor.data) >> kNumFractionBits;
	if ( temp > kMaxValueRaw || temp < - kMaxValueRaw )
		throw GBOverflowError();
	data = temp;
	return *this;
}

GBNumber GBNumber::operator/=(const GBNumber divisor) {
	if ( ! divisor.data ) throw GBDivideByZeroError();
	data = ((GBLongLong)data << kNumFractionBits) / (GBLongLong)divisor.data;
	return *this;
}

GBNumber GBNumber::operator/=(const int divisor) {
	if ( ! divisor ) throw GBDivideByZeroError();
	data /= divisor;
	return *this;
}

GBNumber GBNumber::operator/=(const long divisor) {
	if ( ! divisor ) throw GBDivideByZeroError();
	data /= divisor;
	return *this;
}

GBNumber GBNumber::operator*=(const double factor){
	data = factor * data;
	return *this;
}

GBNumber GBNumber::operator/=(const double divisor) {
	if ( ! divisor ) throw GBDivideByZeroError();
	data = data / divisor;
	return *this;
}

GBNumber GBNumber::operator*(const GBNumber factor) const {
	GBLongLong temp = ((GBLongLong)data * (GBLongLong)factor.data) >> kNumFractionBits;
	if ( temp > kMaxValueRaw || temp < - kMaxValueRaw )
		throw GBOverflowError();
	return MakeRaw(temp);
}

GBNumber GBNumber::operator*(const double factor) const {
	return MakeRaw(factor * data);
}

GBNumber GBNumber::operator/(const GBNumber divisor) const {
	if ( ! divisor.data ) throw GBDivideByZeroError();
	return MakeRaw(((GBLongLong)data << kNumFractionBits) / (GBLongLong)divisor.data);
}

GBNumber GBNumber::operator/(const int divisor) const {
	if ( ! divisor ) throw GBDivideByZeroError();
	return MakeRaw(data / divisor);
}

GBNumber GBNumber::operator/(const long divisor) const {
	if ( ! divisor ) throw GBDivideByZeroError();
	return MakeRaw(data / divisor);
}

GBNumber GBNumber::operator/(const double divisor) const {
	if ( ! divisor ) throw GBDivideByZeroError();
	return MakeRaw(data / divisor); // is this right?
}

GBNumber GBNumber::Mod(const GBNumber divisor) const {
	if ( ! divisor.data ) throw GBDivideByZeroError();
	if ( data < 0 ) {
		if ( divisor < 0 ) return MakeRaw(- (- data) % - divisor.data);
		long remainder = (- data) % divisor.data;
		if ( remainder ) return MakeRaw(divisor.data - remainder);
		return 0;
	}
	if ( divisor < 0 ) {
		long remainder = data % - divisor.data;
		if ( remainder ) return MakeRaw(divisor.data + remainder);
		return 0;
	}
	return MakeRaw(data % divisor.data);
}

GBNumber GBNumber::Rem(const GBNumber divisor) const {
	if ( ! divisor.data ) throw GBDivideByZeroError();
	// depending on % being REM for now...
	return MakeRaw(data % divisor.data);
}

bool GBNumber::operator==(const double other) const {
	return ToDouble() == other;}

bool GBNumber::operator!=(const double other) const {
	return ToDouble() != other;}

bool GBNumber::operator<(const double other) const {
	return ToDouble() < other;}

bool GBNumber::operator>(const double other) const {
	return ToDouble() > other;}

bool GBNumber::operator<=(const double other) const {
	return ToDouble() <= other;}

bool GBNumber::operator>=(const double other) const {
	return ToDouble() >= other;}

GBNumber GBNumber::Sqrt() const {
	if ( data < 0 ) throw GBImaginaryError();
	double temp = data / (double)(1 << kNumFractionBits);
	return GBNumber(sqrt(temp));
}

GBNumber sqrt(GBNumber & x) {
	if ( x < 0 ) throw GBImaginaryError();
	double temp = x.GetRaw() / (double)(1 << kNumFractionBits);
	return GBNumber(sqrt(temp));
}

GBNumber GBNumber::Exponent(const GBNumber ex) const {
	return pow(this->ToDouble(), ex.ToDouble());
}

GBNumber pow(GBNumber base, GBNumber ex) {
	return pow(base.ToDouble(), ex.ToDouble());
}

long GBNumber::Ceiling() const {
	if ( data & kFractionalPartMask )
		return (data >> kNumFractionBits) + 1;
	else
		return data >> kNumFractionBits;
}

long GBNumber::Truncate() const {
	if (data < 0)
		return Ceiling();
	else
		return Floor();
}

long GBNumber::Round() const {
	return (*this + GBNumber(0.5)).data >> kNumFractionBits;
}

GBNumber GBNumber::FractionalPart() const {
	return *this - Truncate();
}

GBNumber GBNumber::Reorient() const {
	return (*this + pi - epsilon).Mod(pi * 2) - pi + epsilon;
}

GBNumber GBNumber::Cos() const {
	return cos(this->ToDouble());
}

GBNumber GBNumber::Sin() const {
	return sin(this->ToDouble());
}

GBNumber GBNumber::Tan() const {
	return tan(this->ToDouble());
}

GBNumber GBNumber::ArcCos() const {
	return acos(this->ToDouble());
}

GBNumber GBNumber::ArcSin() const {
	return asin(this->ToDouble());
}

GBNumber GBNumber::ArcTan() const {
	return atan(this->ToDouble());
}

GBNumber GBNumber::ArcTan2(const GBNumber x) const {
	return atan2(this->ToDouble(), x.ToDouble());
}

const GBNumber GBNumber::epsilon = MakeRaw(1);
const GBNumber GBNumber::infinity = MakeRaw(0x7FFFFFFF);
const GBNumber GBNumber::pi = 3.14159265358979;
const GBNumber GBNumber::e = 2.71828182846;

// errors //

GBArithmeticError::GBArithmeticError() {}
GBArithmeticError::~GBArithmeticError() {}

string GBArithmeticError::ToString() const {
	return "arithmetic error";
}

string GBDivideByZeroError::ToString() const {
	return "division by zero";
}

string GBOverflowError::ToString() const {
	return "arithmetic overflow";
}

string GBImaginaryError::ToString() const {
	return "imaginary result";
}

