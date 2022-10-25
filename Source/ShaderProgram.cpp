#include "ShaderProgram.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& vertexName, const std::string& fragmentName)
{
	// Retrieve the vertex/fragment source code
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Ensures ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	std::string vertexPath = "Source/Shaders/" + vertexName;
	std::string fragmentPath = "Source/Shaders/" + fragmentName;

	try
	{
		// Open the files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		std::stringstream vShaderStream;
		std::stringstream fShaderStream;

		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// Conver stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure& e)
	{
		throw std::runtime_error(e);
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. Compile shaders
	unsigned int vertexID, fragmentID;
	int success;
	char infoLog[512];

	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vShaderCode, NULL);
	glCompileShader(vertexID);

	// Checker for errors
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexID, 512, NULL, infoLog);
		std::cout << "Error: shader vertex compilation failed: " << infoLog << std::endl;
		throw std::runtime_error("Error: Vertex shader compilation FAILED");
	}

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fShaderCode, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentID, 512, NULL, infoLog);
		std::cout << "Error: shader fragment compilation failed: " << infoLog << std::endl;
		throw std::runtime_error("Error: Fragment shader compilation FAILED");
	}

	shaderProgramID = glCreateProgram();
	glAttachShader(shaderProgramID, vertexID);
	glAttachShader(shaderProgramID, fragmentID);
	glLinkProgram(shaderProgramID);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "Error: Shader program linking failed: " << infoLog << std::endl;
	}

	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(shaderProgramID);
}

void ShaderProgram::Bind()
{
	glUseProgram(shaderProgramID);
}

void ShaderProgram::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}


void ShaderProgram::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y);
}

void ShaderProgram::SetVec2(const std::string& name, glm::vec2& vec) const
{
	glUniform2f(glGetUniformLocation(shaderProgramID, name.c_str()), vec.x, vec.y);
}

void ShaderProgram::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), x, y, z);
}

void ShaderProgram::SetVec3(const std::string& name, glm::vec3& vec) const
{
	glUniform3f(glGetUniformLocation(shaderProgramID, name.c_str()), vec.x, vec.y, vec.z);
}

void ShaderProgram::SetMat4(const std::string& name, glm::mat4& mat4) const
{
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
}