#include "Shader.h"

#include <iostream>
#include <fstream>

void Shader::AddSourceAndCompile(const std::string& path, GLenum type)
{
    auto source = ReadFile(path);
    CompileShader(source, type);
}

void Shader::Link()
{
    GLuint program = glCreateProgram();

    for (auto shader : shaders)
        glAttachShader(program, shader);

    glLinkProgram(program);
    int isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint size;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
        std::vector<GLchar> infoLog(size);
        glGetProgramInfoLog(program, size, &size, infoLog.data());

        std::cout << "Failed to link program! Error:\n" << infoLog.data() << std::endl;

        glDeleteProgram(program);
        return;
    }

    for (auto shader : shaders)
    {
        glDetachShader(program, shader);
        glDeleteShader(shader);
    }

    m_Handle = program;
}

void Shader::Use()
{
    glUseProgram(m_Handle);
}

void Shader::SetInt(const std::string& name, int value)
{
    int location = glGetUniformLocation(m_Handle, name.c_str());
    glUniform1i(location, value);
}

void Shader::SetFloat(const std::string& name, float value)
{
    int location = glGetUniformLocation(m_Handle, name.c_str());
    glUniform1f(location, value);
}

std::string Shader::ReadFile(const std::string& path)
{
    std::ifstream file(path, std::ios::in || std::ios::binary);
    std::string result;
    if (file.is_open())
    {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        result.resize(size);
        file.seekg(0, std::ios::beg);
        file.read(&result[0], size);
    }
    file.close();
    return result;
}

void Shader::CompileShader(const std::string& source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const GLchar* sourceCStr = source.c_str();
    glShaderSource(shader, 1, &sourceCStr, 0);

    glCompileShader(shader);
    int isCompiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        std::vector<GLchar> infoLog(size);
        glGetShaderInfoLog(shader, size, &size, infoLog.data());

        std::cout << "Failed to compile shader! Error:\n" << infoLog.data() << std::endl;

        glDeleteShader(shader);
        return;
    }

    shaders.push_back(shader);
}
