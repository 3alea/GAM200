#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./extern/STB/stb_image.h"
#include <./extern/glad/glad.h>
#include <./extern/glfw/glfw3.h>

#include <iostream>

Texture::Texture(const char * imagePath, TexWrappingMethod texWrapMethod, TexFilterMethod texFiltMethod) : eWrapMethod(texWrapMethod), eFilterMethod(texFiltMethod)
{
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0); // Load image
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); // Create texture
		glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps
	}
	else
		std::cout << "Failed to load texture" << std::endl;

	stbi_image_free(data);
}

void Texture::SetBorderColor(f32 * borderColor)
{
	for (u32 i = 0; i < 4; i++)
		mBorderColor[i] = borderColor[i];

	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, mBorderColor);
}

void Texture::CheckTextureMethods()
{
	// set the texture wrapping parameters
	switch (eWrapMethod)
	{
	case eRepeat:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;

	case eRepeat_Mirror:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;

	case eClamp:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;

	case eBorderCol:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;
	}

	// set texture filtering parameters
	switch (eFilterMethod)
	{
	case eLinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case eNearest:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	}

	// Magnifying a texture doesn't alter the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
