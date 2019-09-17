#pragma once
//#include "../Composition/AEXSerialization.h"
#include "..\Core\AEXBase.h"

namespace AEX
{
	// Game State
	class IGameState : public IBase//, public ISerializable
	{
		AEX_RTTI_DECL(IGameState, IBase);

	public:
		// State inherited
		virtual void Update(){}
		virtual void Initialize(){}
		virtual void Shutdown(){}
		virtual ~IGameState(){}

		// Helper functions
		virtual void LoadResources(){}
		virtual void FreeResources(){}

		/*json& operator<< (json&j) const;
		void operator >> (json&j);
		std::ostream& operator << (std::ostream & o) const;*/

		// Custom Render
		virtual void Render(){}

		//std::list<IGameState*> mGameStateList;
		IGameState* editor_currentGS;
	};
}
