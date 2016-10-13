#ifndef _POMELOCPP_JSON_H_
#define _POMELOCPP_JSON_H_

#include "pomelo.h"
#include "../Lib/json/json.h"

namespace PomeloCpp
{
	class JsonHelper
	{
	public:
		static Json::Value translate(json_t *json) {
			if (json == NULL)
				return NULL;

			switch(json_typeof(json))
			{
			case JSON_OBJECT:
			{
				Json::Value obj;
				const char *key;
				json_t *value;
				json_object_foreach(json, key, value) {
					obj[key] = translate(value);
				}
				return obj;
			}
			case JSON_ARRAY:
			{
				Json::Value obj;
				int size = json_array_size(json);
				for (int i = 0; i < size; i++) {
					obj[i] = translate(json_array_get(json, i));
				}
				return obj;
			}
			case JSON_STRING:
				return json_string_value(json);
			case JSON_INTEGER:
				return static_cast<int>(json_integer_value(json));
			case JSON_REAL:
				return json_real_value(json);
			case JSON_TRUE:
				return true;
			case JSON_FALSE:
				return false;
			case JSON_NULL:
			default:
				return NULL;
			}
		}

		static json_t* translate(Json::Value value) {
			switch(value.type()) {
			case Json::booleanValue:
				if (value.asBool())
					return json_true();
				else
					return json_false();
			case Json::stringValue:
				return json_string(value.asCString());
			case Json::uintValue:
				return json_integer(value.asUInt());
			case Json::intValue:
				return json_integer(value.asInt());
			case Json::realValue:
				return json_real(value.asDouble());
			case Json::arrayValue:
			{
				json_t *t = json_array();
				for (int i = 0; i < value.size(); i++)
					json_array_append_new(t, translate(value[i]));
				return t;
			}
			case Json::objectValue:
			{
				json_t *t = json_object();
				Json::Value::Members members = value.getMemberNames();
				for(Json::Value::Members::iterator ptr = members.begin(); ptr != members.end(); ptr++)
					json_object_set_new(t, (*ptr).c_str(), translate(value[*ptr]));
				return t;
			}
			case Json::nullValue:
			default:
				return json_null();
			}
		}
	};

	class JsonBuilder
	{
	public:
		static Json::Value object() {
			return Json::Value();
		}
		template<typename T>
		static Json::Value object(T& value) {
			return value;
		}
		template<typename... ARGS>
		static Json::Value object(ARGS... args) {
			Json::Value value;
			_buildObject(value, args...);
			return value;
		}
		static Json::Value array() {
			return Json::Value(Json::arrayValue);
		}
		template<typename... ARGS>
		static Json::Value array(ARGS... args) {
			Json::Value value;
			_buildArray(value, args...);
			return value;
		}

	private:
		template<typename T>
		static Json::Value _buildObject(Json::Value &data, std::string key, T& value) {
			data[key] = value;
			return data;
		}
		template<typename T, typename... ARGS>
		static Json::Value _buildObject(Json::Value &data, std::string key, T& value, ARGS... args) {
			data[key] = value;
			_buildObject(data, args...);
			return value;
		}
		template<typename T>
		static Json::Value _buildArray(Json::Value &data, T& value) {
			data.append(value);
			return data;
		}
		template<typename T, typename... ARGS>
		static Json::Value _buildArray(Json::Value &data, T& value, ARGS... args) {
			data.append(value);
			_buildArray(data, args...);
			return data;
		}
	};
}

namespace Json
{
	class ObjectConstructor {
		Json::Value object;
	public:
		ObjectConstructor() {
			object = Json::Value(Json::objectValue);
		}
		template<typename T>
		ObjectConstructor(T& value) {
			object = value;
		}
		template<typename T, typename... ARGS>
		ObjectConstructor(T first, ARGS... args) {
			_buildObject(first, args...);
		}
		operator Json::Value() const {
			return object;
		}
	private:
		template<typename T>
		Json::Value _buildObject(std::string key, T& value) {
		    object[key] = value;
			return object;
		}
		template<typename T, typename... ARGS>
		Json::Value _buildObject(std::string key, T& value, ARGS... args) {
			object[key] = value;
			_buildObject(args...);
			return object;
		}
	};

	class ArrayConstructor
	{
		Json::Value list;
	public:
		ArrayConstructor() {
			list = Json::Value(Json::arrayValue);
		}
		template<typename T, typename... ARGS>
		ArrayConstructor(T first, ARGS... args) {
			_buildArray(first, args...);
		}
		operator Json::Value() const {
			return list;
		}
	private:
		template<typename T>
		Json::Value _buildArray(T& value) {
			list.append(value);
			return list;
		}
		template<typename T, typename... ARGS>
		Json::Value _buildArray(T& value, ARGS... args) {
			list.append(value);
			_buildArray(args...);
			return list;
		}
	};

	typedef ObjectConstructor Object;
	typedef ArrayConstructor Array;

	class SuperParser
	{
	public:
		template<typename T>
		static void writeObject(Json::Value o, T& value) {
			_assign(value, o);
		}
		template<typename T>
		static void writeObject(Json::Value o, std::string key, T& first) {
			_assign(first, o[key]);
		}
		template<typename T, typename... ARGS>
		static void writeObject(Json::Value o, std::string key, T& first, ARGS&... args) {
			_assign(first, o[key]);
			writeObject(o, args...);
		}

		template<typename T>
		static void writeArray(Json::Value o, int index, T& first) {
			_assign(first, o[index]);
		}
		template<typename T, typename... ARGS>
		static void writeArray(Json::Value o, int index, T& first, ARGS&... args) {
			_assign(first, o[index]);
			writeObject(o, index+1, args...);
		}

	private:
		template<typename T>
		static void _assign(T &v, Json::Value o) {
			v = o;
		}
		template<>
		static void _assign<std::string>(std::string &v, Json::Value o) {
			if (o.isString())
				v = o.asString();
			else
				throw "Json SuperParser type invalid!";
		}
		template<>
		static void _assign<int>(int &v, Json::Value o) {
			if (o.isInt())
				v = o.asInt();
			else
				throw "Json SuperParser type invalid!";
		}
		template<>
		static void _assign<double>(double &v, Json::Value o) {
			if (o.isDouble())
				v = o.asDouble();
			else
				throw "Json SuperParser type invalid!";
		}
		template<>
		static void _assign<bool>(bool &v, Json::Value o) {
			if (o.isBool())
				v = o.asBool();
			else
				throw "Json SuperParser type invalid!";
		}
	};
}

#endif