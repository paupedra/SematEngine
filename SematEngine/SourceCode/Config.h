#include "Globals.h"
#include "Dependecies/parson/parson.h"

class JsonArray;

class JsonNode
{
public:
	JsonNode();
	JsonNode(JSON_Object* node);
	JsonNode(const char* buffer);

	uint Serialize(char** buffer);

	void AddNumber(const char* name, double number);
	void AddString(const char* name, const char* string);
	void AddBool(const char* name, bool _bool);
	JsonNode AddNode(const char* name);

	double GetNumber(const char* name);
	const char* GetString(const char* name);
	bool GetBool(const char* name);

	JsonArray GetArray(const char* name);

	JsonArray InitArray(const char* name);

public:

	JSON_Object* node; //current object
	JSON_Value* rootNode; //root
};

class JsonArray
{
public:
	JsonArray(JSON_Array* array);
	~JsonArray();

	void AddNumber(double number);
	void AddString(const char* string);

	JsonNode AddNode();

	double GetNumber(uint index, double number = 0);
	const char* GetString(uint index, const char* string = "");

	JsonNode GetNode(uint index);

public:
	JSON_Array* arr;
	uint size = 0;
};