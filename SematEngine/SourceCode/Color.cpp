
#include "Color.h"

Color Red = Color(1.0f, 0.0f, 0.0f);
Color Green = Color(0.0f, 1.0f, 0.0f);
Color Blue = Color(0.0f, 0.0f, 1.0f);
Color Black = Color(0.0f, 0.0f, 0.0f);
Color White = Color(1.0f, 1.0f, 1.0f);

Color Color::Convert255()
{
	Color newColor;

	newColor.r *= 255;
	newColor.g *= 255;
	newColor.b *= 255;
	newColor.a *= 255;

	return newColor;
}