#include "Random.h"
#include "Dependecies/MathGeoLib/include/Algorithm/Random/LCG.h"

UID Random::GenerateUID()
{
	UID uid = 0;

	LCG lcg;

	uid = lcg.Int();

	return uid;
}