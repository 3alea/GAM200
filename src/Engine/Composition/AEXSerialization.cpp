#include <aexmath\AEXMath.h>
#include "AEXSerialization.h"

namespace AEX
{
	// json - AEVec2 conversions
	void ToJsonVec2(json & val, const AEVec2 & in)
	{
		val["x"] = in.x;
		val["y"] = in.y;
	}
	void FromJsonVec2(json & val, AEVec2 & in)
	{
		in.x = val["x"];
		in.y = val["y"];
	}

	// basic types - write
	json & operator<<(json &j, const int & val) { j = val; return j; }
	json & operator<<(json &j, const float & val) { j = val; return j; }
	json & operator<<(json &j, const double & val) { j = val; return j; }
	json & operator<<(json &j, const bool & val) { j = val; return j; }
	json & operator<<(json &j, const std::string & val) { j = val; return j; }


	// basic types - read
	int & operator >> (const json &j, int & val) { val = j; return val; }
	float & operator >> (const json &j, float & val) { val = j; return val; }
	double& operator >> (const json &j, double & val) { val = j; return val; }
	bool & operator >> (const json &j, bool & val) { val = j; return val; }
	std::string & operator >> (const json &j, std::string & val) { val = j.get<std::string>(); return val; }

	// complex types - write
	nlohmann::json& operator<<(nlohmann::json& j, const AEVec2 & v)
	{
		j["x"] = v.x;
		j["y"] = v.y;
		return j;
	}
	nlohmann::json& operator<<(nlohmann::json& j, const AEVec3 & v)
	{
		j["x"] = v.x;
		j["y"] = v.y;
		j["z"] = v.z;
		return j;
	}
	nlohmann::json& operator<<(nlohmann::json& j, const Transform & tr)
	{
		j["translation"] << tr.mTranslation;
		j["translationZ"] << tr.mTranslationZ;
		j["scale"] << tr.mScale;
		j["rotation"] = tr.mOrientation;
		return j;
	}


	// complex types - read
	AEVec2& operator >> (const nlohmann::json& j, AEVec2& v)
	{
		if (j.find("x") != j.end())
			v.x = j["x"];
		if (j.find("y") != j.end())
			v.y = j["y"];
		return v;
	}
	AEVec3& operator >> (const nlohmann::json& j, AEVec3& v)
	{
		if (j.find("x") != j.end())
			v.x = j["x"];
		if (j.find("y") != j.end())
			v.y = j["y"];
		if (j.find("z") != j.end())
			v.z = j["z"];
		return v;
	}
	Transform& operator >> (const nlohmann::json& j, Transform& mtr)
	{
		if (j.find("translation") != j.end())
			j["translation"] >> mtr.mTranslation;
		if (j.find("translationZ") != j.end())
			j["translationZ"] >> mtr.mTranslationZ;
		if (j.find("scale") != j.end())
			j["scale"] >> mtr.mScale;
		if (j.find("rotation") != j.end())
			mtr.mOrientation = j["rotation"];
		return mtr;
	}
}
