// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	ObjectManager.cpp
// Author			:	Thomas Komair
// Creation Date	:	2015/02/04
// Purpose			:	Implementation file for Object Manager - This is the first implementation 
//						for the ObjectManager
// History          :   2019/02/01 - Alejandro Balea started the lab
// ---------------------------------------------------------------------------
#include "../AEX.h"
#include "AEXObjectManager.h"
#include "../Imgui/imgui.h"
#include "../Core/AEXGlobalVariables.h"
#include "../Components/AEXComponents.h"
#include "AEXFactory.h"

namespace AEX
{
	ObjectManager::ObjectManager() {}

	/**************************************************************************
	*!
	\fn
	Initialize

	\brief
	Initializes the object manager, setting the Id Generator to zero.
	*
	***************************************************************************/
	bool			ObjectManager::Initialize()
	{
		GameObject::objectID = 0;
		mIdGenerator = 0;
		aexFactory->Register<GameObject>();
		return true;
	}

	void ObjectManager::Render()
	{
		for (auto it = mEditorObjects.begin(); it != mEditorObjects.end(); it++)
			(*it)->Render();
	}

	/**************************************************************************
	*!
	\fn
	Shutdown

	\brief
	Calls FreeAllObjects().
	*
	***************************************************************************/
	void			ObjectManager::Shutdown()
	{
		FreeAllObjects();
	}

	GameObject* ObjectManager::CreateGO(const char * name)
	{
		GameObject* newGO = aexFactory->Create<GameObject>();
		return AddObject(newGO, name);
	}

	GameObject* ObjectManager::CreateGO(const char * name, f32 x, f32 y, f32 sx, f32 sy, f32 rot)
	{
		GameObject* newGO = aexFactory->Create<GameObject>();
		return AddObject(newGO, name, x, y, sx, sy, rot);
	}

	/**************************************************************************
	*!
	\fn
	AddObject

	\brief
	Adds the specified game object with default values:
	- mPosition to (0,0)
	- mScale to (1,1)
	- mRotation to 0
	- mpMesh to NULL
	- mpTex to NULL

	\param
	GameObject * newObj : A pointer to the GameObject.
	const char* name : A string which stores the name of the object
	we want to create.
	*
	***************************************************************************/
	GameObject *	ObjectManager::AddObject(GameObject * newObj, const char * name)
	{
		return AddObject(newObj, name, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	}

	/**************************************************************************
	*!
	\fn
	AddObject

	\brief
	Adds the specified game object with the specified values.

	\param
	GameObject * newObj : A pointer to the GameObject.
	const char* name : A string which stores the name of the object
	we want to create.
	f32 x : The x position of the GameObject.
	f32 y : The y position of the GameObject.
	f32 sx : The x value of the scale of the GameObject.
	f32 sy : The y value of the scale of the GameObject.
	f32 rot : The rotation of the GameObject.
	AEGfxTriList * pMesh : The mesh of the GameObject.
	AEGfxTexture * pTex : The texture which will be used in the GameObject.
	*
	***************************************************************************/
	GameObject *	ObjectManager::AddObject(GameObject * newObj, const char * name, f32 x, f32 y, f32 sx, f32 sy, f32 rot)
	{
		// if newObjis not NULL, set the inputted values, else return
		if (newObj)
		{
			// Set everything to true
			newObj->mbActive = true;
			newObj->mbAlive = true;
			newObj->mbVisible = true;

			// Set the values as the inputted ones
			newObj->SetName(name);

			// @TODO
			/*dynamic_cast<TransformComp*>(newObj->GetComp("TransformComp"))->mLocal.mTranslation = AEVec2(x, y);
			dynamic_cast<TransformComp*>(newObj->GetComp("TransformComp"))->mLocal.mScale = AEVec2(sx, sy);
			dynamic_cast<TransformComp*>(newObj->GetComp("TransformComp"))->mLocal.mOrientation = rot;*/

			if (inEditor)
				mEditorObjects.push_back(newObj);
			else
				mAliveObjects.push_back(newObj);

			// Initialize the GameObject
			newObj->Initialize();

			return newObj;
		}
		else
			return NULL;
	}

	/**************************************************************************
	*!
	\fn
	DestroyObject

	\brief
	Prepares an object for destruction, setting it to .

	\param
	GameObject * pObj : A pointer to the object we want to destroy.
	*
	***************************************************************************/
	void			ObjectManager::DestroyObject(GameObject * pObj)
	{
		pObj->mbAlive = false;
		mDestroyedObjects.push_back(pObj);
	}

	/**************************************************************************
	*!
	\fn
	FreeAllObjects

	\brief
	Destroys all objects right away.
	*
	***************************************************************************/
	void			ObjectManager::FreeAllObjects()
	{
		// Destroy all the GameObject from mDestroyedObjects, just in case
		FreeDestroyedObjects();

		// While the mAliveObjects list is not empty, keep deleting and freeing the alive GameObjects
		while (!mAliveObjects.empty())
		{
			delete mAliveObjects.back();
			mAliveObjects.pop_back();
		}
	}

	/**************************************************************************
	*!
	\fn
	FindObject

	\brief
	Search the alive list for the first object whose id matches the one passed
	as an argument.

	\param
	u32 id : The id of the GameObject we want to find.

	\return
	The address of the GameObject with the same id. returns NULL if
	the object is not found.
	*
	***************************************************************************/
	GameObject *	ObjectManager::FindObject(u32 id)
	{
		OBJECT_PTR_LIST::iterator it;

		for (it = mAliveObjects.begin(); it != mAliveObjects.end(); it++)
		{
			// If the GameObject's id is the same as the one inputted, return it
			if ((*it)->GetID() == id)
				return *it;
		}

		return NULL; // not found
	}

	/**************************************************************************
	*!
	\fn
	FindObject

	\brief
	Search the alive list for the first object whose name matches the string
	passed as an argument.

	\param
	const char* name : The name of the GameObject we want to find.

	\return
	The address of the GameObject with the same name. returns NULL if
	the object is not found.
	*
	***************************************************************************/
	GameObject *	ObjectManager::FindObject(const char* name)
	{
		// Create an iterator to access each member of the mAliveObjects list
		OBJECT_PTR_LIST::iterator it;

		for (it = mAliveObjects.begin(); it != mAliveObjects.end(); it++)
		{
			// If the GameObject's name is the same as the one inputted, return it
			if ((*it)->GetName() == name)
				return *it;
		}
		return NULL; // not found
	}

	/**************************************************************************
	*!
	\fn
	FindObject

	\brief
	Search the alive list for all the object whose name matches the string
	passed as an argument and add them to the list passed as an argument.

	\param
	const char* name : The name of the GameObject we want to find.

	\return
	The address of the GameObject with the same name. returns NULL if
	the object is not found.
	*
	***************************************************************************/
	void			ObjectManager::FindObject(const char* name, OBJECT_PTR_LIST & objects)
	{
		// If the GameObject's name is the same as the one inputted, push it to an object list
		if (GameObject * pObj = FindObject(name))
		{
			objects.push_back(pObj);
		}
	}

	/**************************************************************************
	*!
	\fn
	GetAliveObjects

	\brief
	Returns a const reference to the alive list.
	*
	***************************************************************************/
	const std::list<GameObject*> & 	ObjectManager::GetAliveObjects()
	{
		return mAliveObjects;
	}

	json & ObjectManager::operator<<(json & j) const
	{
		json & array_val = j["gameObject_list"];

		for (auto it = mEditorObjects.begin(); it != mEditorObjects.end(); it++)
		{
			array_val << *(*it);
		}

		return array_val;
	}

	void ObjectManager::operator >> (json & j)
	{
		FreeAllObjects();

		aexFactory->Register<GameObject>();
		const json & array_val = j["gameObject_list"];
		
		// Iterate over sequence elements
		for (unsigned int index = 0; index < array_val.size(); ++index)
		{
			GameObject* currentGO = dynamic_cast<GameObject*>(aexFactory->Create(GameObject::TYPE().GetName()));
			j >> *currentGO;
		}
	}

	std::ostream & ObjectManager::operator<<(std::ostream & o) const
	{
		json j;
		this->operator<<(j);
		return o << j;
	}

	/**************************************************************************
	*!
	\fn
	OnGui

	\brief
	Used to edits all editor GOs on Gui.
	*
	***************************************************************************/
	void ObjectManager::OnGui()
	{
		static int selected = -1;
		int n = 0;
		for (auto it = mEditorObjects.begin(); it != mEditorObjects.end(); it++, n++)
		{
			char buf[32];
			sprintf(buf, (*it)->GetName(), n);
			if (ImGui::Selectable(buf, selected == n))
			{
				imGui_selectedGO = *it;
				selected = n;
			}
		}

		if (imGui_selectedGO)
			imGui_selectedGO->OnGui();
	}

	/**************************************************************************
	*!
	\fn
	RemoveObjectFromAliveList

	\brief
	Searches the alive list for the object and erases it from the list if found.
	*
	***************************************************************************/
	void ObjectManager::RemoveObjectFromAliveList(GameObject * pObj)
	{
		mAliveObjects.remove(pObj);
	}

	/**************************************************************************
	*!
	\fn
	FreeDestroyedObjects

	\brief
	For each object in the destroyed list, remove from the alive list and
	delete the object (free the memory)
	*
	***************************************************************************/
	void ObjectManager::FreeDestroyedObjects()
	{
		// Create an iterator to access each member of the mDestroyedObjects list
		OBJECT_PTR_LIST::iterator it;

		while (!mDestroyedObjects.empty())
		{
			//remove it from the mAliveObjects list
			RemoveObjectFromAliveList(mDestroyedObjects.back());
			delete mDestroyedObjects.back();
			mDestroyedObjects.pop_back();
		}
	}
}
