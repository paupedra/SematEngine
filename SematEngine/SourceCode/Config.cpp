#include "Application.h"
#include "Config.h"

#include "MFileSystem.h"


ConfigNode::ConfigNode()
{

}

ConfigNode::ConfigNode(JSON_Object* node) : node(node)
{

}

void ConfigNode::Serialize()
{
	uint size = json_serialization_size(rootNode);
	char* buffer = new char[size];
	json_serialize_to_buffer(rootNode,buffer,size);
	App->fileSystem->Save("Library/Scenes/12345.scene", buffer, size);
}


ConfigArray ConfigNode::SetArray(const char* name) //Create a array storing into class -> addthings to array
{
	json_object_set_value(node, name, json_value_init_array());
	return ConfigArray(json_object_get_array(node,name));
}


//ConfigArray ----------------------------------------------
ConfigArray::ConfigArray(JSON_Array* array) : arr(array)
{
	size = json_array_get_count(array);
}

ConfigArray::~ConfigArray()
{

}

void ConfigArray::AddNumber(double number)
{
	json_array_append_number(arr, number);
	size++;
}

void ConfigArray::AddString(const char* string)
{
	json_array_append_string(arr, string);
}

ConfigNode ConfigArray::AddNode()
{
	json_array_append_value(arr, json_value_init_object());
	size++;
	return ConfigNode(json_array_get_object(arr, size - 1));
}