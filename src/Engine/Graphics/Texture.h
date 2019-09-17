#pragma once
#include "../Core/AEXBase.h"

using namespace AEX;

enum TexWrappingMethod
{
	eRepeat,		// Repeats the texture image
	eRepeat_Mirror, // Same as 'eRepeat' but mirrors the image with each repeat
	eClamp,			// Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a straight edge pattern
	eBorderCol		// Coordinates outside the range are now given a user-specified border color.
};

enum TexFilterMethod
{
	eLinear,
	eNearest
};

class Texture : public IBase
{
	AEX_RTTI_DECL(Texture, IBase); 

	Texture(const char * imagePath, TexWrappingMethod texWrapMethod = eRepeat, TexFilterMethod texFiltMethod = eLinear);

public:
	unsigned int GetID() { return mID; }
	TexWrappingMethod GetWrapMethod() { return eWrapMethod; }
	void SetWrapMethod(TexWrappingMethod method) { eWrapMethod = method; }
	TexFilterMethod GetFilterMethod() { return eFilterMethod; }
	void SetFilterMethod(TexFilterMethod method) { eFilterMethod = method; }

	void SetBorderColor(f32* borderColor);

private:
	void CheckTextureMethods();   // Used in constructors, should not be used elsewhere

private:
	unsigned int		mID;
	TexWrappingMethod	eWrapMethod;
	TexFilterMethod		eFilterMethod;
	f32					mBorderColor[4]; // Only used when eWrapMethod == eBorderCol
};