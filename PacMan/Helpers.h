#pragma once
namespace helpers
{
	template<typename T>
	bool is_in_range(T val, T min, T max)
	{
		return val > min && val < max;
	}

	template<typename T>
	bool is_in_vector(const std::vector<T>& vec, T val)
	{
		for (const auto& v : vec) if (v == val) return true;
		return false;
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

	inline int rand_range(const int min, const int max)
	{
		static bool first = true;
		if (first)
		{
			srand(static_cast<unsigned>(time(nullptr)));
			first = false;
		}
		return min + (rand() % (max - min + 1));
	}

	constexpr int world_coord_to_array_index(const int worldCoord)
	{
		const int index = worldCoord / constants::k_gridCellSize;
		if (index < 0) return 0;
		if (index > constants::k_gridSize - 1) return constants::k_gridSize - 1;
		return index;
	}

	constexpr int interpolate(const int color1, const int color2, const float fraction)
	{
		const unsigned char r1 = (color1 >> 16) & 0xff;
		const unsigned char r2 = (color2 >> 16) & 0xff;
		const unsigned char g1 = (color1 >> 8) & 0xff;
		const unsigned char g2 = (color2 >> 8) & 0xff;
		const unsigned char b1 = color1 & 0xff;
		const unsigned char b2 = color2 & 0xff;

		return static_cast<int>((r2 - r1) * fraction + r1) << 16 |
			static_cast<int>((g2 - g1) * fraction + g1) << 8 |
			static_cast<int>((b2 - b1) * fraction + b1);
	}

}
