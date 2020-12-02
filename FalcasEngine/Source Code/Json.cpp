#include "Json.h"
#include "External Libraries/JSON/parson.h"
#include "Globals.h"

JsonObj::JsonObj(const char* buffer) : obj(nullptr)
{
	val = json_parse_string(buffer);
	//i had so many errors here. idk why the first item in the list cant be an array.
	if (val != NULL)
	{
		obj = json_value_get_object(val);
		LOG("File Loaded");
	}
	else
	{
		LOG("Error loading config");
	}
}

JsonObj::JsonObj()
{
	val = json_value_init_object();
	obj = json_value_get_object(val);
}

JsonObj::JsonObj(JSON_Object* obj)
{
	this->obj = obj;
}

JsonObj::~JsonObj()
{
	obj = nullptr;
	val = nullptr;
}

JSON_Object* JsonObj::GetObj()
{
	return obj;
}

JSON_Value* JsonObj::GetVal()
{
	return val;
}

JSON_Array* JsonObj::GetArray(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_array(obj, name);
	else
		return nullptr;
}

uint JsonObj::Save(char** buffer)
{
	uint size = json_serialization_size_pretty(val);
	*buffer = new char[size];
	json_serialize_to_buffer_pretty(val, *buffer, size);
	return size;
}



int JsonObj::GetInt(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_number(obj, name);
	else
		return 0;
}

float JsonObj::GetFloat(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_number(obj, name);
	else
		return 0.f;
}

bool JsonObj::GetBool(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_boolean(obj, name);
	else
		return false;
}

const char* JsonObj::GetString(const char* name)
{
	if (json_object_has_value(obj, name) == 1)
		return json_object_get_string(obj, name);
	else
		return "null";
}

float3 JsonObj::GetFloat3(const char* name)
{
	float3 num;
	JSON_Array* arr;
	arr = json_object_get_array(obj, name);
	num.x = json_array_get_number(arr, 0);
	num.y = json_array_get_number(arr, 1);
	num.z = json_array_get_number(arr, 2);

	return num;
}

Quat JsonObj::GetQuaternion(const char* name)
{
	Quat quat;

	JSON_Array* arr;
	arr = json_object_get_array(obj, name);

	quat.x = json_array_get_number(arr, 0);
	quat.y = json_array_get_number(arr, 1);
	quat.z = json_array_get_number(arr, 2);
	quat.w = json_array_get_number(arr, 3);

	return quat;
}

float4x4 JsonObj::GetFloat4x4(const char* name)
{
	float4x4 num;

	JSON_Array* arr;
	arr = json_object_get_array(obj, name);
	int j = 0;
	for (int i = 0; i < 4; ++i) {
		num.At(i, 0) = json_array_get_number(arr, j);
		num.At(i, 1) = json_array_get_number(arr, j + 1);
		num.At(i, 2) = json_array_get_number(arr, j + 2);
		num.At(i, 3) = json_array_get_number(arr, j + 3);
		j += 4;
	}

	return num;
}

void JsonObj::AddInt(const char* name, int num)
{
	json_object_set_number(obj, name, num);
}

void JsonObj::AddFloat(const char* name, float num)
{
	json_object_set_number(obj, name, num);
}

void JsonObj::AddBool(const char* name, bool b)
{
	json_object_set_boolean(obj, name, b);

}

void JsonObj::AddString(const char* name, const char* string)
{
	json_object_set_string(obj, name, string);
}

void JsonObj::AddFloat3(const char* name, float3 num)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(obj, name, value);
	JSON_Array* arr = json_object_get_array(obj, name);

	json_array_append_number(arr, num.x);
	json_array_append_number(arr, num.y);
	json_array_append_number(arr, num.z);
}

void JsonObj::AddQuat(const char* name, Quat quat)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(obj, name, value);
	JSON_Array* arr = json_object_get_array(obj, name);

	json_array_append_number(arr, quat.x);
	json_array_append_number(arr, quat.y);
	json_array_append_number(arr, quat.z);
	json_array_append_number(arr, quat.w);
}

void JsonObj::AddFloat4x4(const char* name, float4x4 num)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(obj, name, value);
	JSON_Array* arr = json_object_get_array(obj, name);

	for (int i = 0; i < 4; ++i) {
		json_array_append_number(arr, num.At(i,0));
		json_array_append_number(arr, num.At(i,1));
		json_array_append_number(arr, num.At(i,2));
		json_array_append_number(arr, num.At(i,3));
	}
}

JsonArray JsonObj::AddArray(const char* name)
{
	JSON_Value* value = json_value_init_array();
	JSON_Status status = json_object_set_value(obj, name, value);

	if (status == JSONFailure)
	{
		json_value_free(value);
	}
	else
	{
		return JsonArray(json_object_get_array(obj, name));
	}
}


JsonArray::JsonArray(){
	this->array = nullptr;
	val = json_value_init_array();
	json_value_get_array(val);
}

JsonArray::JsonArray(JSON_Array* array)
{
	this->array = array;
}

JsonArray::JsonArray(JSON_Array* array, JSON_Object* obj)
{
	this->array = array;
}

JsonArray::JsonArray(const char* name)
{
	JSON_Value* value = json_value_init_array();
	json_value_get_array(value);
}

JsonArray::~JsonArray()
{	
	array = nullptr;
}

int JsonArray::Size()
{
	return json_array_get_count(array);
}

JsonObj JsonArray::GetObjectInArray(const char* name)
{
	for (size_t i = 0; i < json_array_get_count(array); i++)
	{
		JSON_Object* object = json_array_get_object(array, i);
		const char* object_name = json_object_get_string(object, "name");

		if (strcmp(name, object_name) == 0)
			return JsonObj(object);
	}

	LOG("JSON object %s not found", name);
}

JsonObj JsonArray::GetObjectAt(int index)
{
	JSON_Object* object = json_array_get_object(array, index);
	return JsonObj(object);
}

void JsonArray::AddObject(JsonObj& object)
{
	json_array_append_value(array, object.GetVal());
}

void JsonArray::AddInt(int num)
{
	json_array_append_number(array, num);
}

void JsonArray::AddFloat(float num)
{
	json_array_append_number(array, num);
}

void JsonArray::AddString(const char* string)
{
	json_array_append_string(array, string);
}
