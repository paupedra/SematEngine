
#include "Globals.h"
#include "Dependecies/parson/parson.h"

class ConfigArray;

class ConfigNode
{
public:
	ConfigNode();
	ConfigNode(JSON_Object* node);

	uint Serialize(char** buffer);

	void AddNumber(const char* name, double number);
	void AddString(const char* name, const char* string);

	ConfigArray InitArray(const char* name);

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