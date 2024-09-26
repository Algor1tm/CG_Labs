#include "Shader.h"

#include <glad/glad.h>
#include <fstream>
#include <array>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	: m_IsCompiled(false)
{
	std::string shaderName = vertexPath.stem().string();

	std::unordered_map<GLenum, std::string> shaderSources;
	shaderSources[GL_VERTEX_SHADER] = ReadFileContent(vertexPath);
	shaderSources[GL_FRAGMENT_SHADER] = ReadFileContent(fragmentPath);

	std::array<GLenum, 2> shaderIDs = { (uint32_t)-1, (uint32_t)-1};
	int shaderIDIndex = 0;

	GLuint program = glCreateProgram();

	for (const auto& [glType, sourceString] : shaderSources)
	{
		GLuint shader = glCreateShader(glType);

		const char* source = sourceString.c_str();

		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

			for (auto id : shaderIDs)
			{
				if (id != (uint32_t)-1)
					glDeleteShader(id);
			}

			glDeleteProgram(program);
			LOG_ERROR("Shader {} compilation failed!\nErrors/Warnings:\n{}", shaderIDIndex == 0 ? vertexPath.string() : fragmentPath.string(), (char*)infoLog.data());

			return;
		}

		glAttachShader(program, shader);
		shaderIDs[shaderIDIndex++] = shader;
	}

	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

		for (auto id : shaderIDs)
			glDeleteShader(id);

		glDeleteProgram(program);
		LOG_ERROR("Shader {} linking failed!\nErrors/Warnings:\n{}", vertexPath.stem().string(), (char*)infoLog.data());

		return;
	}

	for (auto id : shaderIDs)
		glDetachShader(program, id);

	LOG_INFO("Successfully compile shader '{}'", vertexPath.stem().string());

	m_RendererID = program;
	m_IsCompiled = true;
}

Shader::~Shader()
{
	if (m_IsCompiled)
		glDeleteProgram(m_RendererID);
}

void Shader::Bind()
{
	if (m_IsCompiled)
		glUseProgram(m_RendererID);
	else
		glUseProgram(0);
}

void Shader::SetVector4(const std::string& name, glm::vec4 vec4)
{
	int location = GetUniformLocation(name);
	glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat4)
{
	int location = GetUniformLocation(name);
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (!m_IsCompiled)
		return -1;

	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1)
		LOG_WARN("Uniform name: '{0}' does not exist!", name.c_str());

	m_UniformLocationCache[name] = location;

	return location;
}

std::string Shader::ReadFileContent(const std::filesystem::path& path)
{
	std::string result;
	std::ifstream in(path, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(result.data(), result.size());
		in.close();
	}
	else
	{
		LOG_ERROR("FileSystem could not open file: '{0}'", path.string());
	}

	return result;
}
