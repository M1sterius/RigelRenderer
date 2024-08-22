#include "Shader.hpp"
#include "glad.h"
#include "gtc/type_ptr.hpp"
#include "glm.hpp"
#include "Texture.hpp"
#include "Core.hpp"

#include <fstream>
#include <iostream>
#include <string>

namespace rgr
{
	// Returns 1 if the shader was compiled with an error and prints this error to the console, returns 0 if the shader was compiled successfully
	static int GetShaderCompileInfo(const unsigned int shaderHandle)
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

	// Processes glsl source code in the file at the given path, so it can be used as OpenGL shader source
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
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_Handle);
	}

	rgr::Shader Shader::FromFiles(const std::string& vertexPath, const std::string& fragmentPath)
	{
		const std::string processedVertexSource = ProcessShaderSource(vertexPath);
		const std::string processedFragmentSource = ProcessShaderSource(fragmentPath);

		if (processedVertexSource.empty())
		{
			std::cout << "The file at path: '" << vertexPath << "' does not exist!" << '\n';
			//return GetBuiltInShader(BUILT_IN_SHADERS::PLAIN_COLOR);
		}
		if (processedFragmentSource.empty())
		{
			std::cout << "The file at path: '" << fragmentPath << "' does not exist!" << '\n';
			//return GetBuiltInShader(BUILT_IN_SHADERS::PLAIN_COLOR);
		}

		return {processedVertexSource, processedFragmentSource};
	}

	rgr::Shader Shader::FromSources(const std::string& vertexSource, const std::string& fragmentSource)
	{
		return { vertexSource, fragmentSource };
	}

    const Shader& Shader::GetBuiltInShader(const Shader::BUILT_IN_SHADERS type)
    {
        static const std::string builtInShadersPath = rgr::Core::GetBuiltInResourcesPath() + "/shaders";

        // Instantiation of plain color shader may create infinite recursive call chain of GetBuiltInShader() and FromFiles(), gotta fix that!!
        static auto plainColor = Shader::FromFiles(builtInShadersPath + "/plain_color_vertex.glsl",
                                                         builtInShadersPath + "/plain_color_fragment.glsl");
        static auto depthMap = Shader::FromFiles(builtInShadersPath + "/shadow_mapping/depth_map_vertex.glsl",
                                                       builtInShadersPath + "/shadow_mapping/depth_map_fragment.glsl");
        static auto geometryPass = Shader::FromFiles(builtInShadersPath + "/deferred_rendering/geometry_pass_vertex.glsl",
                                                           builtInShadersPath + "/deferred_rendering/geometry_pass_fragment.glsl");
        static auto stencilPass = Shader::FromFiles(builtInShadersPath + "/deferred_rendering/stencil_pass_vertex.glsl",
                                                    builtInShadersPath + "/deferred_rendering/stencil_pass_fragment.glsl");
        static auto depthVisualization = Shader::FromFiles(builtInShadersPath + "/shadow_mapping/depth_visualization_vertex.glsl",
                                                          builtInShadersPath + "/shadow_mapping/depth_visualization_fragment.glsl");
        static auto fullscreenTexture = Shader::FromFiles(builtInShadersPath + "/fullscreen_texture_vertex.glsl",
                                                          builtInShadersPath + "/fullscreen_texture_fragment.glsl");

        static auto dirLightNoShadows = Shader::FromFiles(builtInShadersPath + "/deferred_rendering/directional_light/dir_light_vertex.glsl",
                                                          builtInShadersPath + "/deferred_rendering/directional_light/dir_light_no_shadows_fragment.glsl");
        static auto dirLightShadowsNoPcf = Shader::FromFiles(builtInShadersPath + "/deferred_rendering/directional_light/dir_light_vertex.glsl",
                                                          builtInShadersPath + "/deferred_rendering/directional_light/dir_light_shadows_no_pcf_fragment.glsl");
        static auto dirLightShadowsPcf = Shader::FromFiles(builtInShadersPath + "/deferred_rendering/directional_light/dir_light_vertex.glsl",
                                                             builtInShadersPath + "/deferred_rendering/directional_light/dir_light_shadows_pcf_fragment.glsl");

        static auto pointLightNoShadows = Shader::FromFiles(builtInShadersPath + "/deferred_rendering/point_light/point_light_vertex.glsl",
                                                            builtInShadersPath + "/deferred_rendering/point_light/point_light_no_shadows_fragment.glsl");

        switch (type)
        {
            case BUILT_IN_SHADERS::PLAIN_COLOR:
                return plainColor;
            case BUILT_IN_SHADERS::DEPTH_MAP:
                return depthMap;
            case BUILT_IN_SHADERS::GEOMETRY_PASS:
                return geometryPass;
            case BUILT_IN_SHADERS::STENCIL_PASS:
                return stencilPass;
            case BUILT_IN_SHADERS::DEPTH_VISUALIZATION:
                return depthVisualization;
            case BUILT_IN_SHADERS::FULLSCREEN_TEXTURE:
                return fullscreenTexture;

            case BUILT_IN_SHADERS::DIR_LIGHT_NO_SHADOWS:
                return dirLightNoShadows;
            case BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_NO_PCF:
                return dirLightShadowsNoPcf;
            case BUILT_IN_SHADERS::DIR_LIGHT_SHADOWS_PCF:
                return dirLightShadowsPcf;

            case BUILT_IN_SHADERS::POINT_LIGHT_NO_SHADOWS:
                return pointLightNoShadows;
        }
    }

	void Shader::Bind() const
	{
		glUseProgram(m_Handle);
	}

	void Shader::Unbind() const
	{
		glUseProgram(0);
	}

	void Shader::BindTexture(const std::string& name, const rgr::Texture* texture, const int slot) const
	{
        texture->BindToSlot(slot);
		SetUniform1is(name, slot);
	}

    void Shader::BindTexture(const std::string &name, const std::shared_ptr<Texture>& texture, const int slot) const
    {
        texture->BindToSlot(slot);
        SetUniform1is(name, slot);
    }

	void Shader::SetUniform1is(const std::string& name, const int value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1i(location, value);
	}

	void Shader::SetUniform1i(const std::string& name, const unsigned int value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1ui(location, value);
	}

	void Shader::SetUniform1i(const std::string& name, const size_t value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1ui(location, value);
	}

	void Shader::SetUniform1f(const std::string& name, const float value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform1f(location, value);
	}

    void Shader::SetUniformBool(const std::string &name, const bool value) const
    {
        const int location = FindUniform(name);
        if (location == -1) return;
        glUniform1i(location, value);
    }

	void Shader::SetUniformVec2(const std::string& name, const glm::vec2& value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform2f(location, value.x, value.y);
	}

	void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::SetUniformVec4(const std::string& name, const glm::vec4& value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void Shader::SetUniformMat3(const std::string& name, const bool transpose, const glm::mat3& value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(value));
	}

	void Shader::SetUniformMat4(const std::string& name, const bool transpose, const glm::mat4& value) const
	{
		const int location = FindUniform(name);
		if (location == -1) return;
		glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(value));
	}

	int Shader::FindUniform(const std::string& name) const
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
		return -1;
	}
}
