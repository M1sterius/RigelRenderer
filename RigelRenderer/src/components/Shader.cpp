#include "Shader.hpp"
#include "glew.h"
#include "gtc/type_ptr.hpp"
#include "glm.hpp"
#include "BuiltInShaders.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace rgr
{
	// Returns 1 if the shader was compiled with an error and prints this error to the console, returns 0 if the shader was compiled successfully
	int GetShaderCompileInfo(const unsigned int shaderHandle)
	{
		int res;

		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &res);
		if (res != GL_FALSE)
		{
			return 0;
		}

		int length;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
		char* message = static_cast<char*>(alloca(length * sizeof(char)));
		glGetShaderInfoLog(shaderHandle, length, &length, message);

		std::cout << "Shader compilation error: " << message << '\n';

		return 1;
	}
	// Processes glsl source code in the file at the given path so it can be used as OpenGL shader source
	std::string ProcessShaderSource(const std::string& sourcePath)
	{
		std::ifstream stream(sourcePath);
		std::string line;
		std::string temp;

		while (std::getline(stream, line))
		{
			temp += (line + '\n');
		}

		stream.close();

		return temp;
	}

	Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
	{	
		const GLchar* vs = vertexSource.c_str();
		const GLchar* fs = fragmentSource.c_str();

		const unsigned int vertexHandle = glCreateShader(GL_VERTEX_SHADER);
		const unsigned int fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertexHandle, 1, &vs, nullptr);
		glShaderSource(fragmentHandle, 1, &fs, nullptr);

		glCompileShader(vertexHandle);
		glCompileShader(fragmentHandle);

		if (GetShaderCompileInfo(vertexHandle) == 1 || GetShaderCompileInfo(fragmentHandle) == 1)
		{
			glDeleteShader(vertexHandle);
			glDeleteShader(fragmentHandle);

			m_ShaderHasError = true;
			return;
		}

		m_ShaderHasError = false;
		m_Handle = glCreateProgram();

		glAttachShader(m_Handle, vertexHandle);
		glAttachShader(m_Handle, fragmentHandle);

		glLinkProgram(m_Handle);

		glValidateProgram(m_Handle);

		glDeleteShader(vertexHandle);
		glDeleteShader(fragmentHandle);

		//int param;
		//glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &param);
		//std::cout << param << '\n';

		//GLchar uniform_name[256];
		//GLsizei length;
		//GLint size;
		//GLenum type;
		//glGetActiveUniform(m_Handle, 0, sizeof(uniform_name), &length, &size, &type, uniform_name);
		//std::cout << uniform_name << '\n';
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Handle);
	}

	rgr::Shader* Shader::FromFiles(const std::string& vertexPath, const std::string& fragmentPath)
	{
		const std::string processedVertexSource = ProcessShaderSource(vertexPath);
		const std::string processedFragmentSource = ProcessShaderSource(fragmentPath);

		if (processedVertexSource.empty())
		{
			std::cout << "The file at path: '" << vertexPath << "' does not exist!" << '\n';
			return GetPlainColorShader();
		}
		if (processedFragmentSource.empty())
		{
			std::cout << "The file at path: '" << fragmentPath << "' does not exist!" << '\n';
			return GetPlainColorShader();
		}

		return new rgr::Shader(processedVertexSource, processedFragmentSource);
	}

	rgr::Shader* Shader::FromSources(const std::string& vertexSource, const std::string& fragmentSource)
	{
		return new rgr::Shader(vertexSource, fragmentSource);
	}

	Shader* Shader::GetPlainColorShader()
	{
		static Shader* shader = new Shader(rgr::PlainColorVertex, rgr::PlainColorFragment);
		return shader;
	}
	
	void Shader::Bind() const
	{
		glUseProgram(m_Handle);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::SetUniform1i(const std::string& name, const int value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1i(location, value);
	}

	void Shader::SetUniform1i(const std::string& name, const unsigned int value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1ui(location, value);
	}

	void Shader::SetUniform1f(const std::string& name, const float value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1f(location, value);
	}

	void Shader::SetUniformVec2(const std::string& name, const glm::vec2& value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetUniformVec4(const std::string& name, const glm::vec4& value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
	}

	void Shader::SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value)
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
	}

	int Shader::FindUniform(const std::string& name)
	{
		if (m_UniformsLocationCache.find(name) != m_UniformsLocationCache.end())
			return m_UniformsLocationCache[name];

		const int location = glGetUniformLocation(m_Handle, name.c_str());
		if (location != -1)
		{
			m_UniformsLocationCache[name] = location;
			return location;
		}
		std::cout << "Unable to find uniform named: " << name << '\n';
		m_UnifromsCallback++;
		return -1;
	}

	int Shader::GetUniformsCallback()
	{
		int callback = m_UnifromsCallback;
		m_UnifromsCallback = 0;
		return callback;
	}
}
