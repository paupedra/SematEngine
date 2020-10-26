#ifndef __COMPONENTTRANSFORM__
#define __COMPONENTTRANSFORM__

#include "Dependecies/MathGeoLib/src/MathGeoLib.h"
#include "glmath.h"

class Component;

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	void Update() override;

	mat4x4 GetTransform() const;

private:

	mat4x4 transform;

};
#endif //__COMPONENTTRANSFORM__