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
		for(const auto& v : vec) if (v == val) return true;
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
		if(first)
		{
			srand(static_cast<unsigned>(time(nullptr)));
			first = false;
		}
		return min + (rand() % (max - min + 1));
	}

	constexpr inline int world_coord_to_array_index(const int worldCoord)
	{
		const int index = worldCoord / constants::k_gridCellSize;
		if(index < 0) return 0;
		if(index > constants::k_gridSize - 1) return constants::k_gridSize - 1;
		return index;
	}
}
