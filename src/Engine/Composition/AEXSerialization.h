#pragma once
#include "..\..\extern\Json\json.hpp"

using json = nlohmann::json;

namespace AEX
{
	struct AEVec2;
	struct AEVec3;
	struct Transform;
	struct TransformComp;

	// basic types - write
	json & operator<<(json &j, const int & val);
	json & operator<<(json &j, const float & val);
	json & operator<<(json &j, const double & val);
	json & operator<<(json &j, const bool & val);
	json & operator<<(json &j, const std::string & val);

	// complex types - write
	nlohmann::json& operator<<(nlohmann::json& j, const AEVec2 & v);
	nlohmann::json& operator<<(nlohmann::json& j, const AEVec3 & v);
	nlohmann::json& operator<<(nlohmann::json& j, const Transform & tr);

	// basic types - read
	int & operator >> (const json &j, int & val);
	float & operator >> (const json &j, float & val);
	double& operator >> (const json &j, double & val);
	bool & operator >> (const json &j, bool & val);
	std::string & operator >> (const json &j, std::string & val);

	// complex types - read
	AEVec2& operator >> (const nlohmann::json& j, AEVec2& v);
	AEVec3& operator >> (const nlohmann::json& j, AEVec3& v);
	Transform& operator >> (const nlohmann::json& j, Transform& mtr);

	// ISerializable struct
	struct ISerializable
	{
		virtual json& operator<< (json&j) const { return j; }
		virtual void operator >> (json&j) { }
		virtual std::ostream& operator << (std::ostream & o) const
		{
			json j;
			this->operator<<(j);
			return o << j;
		}
	};
}

/*
void TestReadWriteWithJson()
{
	MyTransform tr;

	// load
	{
		json value;
		std::ifstream inFile("input.json");
		if (inFile.good() && inFile.is_open())
		{
			inFile >> value;
			inFile.close();
			tr.FromJson(value);
		}
	}

	// save
	{
		json value;

		tr.ToJson(value);

		// save to a file. 
		std::ofstream outFile("test.json");
		if (outFile.good() && outFile.is_open())
		{
			outFile << std::setw(4) << value;
			outFile.close();
		}
	}
}
*//*
#pragma region place in factory.cpp


#pragma endregion
*/// overload the stream operator for json for known types
/*


	

class GameObject2 : public AEX::GameObject, public ISerializable2
{
public:
	
};
*/
/*!
/details
We know we're going to put the serialization interface in the IBase class
or at the very least, for the purist, we'll integrate serialziation at the IComp
class level.

While the system in the previous lesson is robust enough to handle serialization
we'll see that for custom components, a lot of manual labor will be involved

j["health"] = ...
j["vision_range"] = ...
j["bla_bla_bla"] = ...

This is where properties come in, they help get rid of the tedious and error-prone
task of saving the *relevant* variables of the custom component, therefore saving
time. An example is the Zero Engine [Property] tag. In Unity, setting a variable as public
will achieve the same effect, while making it private flags it and the serialization process
simply skips it.

// Zilch Script
[Property] var health : Integer;	// saved
var counter : Integer;			// not saved

So how do we go about this? Well, if we want to automatize the writing of the variables to json,
we'll need to store them somewhere, so that the writing process goes over each of the properties
and write them to json.

Now we know we'll have container of some types for our properties. Properties are in essence a variable
with a flag, so that it's saved by the automatic serialization. Also, properties should be able to contain any type
so we'll need *templates* to encapsulate the variable that we want to save. Usage code would look like this:

struct MyCustomComp : public IComp
{
Property<int> health;	// saved
int counter;			// not saved
};


because we are also going to *USE* the variable in the code for logic, ideally, the Property would behave exactly
like the variable. It is a proxy. Concretely, it will act like a *pointer or reference* to the type. Similar to iterators in STL.


struct MyCustomComp
{
//...
void Update()
{
if(health < 0) ...
health++;
health += 2;
}
};

Let's define the property structure

template<typename T> struct Property
{
T val; // THE value the propety represent. Memory, basically.

// conversion operators
int& operator(T)();
int& operator=(const T & rhs);
};

Then, we add the serialization as friend functions,	thus overloading the << operators for the property of type T(more here).
These functions will write to the json value using the << operator. This means that for this to work `typename T` must also
overload them. For this to work, we also need to overload the << and >> operators of atomic data types (int, float, bool...) done above.

template <typename T> struct Property
{
// value

// conversion operators

// stream operators overloads
friend nlohmann::json& operator<<(nlohmann::json& j, const Property<T>& prop);
friend Property<T>& operator>>(nlohmann::json& j, const Property<T>& prop);
};

see #test_property_1 for sample usage.

From the restults of #test_property_1, we see that it doesn't save the name of the property,
but just the value. Funnily enough, the json acts as a proxy, just like the property.
We'll want to add the name of the property, as declared in the custom struct.

property<int> health; // json -> {"health" = ...}

So we need to store the name of the variable somewhere. We could add a variable of type
string to the property and save that in the stream functions. However, we can leverage
the container of our properties to actually store that info. By choosing a key-value container
like a map<string, Property*>, where the value is a pointer to a base interface that overloads the
*/
/*
#include <unordered_map>

class PROP_MAP : public ISerializable2
{
public:
	std::unordered_map<std::string, ISerializable2*> _props_;
	// overload stream operators
	friend nlohmann::json& operator<<(nlohmann::json& j, const PROP_MAP& properties)
	{
		for (auto prop : properties._props_)
		{
			// note i need to call the explicit operator here because the
			// property woould resolve in the value itself and call one of the global 
			// functions instead.
			prop.second->operator<<(j[prop.first]);
		}
		return j;
	}
	friend PROP_MAP& operator >> (nlohmann::json& j, PROP_MAP& properties)
	{
		for (auto prop : properties._props_)
		{
			prop.second->operator >> (j[prop.first]);
		}
		return properties;
	}
	friend std::ostream& operator << (std::ostream & o, const PROP_MAP& properties)
	{
		json j;
		o << properties.operator<<(j);

		return o;
	}
	virtual json& operator<< (json&j) const
	{
		for (auto prop : this->_props_)
		{
			prop.second->operator<<(j[prop.first]);
		}
		return j;
	}
	virtual void operator >> (json&j)
	{
		for (auto prop : this->_props_)
		{
			prop.second->operator >> (j[prop.first]);
		}
	}
	virtual std::ostream& operator << (std::ostream & o) const
	{
		json j;
		o << this->operator<<(j);

		return o;
	}
};
*/
/*!
/details

Property class acts as a proxy for variable typename T. It's a proxy
this is a common programming pattern.

Additionally, Property implements the ISerializable2 interface to, again
act as a proxy for `val`. This means that whatever the type of `val` is, it
must also implement the serializable interface. Note that for basic data types,
specifically int, `float`, `double`, `bool` and `string`.
*/
/*
template <typename T> struct Property : public ISerializable2
{

	// value
	T val;

	// constructors
	Property() {}
	Property(const char * name, PROP_MAP& prop_map)
	{
		// dont' allow duplicates
		assert(prop_map._props_.find(name) == prop_map._props_.end());
		prop_map._props_[name] = this;
	}
	Property(const char * name, const T &value, PROP_MAP& prop_map)
		:Property(name, prop_map)
	{

		val = value;
	}

	// conversion operators
	operator T() const { return val; }
	T& operator=(const T & rhs) { val = rhs; return val; }

	// overload stream operators
	friend nlohmann::json& operator<<(nlohmann::json& j, const Property<T>& prop)
	{
		j << prop.val;
		return j;
	}
	friend Property<T>& operator >> (nlohmann::json& j, Property<T>& prop)
	{
		j >> prop.val;
		return prop;
	}
	friend std::ostream& operator << (std::ostream & o, const Property<T>&prop)
	{
		json j;
		j << prop.val;

		o << j;
		return o;
	}

	virtual json& operator<< (json&j) const
	{
		j << val;
		return j;
	}
	virtual void operator >> (json&j) {
		j >> val;

	}
	virtual std::ostream& operator << (std::ostream & o)  const
	{
		json j;
		j << val;

		o << j;
		return o;
	}
};

// these maccros will help when declaring properties. 
// they leverage moderne C++ initializer lists to initialize the values (PROP_VAL and PROP_VAL_EX)
// see test functions for sample usage

// properties container passed explicitly. 
#define PROP_EX(_type_, _name_, _properties_) Property<_type_> _name_ {#_name_, _properties_}
#define PROP_VAL_EX(_type_, _name_, val, _properties_) Property<_type_> _name_ {#_name_, val, _properties_}

// these assume that a container variable called 'properties' exists in the local scope of the code
// that uses these maccros. This is intended to work with the IComp3 class example below. 
#define PROP(_type_, _name_) PROP_EX(_type_, _name_, properties)
#define PROP_VAL(_type_, _name_, val)  PROP_VAL_EX(_type_, _name_, val, properties)
*/