
#include "Globals.h"
#include "Dependecies/parson/parson.h"

class ConfigArray;

class ConfigNode
{
public:
	ConfigNode();
	ConfigNode(JSON_Object* node);

	void Serialize();

	ConfigArray SetArray(const char* name);

public:

	JSON_Object* node; //current object
	JSON_Value* rootNode; //root
};

class ConfigArray
{
public:
	ConfigArray(JSON_Array* array);
	~ConfigArray();

	void AddNumber(double number);
	void AddString(const char* string);

	ConfigNode AddNode();

public:
	JSON_Array* arr;
	uint size = 0;
};