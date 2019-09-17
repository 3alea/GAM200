// ---------------------------------------------------------------------------
// Project Name		:	Alpha Engine
// File Name		:	ObjectManager.h
// Author			:	Thomas Komair
// Creation Date	:	2015/02/04
// Purpose			:	Header for Object Manager - This is the first implementation 
//						for the ObjectManager
// ---------------------------------------------------------------------------
#ifndef GAME_OBJECT_MGR_H_
#define GAME_OBJECT_MGR_H_
// ---------------------------------------------------------------------------
#include "AEXGameObject.h"

#define MAX_GAME_OBJECT_NUM  1024

namespace AEX
{
	class ObjectManager : public ISystem
	{
		AEX_RTTI_DECL(ObjectManager, ISystem);
		AEX_SINGLETON(ObjectManager);

	public:
		bool			Initialize();	// Initializes the game object manager
		void			Render();
		void			Shutdown();		// destroy all the object and prints an error if there are alive objects

		GameObject*		CreateGO(const char * name);
		GameObject*		CreateGO(const char * name, f32 x, f32 y, f32 sx, f32 sy, f32 rot);

		// Adding an object - we will assume that creates a new empty object
		GameObject *	AddObject(GameObject * newObj, const char * name); 
		GameObject *	AddObject(GameObject * newObj, const char * name, f32 x, f32 y, f32 sx, f32 sy, f32 rot);

		// Destroy Objects
		void			DestroyObject(GameObject * pObj);	// places an object onto the destroyed list.
		void			FreeAllObjects();					// deletes all the objects right away. 

															// Get Object
		GameObject *	FindObject(u32 id);
		GameObject *	FindObject(const char* name);
		void			FindObject(const char* name, OBJECT_PTR_LIST & objects);

		u32 GetObjectID() { return mIdGenerator; }

		// returns a const reference to the alive list (cannot modify)
		const OBJECT_PTR_LIST & 	GetAliveObjects();

		virtual json& operator<< (json&j) const;
		virtual void operator >> (json&j);
		virtual std::ostream& operator << (std::ostream & o) const;

		void OnGui();

		GameObject * imGui_selectedGO = NULL;

	private:
		void FreeDestroyedObjects();
		void RemoveObjectFromAliveList(GameObject * pObj);

		// In Editor
		OBJECT_PTR_LIST mEditorObjects;

		// In Game
		u32				mIdGenerator;
		OBJECT_PTR_LIST mAliveObjects;
		OBJECT_PTR_LIST mDestroyedObjects;
	};

	// Global and unique object manager. No other object manager should be created. 
	#define  ObjMgr ObjectManager::Instance()

	// ---------------------------------------------------------------------------
}



#endif