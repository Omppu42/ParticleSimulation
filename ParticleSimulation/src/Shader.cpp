#include "Shader.h"

Shader::Shader(std::string pathToSource) {

    ParseShader(pathToSource);
    CreateShader();
    UseProgram();
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_ProgramID));
}

void Shader::UseProgram() const
{
    GLCall(glUseProgram(m_ProgramID));
}

void Shader::DisableProgram() const
{
    GLCall(glUseProgram(0));
}

void Shader::ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss[2];

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;

    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << "\n";
        }
    }
    //std::cout << "VERTEX \n" << ss[0].str() << "FRAGMENT \n " << ss[1].str() << std::endl;
    m_Source = { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = &source[0];
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); //shader error handling

    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

        char* message = (char*)_malloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));

        std::cout << "FAILED TO COMPILE " << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << " SHADER" << std::endl;
        std::cout << message << std::endl;

        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

void Shader::CreateShader() {
    GLCall(m_ProgramID = glCreateProgram());

    unsigned int vs = CompileShader(GL_VERTEX_SHADER, m_Source.VertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, m_Source.FragmentSource);

    GLCall(glAttachShader(m_ProgramID, vs));
    GLCall(glAttachShader(m_ProgramID, fs));
    GLCall(glLinkProgram(m_ProgramID));
    GLCall(glValidateProgram(m_ProgramID));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

}