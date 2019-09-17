// ----------------------------------------------------------------------------
// Project: GAM300 - Sample Engine
// File:	IComp.h
// Purpose:	Header file for the IComp API
// Author:	Thomas Komair
//
// Copyright DigiPen Institute of Technology
// ----------------------------------------------------------------------------
#include "AEXComponent.h"
#include "AEXGameObject.h"

#include "../Components/AEXComponents.h"

namespace AEX
{
	// ----------------------------------------------------------------------------
	// Constructors
	IComp::IComp() : mOwner(NULL), mbEnabled(true) { PushIfNewComponent(); }

	IComp::~IComp() {}

	GameObject* IComp::GetOwner(void)
	{
		// return a shared pointer
		return mOwner;
	}

	json & IComp::operator<<(json & j) const { return j; }

	void IComp::operator >> (json & j) { }

	std::ostream & IComp::operator<<(std::ostream & o) const
	{
		json j;
		this->operator<<(j);
		return o << j;
	}

	// @TODO
	void IComp::PushIfNewComponent()
	{
		/*for (unsigned i = 0; i < mAllComponentList.size(); i++)
			if (mAllComponentList[i] == this->GetType().GetName())
				return;

		mAllComponentList.push_back(this->GetType().GetName());*/
	}

	// ----------------------------------------------------------------------------
	// PUBLIC - State Methods - By default they do nothing in this component
	//void IComp::OnCreate()
	//{}
	void IComp::Initialize()
	{}
	void IComp::Shutdown()
	{}
	void IComp::Update()
	{}

	// ----------------------------------------------------------------------------
	bool IComp::IsEnabled()
	{
		return mbEnabled;
	}

	void IComp::SetEnabled(bool enabled)
	{
		mbEnabled = enabled;
	}

	nlohmann::json & operator<<(nlohmann::json & j, const IComp & comp)
	{
		comp.operator<<(j);
		return j;
	}

	IComp & operator >> (nlohmann::json & j, IComp & comp)
	{
		comp.operator >> (j);
		return comp;
	}

	std::ostream & operator<<(std::ostream & o, const IComp & comp)
	{
		json j;
		comp.operator<<(j);
		return o << j;
	}

	IComp * GetCompByName(const char * name)
	{
		if (!strcmp(name, "TransformComp"))
			return new TransformComp;
		if (!strcmp(name, "RigidBody"))
			return new RigidBody;
		if (!strcmp(name, "Collider"))
			return new Collider;
	}
}