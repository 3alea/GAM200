// ----------------------------------------------------------------------------
// Project: GAM300 - Sample Engine
// File:	IComp.h
// Purpose:	Header file for the IComp API
// Author:	Thomas Komair
//
// Copyright DigiPen Institute of Technology
// ----------------------------------------------------------------------------
#ifndef AEX_OBJECT_H_
#define AEX_OBJECT_H_
// ----------------------------------------------------------------------------
#include "../Graphics/Texture.h"
#include "AEXComponent.h"

#pragma warning (disable:4251) // dll and STL
namespace AEX
{
	class GameObject : public IBase
	{
		AEX_RTTI_DECL(GameObject, IBase);

	public:

		GameObject();
		virtual ~GameObject();

		// State Methods
		virtual void SetEnabled(bool enabled); // Call Set Enabled on all components
		virtual void Initialize();	// Calls initialize on all components
		virtual void Render();
		virtual void Shutdown();

		// --------------------------------------------------------------------
		#pragma region// COMPONENT MANAGEMENT
		// Getters
		u32 GetCompCount() const;
		IComp* GetComp(u32 index)const;
		IComp* GetComp(const char * type)const;
		IComp* GetComp(const Rtti & type)const;
		IComp* GetCompName(const char * compName, const char *compType = NULL)const;
		u32 GetID() { return mId; }
		char* GetName() { return mName; }

		// Setters
		void SetName(const char * newName) { strcpy(mName, newName); }

		// template
		template<class T>
		T* GetComp(const char * name = NULL);

		template<class T>
		T* GetCompDerived(const char * name = NULL);

		template<typename T>
		T * NewComp(const char * name = NULL);

		// Add/Remove by address
		IComp* AddComp(IComp * pComp);
		void RemoveComp(IComp * pComp);

		// Removes first component encoutered that match the search criteria
		void RemoveCompType(const char * compType);
		void RemoveCompType(const Rtti & compType);
		void RemoveCompName(const char * compName, const char * compType = NULL);

		// Removes all components encoutered that match the search criteria
		void RemoveAllCompType(const char * compType);
		void RemoveAllCompType(const Rtti & compType);
		void RemoveAllCompName(const char * compName, const char * compType = NULL);

		// Remove all components
		void RemoveAllComp();
		#pragma endregion

		#pragma region // SERIALIZATION
		virtual json& operator<< (json&j)  const;
		virtual void operator >> (json&j);
		friend nlohmann::json& operator<<(nlohmann::json& j, const GameObject& obj);
		friend GameObject& operator >> (nlohmann::json& j, GameObject& obj);
		virtual std::ostream& operator << (std::ostream & o) const;
		friend std::ostream& operator<< (std::ostream & o, const GameObject& obj);
		#pragma endregion

		// debug only!!
		std::vector<IComp*> &GetComps()  { return mComps; }
		const std::vector<IComp*> &GetComps() const { return mComps; }
		GameObject* GetChildObj() { return mChildObj; }
		void		SetChildObj(GameObject* childObj) { mChildObj = childObj; }

		void OnGui();
		// --------------------------------------------------------------------

	private:
		void AddIfNotRepeatedComp(const std::vector<IComp *> & compList, const std::string & namespaceString, const std::string & compToCheck);

	public:
		// Management
		bool			mbActive;		// Specifies whether the object should be updated or not.
		bool			mbAlive;		// Specifies whether the object is alive or not.
		bool			mbVisible;		// Specifies whether the object should be rendered or not.

		static int		objectID;		// Object ID

	protected:
		AEX_PTR_ARRAY(IComp) mComps;	// Component list
		f32 mColor[4];					// Normalized color attributes
		bool mbEnabled;					

	private:
		u32				mId;			// Integer Id generated by the ObjectManager when created. Guaranteed to be unique.
		char			mName[32];		// STD string. This is used as a const char * and contains the name of the game object. Doesn't have to be unique
		GameObject*		mChildObj;

		Texture* mTex;

		unsigned int VAO;
		unsigned int VBO;
		unsigned int EBO;
	};



// ----------------------------------------------------------------------------
	template<class T>
	T* GameObject::GetComp(const char * compName)
	{
		// go throught the components and look for the same type
		for (auto it = mComps.begin(); it != mComps.end(); ++it)
		{
			if ((*it)->GetType().IsExactly(T::TYPE()))
			{
				// not same name -> continue
				if (compName && strcmp(compName, (*it)->GetName()) != 0)
					continue;

				// same name or don't care about the name
				// return
				return (T*)(*it);
			}
		}
		return NULL; // not found
	}
	template<class T>
	T* GameObject::GetCompDerived(const char * compName)
	{
		// go throught the components and look for the same type
		for (auto it = mComps.begin(); it != mComps.end(); ++it)
		{
			if ((*it)->GetType().IsDerived(T::TYPE()))
			{
				// not same name -> continue
				if (compName && strcmp(compName, (*it)->GetName()) != 0)
					continue;

				// same name or don't care about the name
				// return
				return (T*)(*it);
			}
		}
		return NULL; // not found
	}

	template<typename T>
	T * GameObject::NewComp(const char * compName) {
		T* newComp = new T;
		if (compName)
			newComp->SetName(compName);
		return dynamic_cast<T*>(AddComp(newComp));
	}

	typedef std::list<GameObject*> OBJECT_PTR_LIST;
}

#pragma warning (default:4251) // dll and STL
// ----------------------------------------------------------------------------
#endif