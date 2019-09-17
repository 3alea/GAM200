#pragma once
#include "../Composition/AEXComponent.h"

namespace AEX
{
	struct RenderSettings : public IComp
	{
		bool		EditorDebugDraw;
		

		void Initialize();		// Called when the owner object is finishhed being assembled.
		void Update();			// Called by the system at each update.
		void Shutdown();
	};
}