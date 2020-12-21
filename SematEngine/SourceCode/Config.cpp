#include "Application.h"
#include "Config.h"

#include "MFileSystem.h"
//#include "Dependecies/mmgr/mmgr.h"

JsonNode::JsonNode()
{
	rootNode = json_value_init_object(); //root
	node = json_value_get_object(rootNode);
}

JsonNode::JsonNode(JSON_Object* node) : node(node)
{

}

JsonNode::JsonNode(const char* buffer)
{
	rootNode = json_parse_string(buffer);
	if (rootNode)
	{
		node = json_value_get_object(rootNode);
	}
}

void JsonNode::AddNumber(const char* name, double number)
{
	json_object_set_number(node, name, number);
}

void JsonNode::AddString(const char* name, const char* string)
{
	json_object_set_string(node, name, string);
}

JsonNode JsonNode::AddNode(const char* name)
{
	json_object_set_value(node, name, json_value_init_object());
	return JsonNode(json_object_get_object(node,name));
}

uint JsonNode::Serialize(char** buffer)
{
	uint size = json_serialization_size_pretty(rootNode);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(rootNode,*buffer,size);

	return size;
}

double JsonNode::GetNumber(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONNumber))
		return json_object_get_number(node,name);
	return 0;
}

const char* JsonNode::GetString(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONString))
		return json_object_get_string(node, name);
	return 0;
}

JsonArray JsonNode::GetArray(const char* name)
{
	if (json_object_has_value_of_type(node, name, JSONArray))
		return JsonArray(json_object_get_array(node, name));
	return NULL; //fix this
}

JsonArray JsonNode::InitArray(const char* name) //Create a array storing into class -> addthings to array
{
	json_object_set_value(node, name, json_value_init_array());
	return JsonArray(json_object_get_array(node,name));
}

//ConfigArray ----------------------------------------------
JsonArray::JsonArray(JSON_Array* array) : arr(array)
{
	size = json_array_get_count(array);
}

JsonArray::~JsonArray()
{

}

void JsonArray::AddNumber(double number)
{
	json_array_append_number(arr, number);
	size++;
}

void JsonArray::AddString(const char* string)
{
	json_array_append_string(arr, string);
}

JsonNode JsonArray::AddNode()
{
	json_array_append_value(arr, json_value_init_object());
	size++;
	return JsonNode(json_array_get_object(arr, size - 1));
}

JsonNode JsonArray::GetNode(uint index)
{
	return JsonNode(json_array_get_object(arr, index));
}

double JsonArray::GetNumber(uint index, double number)
{
	if (index < size)
		return json_array_get_number(arr, index);
	return number;
}