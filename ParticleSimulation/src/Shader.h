#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include "GLUtils.h"


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader {
private:
	unsigned int m_ProgramID;
	ShaderProgramSource m_Source;

    unsigned int CompileShader(unsigned int type, const std::string& source);
	
    void ParseShader(const std::string& filepath);
	void CreateShader();

public:
	Shader(std::string pathToSource);
	~Shader();

	void UseProgram() const;
	void DisableProgram() const;
};