#pragma once
namespace helpers
{
	template<typename T>
	bool is_in_range(T val, T min, T max)
	{
		return val > min && val < max;
	}

	inline bool approximately_equal(const float a, const float b, const float epsilon = FLT_EPSILON)
	{
		return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	inline bool essentially_equal(const float a, const float b, const float epsilon = FLT_EPSILON)
	{
		return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	inline bool definitely_greater_than(const float a, const float b, const float epsilon = FLT_EPSILON)
	{
		return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}

	inline bool definitely_less_than(const float a, const float b, const float epsilon = FLT_EPSILON)
	{
		return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
	}
}
