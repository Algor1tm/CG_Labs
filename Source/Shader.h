#pragma once

#include "Core.h"

#include <filesystem>
#include <unordered_map>
#include <glm/glm.hpp>


class Shader
{
public:
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	~Shader();

	void Bind();
	bool IsCompiled() const { return m_IsCompiled; }

	void SetVector4(const std::string& name, glm::vec4 vec4);
	void SetMat4(const std::string& name, const glm::mat4& mat4);

private:
	int GetUniformLocation(const std::string& name);
	std::string ReadFileContent(const std::filesystem::path& path);

private:
	uint32_t m_RendererID = 0;
	bool m_IsCompiled;
	std::unordered_map<std::string, int> m_UniformLocationCache;
};
