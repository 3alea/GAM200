#pragma once
#include "../Core/AEXBase.h"
#include "../Core/AEXSystem.h"

namespace AEX
{
	struct ICreator {
		virtual IBase * Create() = 0; // pure virtual 
	};

	template<typename T>
	struct TCreator : public ICreator {
		virtual IBase * Create()
		{
			return new T();
		}
	};

	class Factory : public ISystem
	{
		AEX_RTTI_DECL(Factory, ISystem);
		AEX_SINGLETON(Factory);
	private:
		void Register(const char * typeName, ICreator * creator)
		{
			// Conditional may be redundant (Check templated function)
			if (mCreators.find(typeName) == mCreators.end()) // no duplicate
				mCreators[typeName] = creator;
		}

		std::map<std::string, ICreator*> mCreators;
	public:

		IBase * Create(const char * typeName)
		{
			// IMPORTANT: FIND THE CREATOR HERE
			if (mCreators.find(typeName) != mCreators.end())
				return mCreators[typeName]->Create();
			// NO CREATOR REGISTERED
			return NULL;
		}
		ICreator * FindCreator(const char * typeName) { return mCreators[typeName]; }

		template <typename T> void Register() {
			if (mCreators.find(T::TYPE().GetName()) == mCreators.end())
				Register(T::TYPE().GetName(), new TCreator<T>());
			// @CHECK: handle duplicate creator (avoid memory leaks)
		}
		template <typename T> T* Create() {
			return dynamic_cast<T*>(Create(T::TYPE().GetName()));
		}
	};
	
#define aexFactory (Factory::Instance())
}