// ----------------------------------------------------------------------------
// Project: GAM300 - Sample Engine
// File:	IComp.h
// Purpose:	Header file for the IComp API
// Author:	Thomas Komair
//
// Copyright DigiPen Institute of Technology
// ----------------------------------------------------------------------------
#ifndef AEX_COMPONENT_H_
#define AEX_COMPONENT_H_

#include "..\Core\AEXCore.h"
#include "AEXSerialization.h"

namespace AEX
{
	// ----------------------------------------------------------------------------
	// \class	IComp 
	// \brief	Base component class
	class	IComp : public IBase, public ISerializable
	{
		AEX_RTTI_DECL(IComp, IBase);
		friend class GameObject;

	public:
		IComp();
		virtual ~IComp() = 0;

		// State Methods
		// Overridable methods - each component should implement their own versions of those
		bool IsEnabled();						
		virtual void SetEnabled(bool enabled);
		virtual void Initialize();		// Called when the owner object is finishhed being assembled.
		virtual void Update();			// Called by the system at each update.
		virtual void Shutdown();		// Called by the owner object when destroyed

		// Gets the owner, only the gameobject class can modify this
		GameObject* GetOwner(void);

		// -------------------------------------------------------------------------
		// Serialization
		virtual json& operator<< (json&j)  const;
		virtual void operator>> (json&j);
		virtual std::ostream& operator<< (std::ostream & o) const;
		friend nlohmann::json& operator<< (nlohmann::json& j, const IComp& comp);
		friend IComp & operator>> (nlohmann::json& j, IComp& comp);
		friend std::ostream& operator<< (std::ostream & o, const IComp& comp);
		// -------------------------------------------------------------------------

	private:
		void PushIfNewComponent();

	public:
		GameObject			*mOwner; // owner object
		bool				mbEnabled;
		bool				mbRemovable = true;
		bool				mbGlobalComp = false;
		//static std::vector<const char*> mAllComponentList;
	};

	IComp* GetCompByName(const char * name);
}
// ----------------------------------------------------------------------------
#endif