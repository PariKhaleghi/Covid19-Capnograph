// Copyright 2022 - Partow Rayan Rastak - All rights reserved

#pragma once

#include <functional>
#include <Arduino.h>
#include <math.h>

#include "../Platform.h"


struct Math
{
	//static constexpr float PI{ 3.14159265358979323846f };
	//static constexpr float HALF_PI = PI * 0.5f;
	static constexpr float INV_PI = 1 / PI;
	static constexpr float RADTODEG = 180.f / PI;
	static constexpr float DEGTORAD = PI / 180.f;

	static constexpr float SMALL_NUMBER{ 1.e-4f };
	static constexpr float SMALLER_NUMBER{ 0.00001f };
	static constexpr float VERY_SMALL_NUMBER{ 1.e-8f };
	static constexpr float BIG_NUMBER{ 3.4e+38f };
	//static constexpr float EULER{ 2.71828182845904523536f };



	template<typename Type>
	static inline constexpr Type Max(Type a, Type b) { return a >= b ? a : b; }

	template<typename Type>
	static inline constexpr Type Min(Type a, Type b) { return a <= b ? a : b; }

	template<typename Type>
	static inline constexpr Type Clamp(Type a, Type min, Type max) { return Max(min, Min(a, max)); }

	static inline float ClampAngle(float a) {
		const float cAngle = Mod(a, 360.f); //(-360,360)
		return cAngle + (cAngle < 0.f) * 360.f; //[0, 360)
	}

	static inline float NormalizeAngle(float a)
	{
		a = ClampAngle(a); //[0,360)

		if (a > 180.f) a -= 360.f;
		return a; //(-180, 180]
	}

	static inline float ClampAngle(float a, float min, float max) {
		const float maxDelta = ClampAngle(max - min) * 0.5f;    // 0..180
		const float rangeCenter = ClampAngle(min + maxDelta);      // 0..360
		const float deltaFromCenter = NormalizeAngle(a - rangeCenter); // -180..180

		// maybe clamp to nearest edge
		if (deltaFromCenter > maxDelta)
		{
			return NormalizeAngle(rangeCenter + maxDelta);
		}
		else if (deltaFromCenter < -maxDelta)
		{
			return NormalizeAngle(rangeCenter - maxDelta);
		}
		return NormalizeAngle(a); // Already in range
	}

	/** Computes absolute value in a generic way */
	template<typename Type>
	static inline constexpr Type Abs(const Type a)
	{
		return (a >= (Type)0) ? a : -a;
	}

	/** Returns 1, 0, or -1 depending on relation of Type to 0 */
	template<typename Type>
	static inline constexpr Type Sign(const Type a)
	{
		return (a > (Type)0) ? (Type)1 : ((a < (Type)0) ? (Type)-1 : (Type)0);
	}

	static inline i32 RoundToInt(float f)
	{
		return (i32)round(f);
	}

	/**
	* Converts a floating point number to an integer which is further from zero, "larger" in absolute value: 0.1 becomes 1, -0.1 becomes -1
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static inline float RoundFromZero(float F)
	{
		return (F < 0.0f) ? FloorToFloat(F) : CeilToFloat(F);
	}

	static inline double RoundFromZero(double F)
	{
		return (F < 0.0) ? FloorToDouble(F) : CeilToDouble(F);
	}

	/**
	* Converts a floating point number to an integer which is closer to zero, "smaller" in absolute value: 0.1 becomes 0, -0.1 becomes 0
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static inline float RoundToZero(float f)
	{
		return (f < 0.0f) ? CeilToFloat(f) : FloorToFloat(f);
	}

	static inline double RoundToZero(double d)
	{
		return (d < 0.0) ? CeilToDouble(d) : FloorToDouble(d);
	}

	/**
	* Converts a floating point number to an integer which is more negative: 0.1 becomes 0, -0.1 becomes -1
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static inline float RoundToNegativeInfinity(float f)
	{
		return FloorToFloat(f);
	}

	static inline double RoundToNegativeInfinity(double d)
	{
		return FloorToDouble(d);
	}

	/**
	* Converts a floating point number to an integer which is more positive: 0.1 becomes 1, -0.1 becomes 0
	* @param F		Floating point value to convert
	* @return		The rounded integer
	*/
	static inline float RoundToPositiveInfinity(float f)
	{
		return CeilToFloat(f);
	}

	static inline double RoundToPositiveInfinity(double d)
	{
		return CeilToDouble(d);
	}

	static inline i32 FloorToI32(float f)
	{
		return i32(FloorToFloat(f));
	}

	static inline i64 FloorToI64(double f)
	{
		return i64(FloorToDouble(f));
	}

	static inline float FloorToFloat(float f)
	{
		return floor(f);
	}

	static inline double FloorToDouble(double d)
	{
		return floor(d);
	}

	static inline i32 CeilToI32(float f)
	{
		return i32(CeilToFloat(f));
	}

	static inline i64 CeilToI64(double f)
	{
		return i64(CeilToDouble(f));
	}

	static inline float CeilToFloat(float f)
	{
		return ceil(f);
	}

	static inline double CeilToDouble(double d)
	{
		return ceil(d);
	}

	template<class T>
	static inline T Sqrt(T val) {
		return sqrt(val);
	}

	template<class T>
	static inline T Square(T val) {
		return val * val;
	}

	// Same as Max but with N arguments
	template<typename Type, typename... Args>
	static inline constexpr Type Max(Type a, Type b, Args... args) {
		return Max(a, Max(b, std::forward<Args>(args)...));
	}

	// Same as Min but with N arguments
	template<typename Type, typename... Args>
	static inline constexpr Type Min(Type a, Type b, Args... args) {
		return Min(a, Min(b, std::forward<Args>(args)...));
	}


	/*template< class T, class U, EnableIfPassByValue(T)>
	static constexpr T Lerp(const T A, const T B, const U Alpha)
	{
		return (T)(A + Alpha * (B - A));
	}*/

	template< class T, class U/*, EnableIfNotPassByValue(T)*/>
	static constexpr T Lerp(const T& a, const T& b, const U& alpha)
	{
		return T(a + (b - a) * alpha);
	}

	static float Mod(float a, float b) { return fmod(a, b); }

	/**
	* Computes the sine and cosine of a scalar value.
	*
	* @param ScalarSin	Pointer to where the Sin result should be stored
	* @param ScalarCos	Pointer to where the Cos result should be stored
	* @param Value  input angles
	*/
	static inline void SinCos(float* ScalarSin, float* ScalarCos, float  Value)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (INV_PI * 0.5f) * Value;
		if (Value >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = Value - (2.0f * PI) * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > HALF_PI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HALF_PI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*ScalarCos = sign * p;
	}

	static float Atan2(float Y, float X);

	// Note:  We use FASTASIN_HALF_PI instead of HALF_PI inside of FastASin(), since it was the value that accompanied the minimax coefficients below.
	// It is important to use exactly the same value in all places inside this function to ensure that FastASin(0.0f) == 0.0f.
	// For comparison:
	//		HALF_PI				== 1.57079632679f == 0x3fC90FDB
	//		FASTASIN_HALF_PI	== 1.5707963050f  == 0x3fC90FDA
#define FASTASIN_HALF_PI (1.5707963050f)
	/**
	* Computes the ASin of a scalar value.
	*
	* @param Value  input angle
	* @return ASin of Value
	*/
	static inline float FastAsin(float Value)
	{
		// Clamp input to [-1,1].
		bool nonnegative = (Value >= 0.0f);
		float x = Math::Abs(Value);
		float omx = 1.0f - x;
		if (omx < 0.0f)
		{
			omx = 0.0f;
		}
		float root = Math::Sqrt(omx);
		// 7-degree minimax approximation
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + FASTASIN_HALF_PI;
		result *= root;  // acos(|x|)
		// acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
		return (nonnegative ? FASTASIN_HALF_PI - result : result - FASTASIN_HALF_PI);
	}
#undef FASTASIN_HALF_PI

	static inline bool NearlyEqual(float a, float b, float tolerance = SMALL_NUMBER) {
		return Abs(b - a) <= tolerance;
	}

	static const float Log(const float k) { return log(k); }
	static const float Log(const float k, float base) { return log(k) / log(base); }

	static const double Log(const double k) { return log(k); }
	static const double Log(const double k, double base) { return log(k) / log(base); }
};
