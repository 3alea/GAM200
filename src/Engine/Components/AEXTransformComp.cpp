#include "AEXTransformComp.h"
#include "../Composition/AEXSerialization.h"
#include "../Imgui/imgui.h"

namespace AEX
{
	// --------------------------------------------------------------------
	#pragma region// TRANSFORM2D

	// --------------------------------------------------------------------
	TransformComp::TransformComp()
	{
		
	}
	// --------------------------------------------------------------------
	TransformComp::~TransformComp()
	{
		
	}
	// --------------------------------------------------------------------
	f32 TransformComp::GetRotationAngle()
	{
		return mLocal.mOrientation;
	}
	// --------------------------------------------------------------------
	AEVec2 TransformComp::GetDirection()
	{
		AEVec2 dir;
		dir.FromAngle(DegToRad(mLocal.mOrientation));
		return dir;
	}
	// --------------------------------------------------------------------
	AEVec2 TransformComp::GetPosition()
	{
		return mLocal.mTranslation;
	}
	// --------------------------------------------------------------------
	AEVec3 TransformComp::GetPosition3D()
	{
		return mLocal.mTranslationZ;
	}
	// --------------------------------------------------------------------
	AEVec2 TransformComp::GetScale()
	{
		return mLocal.mScale;

	}
	// --------------------------------------------------------------------
	void TransformComp::SetDirection(AEVec2 dir)
	{
		mLocal.mOrientation = RadToDeg(dir.GetAngle());
	}
	// --------------------------------------------------------------------
	void TransformComp::SetRotationAngle(f32 angle)
	{
		mLocal.mOrientation = angle;
	}
	// --------------------------------------------------------------------
	void TransformComp::SetPosition(const AEVec2 & pos)
	{
		mLocal.mTranslation = pos;
		mLocal.mTranslationZ.x = pos.x;
		mLocal.mTranslationZ.y = pos.y;
	}
	// --------------------------------------------------------------------
	void TransformComp::SetPosition3D(const AEVec3 & pos)
	{
		mLocal.mTranslationZ = pos;
		mLocal.mTranslation.x = pos.x;
		mLocal.mTranslation.y = pos.y;
	}
	// --------------------------------------------------------------------
	void TransformComp::SetScale(const AEVec2 & scale)
	{
		mLocal.mScale = scale;
	}

	/*void TransformComp::ToJson(json & val) 
	{
		ToJsonVec2(val["position"], mLocal.mTranslation);
		ToJsonVec2(val["scale"], mLocal.mScale);
		val["rotation"] = mLocal.mOrientation;
	}

	void TransformComp::FromJson(json & val)
	{
		// sanity
		if (auto it = val.find("position") != val.end())
			FromJsonVec2(val["position"], mLocal.mTranslation);
		if (auto it = val.find("scale") != val.end())
			FromJsonVec2(val["scale"], mLocal.mScale);
		if (auto it = val.find("rotation") != val.end())
			mLocal.mOrientation = val["rotation"];
	}*/

	// --------------------------------------------------------------------
	AEMtx33 TransformComp::GetModelToWorld()
	{
		return mLocal.GetMatrix();
	}
	// --------------------------------------------------------------------
	AEMtx33 TransformComp::GetWorldToModel()
	{
		return mLocal.GetInvMatrix();
	}
	// --------------------------------------------------------------------
	AEMtx44 TransformComp::GetModelToWorld4x4()
	{
		AEMtx44 scale_mtx = AEMtx44::Scale(mLocal.mScale.x, mLocal.mScale.y, 1.0f);
		AEMtx44 rot_mtx = AEMtx44::RotateXYZ(0, 0, mLocal.mOrientation);
		AEMtx44 trans_mtx = AEMtx44::Translate(mLocal.mTranslationZ.x, mLocal.mTranslationZ.y, mLocal.mTranslationZ.z);
		return (trans_mtx * rot_mtx * scale_mtx);
	}
	// --------------------------------------------------------------------
	AEMtx44 TransformComp::GetWorldToModel4x4()
	{
		AEMtx44 scale_mtx = AEMtx44::Scale(1.0f/mLocal.mScale.x, 1.0f/mLocal.mScale.y, 1.0f);
		AEMtx44 rot_mtx = AEMtx44::RotateXYZ(0, 0, -mLocal.mOrientation);
		AEMtx44 trans_mtx = AEMtx44::Translate(-mLocal.mTranslationZ.x, -mLocal.mTranslationZ.y, -mLocal.mTranslationZ.z);
		return(scale_mtx*rot_mtx*trans_mtx);
	}


	#pragma endregion

	// --------------------------------------------------------------------
	#pragma region// TRANSFORM3D

	// --------------------------------------------------------------------
	TransformComp3D::TransformComp3D()
	{}
	// --------------------------------------------------------------------
	TransformComp3D::~TransformComp3D()
	{}
	// --------------------------------------------------------------------
	AEVec3	TransformComp3D::GetRotationXYZ()
	{
		return mLocal.rot.ToEulerXYZ();
	}
	// --------------------------------------------------------------------
	AEMtx33 TransformComp3D::GetRotationMtx33()
	{
		// get a rotation matrix
		AEMtx44 mtx44 = GetRotationMtx44();
		AEMtx33 mtx33;
		// copy data
		for (u32 i = 0; i < 3; ++i)
			for (u32 j = 0; j < 3; ++j)
				mtx33.m[i][j] = mtx44.v[i*4+j];
		// return
		return mtx33;
	}
	// --------------------------------------------------------------------
	AEMtx44 TransformComp3D::GetRotationMtx44()
	{
		return mLocal.rot.ToMtx44();
	}
	// --------------------------------------------------------------------
	AEVec3	TransformComp3D::GetPosition()
	{
		return mLocal.position;
	}
	// --------------------------------------------------------------------
	AEVec3	TransformComp3D::GetScale()
	{
		return mLocal.scale;
	}
	// --------------------------------------------------------------------
	AEMtx44 TransformComp3D::GetModelToWorld()
	{
		return mLocal.GetMatrix();
	}
	// --------------------------------------------------------------------
	AEMtx44 TransformComp3D::GetWorldToModel()
	{
		return mLocal.GetInverseMatrix();
	}
	// --------------------------------------------------------------------
	void TransformComp3D::SetRotationXYZRad(f32 xRad, f32 yRad, f32 zRad)
	{
		mLocal.rot.FromEulerXYZ(xRad, yRad, zRad);
	}
	// --------------------------------------------------------------------
	void TransformComp3D::SetRotationXYZDeg(f32 xDeg, f32 yDeg, f32 zDeg)
	{
		SetRotationXYZRad(DegToRad(xDeg), DegToRad(yDeg), DegToRad(zDeg));
	}
	// --------------------------------------------------------------------
	void TransformComp3D::SetPosition(const AEVec3 & pos)
	{
		mLocal.position = pos;
	}
	// --------------------------------------------------------------------
	void TransformComp3D::SetScale(const AEVec3 & scale)
	{
		mLocal.scale = scale;
	}
	// --------------------------------------------------------------------
	void TransformComp3D::SetScale(f32 sx, f32 sy, f32 sz)
	{
		mLocal.scale = AEVec3(sx, sy, sz);
	}
	// --------------------------------------------------------------------
	void TransformComp3D::SetScale(f32 sc)
	{
		mLocal.scale = AEVec3(sc, sc, sc);
	}

	#pragma endregion

	json & TransformComp::operator<<(json & j) const
	{
		j["local"] << mLocal;
		return j;
	}
	void TransformComp::operator >> (json & j)
	{
		if (j.find("local") != j.end())
			j["local"] >> mLocal;
	}
	std::ostream & TransformComp::operator<<(std::ostream & o) const
	{
		json j;
		this->operator<<(j);
		return o << j;
	}

	void TransformComp::OnGui()
	{
		if (ImGui::CollapsingHeader("Tranform"))
		{
			float transTemp[3]{ mLocal.mTranslationZ.x, mLocal.mTranslationZ.y, mLocal.mTranslationZ.z };
			ImGui::Text("Translation");
			ImGui::InputFloat3("Translation", transTemp);
			mLocal.mTranslationZ.x = transTemp[0];
			mLocal.mTranslationZ.y = transTemp[1];
			mLocal.mTranslationZ.z = transTemp[2];

			float scaleTemp[2]{ mLocal.mScale.x, mLocal.mScale.y };
			ImGui::Text("Scale");
			ImGui::InputFloat2("Scale", scaleTemp);
			mLocal.mScale.x = scaleTemp[0];
			mLocal.mScale.y = scaleTemp[1];

			ImGui::Text("Rotation");
			ImGui::InputFloat("Rotation", &mLocal.mOrientation);
		}
	}
}