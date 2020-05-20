#include "CommonDefines.h"

bool IsEqualFloat(const float fval, float epsilon)
{
	return std::abs(fval) <= epsilon;
}

bool IsEqualDouble(const double dval, double epsilon)
{
	return std::abs(dval) <= epsilon;
}