#include "stm32f103xe.h"
#include "pt100.h"
#include "math.h"


float PT100_convert(float Rt)
{
	float t;
	
	t=3383.8098-8658.0088*sqrt(0.178481-0.000231*Rt);
	
	return t;
}

