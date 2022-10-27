#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>

class Shader
{
public:
	Shader() = default;
	virtual ~Shader() = default;

	void AddSourceAndCompile(const std::string& path, GLenum type);

	void Link();

	void Use();

	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);

private:
	std::string ReadFile(const std::string& path);
	void CompileShader(const std::string& source, GLenum type);

private:
	GLuint m_Handle = 0;
	std::vector<GLuint> shaders;
};

