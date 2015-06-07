#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <cstdlib>

class Utility
{
public:

	static float RandomFloat(float _min = 0.0f, float _max = 1.0f)
	{
		return _min +
			   static_cast <float> (rand()) /
			   (static_cast <float> (RAND_MAX / (_max - _min)));
	}

	static float Difference(float _a, float _b)
	{
		float diff = _a - _b;
		return diff > 0 ? diff : -diff;
	}

};

#endif