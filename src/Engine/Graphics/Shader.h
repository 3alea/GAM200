#pragma once
#include "../Core/AEXBase.h"
#include <./extern/glad/glad.h>
#include <./extern/glfw/glfw3.h>
#include <./extern/aexmath/aexmath/AEXMath.h>

using namespace AEX;

class Shader : public IBase
{
	AEX_RTTI_DECL(Shader, IBase);
public:
    unsigned int ID; // Shader program ID
  
    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
	void setFloat(const std::string &name, float value) const;
	// ------------------------------------------------------------------------
	void setVec2(const std::string &name, const AEVec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	// ------------------------------------------------------------------------
	void setVec3(const std::string &name, const AEVec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	// ------------------------------------------------------------------------
	void setVec4(const std::string &name, const AEVec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	// ------------------------------------------------------------------------
	void setMat3(const std::string &name, const AEMtx33 &mat) const;
	// ------------------------------------------------------------------------
	void setMat4(const std::string &name, const AEMtx44 &mat) const;
};