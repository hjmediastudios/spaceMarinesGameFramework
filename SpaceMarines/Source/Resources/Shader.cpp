#include "Shader.hpp"
#include <GL/glew.h>
#include <GL/gl.h>
using namespace std;

namespace SpaceMarines
{

Shader::Shader()
{
	inited = false;
}

void Shader::validateProgram(GLuint program)
{
	const unsigned int BUFFERSIZE = 512;
	char buffer[BUFFERSIZE];
	memset(buffer, 0, BUFFERSIZE);
	int length = 0;
	glGetProgramInfoLog(program, BUFFERSIZE, &length, buffer);
	if (length > 0)
	{
		throw Exception("Link error in shader " + program);
		exit(-1);
	}

	glValidateProgram(program);
	int status;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		throw Exception("Error validating program " + program);
		exit(-1);
	}
}

void Shader::validateShader(GLuint shader, const char* shaderSourceText, const char* filename)
{
	const unsigned int BUFFERSIZE = 512;
	char buffer[BUFFERSIZE];
	memset(buffer, 0, BUFFERSIZE);
	int length = 0;
	glGetShaderInfoLog(shader, BUFFERSIZE, &length, buffer);
	if (length > 0)
	{
		std::cerr << "Shader " << shader << " ("
				<< filename << ") compile error: " << buffer << std::endl << shaderSourceText;
		throw Exception("Error validating shader:");
	}
}

Shader* Shader::newShaderFromTwoFiles(const char* vsFilename,
		const char *fsFilename)
{
	string vsPath = vsFilename;
	string fsPath = fsFilename;

	Shader *newShader = new Shader();
	cout << "Loading vsSource \"" << vsPath << "\"\n";
	string vsSource = Math::textFileRead(vsPath);
	cout << "Loading fsSource \"" << fsPath << "\"\n";
	string fsSource = Math::textFileRead(fsPath);
	newShader->init(vsSource.c_str(), fsSource.c_str(), vsFilename, fsFilename);
	return newShader;
}

void Shader::bindUniformBlock(const char *uboNameInShader, GLuint uboBufferIndex)
{
	glUniformBlockBinding(shaderID, getUniformBlockLocation(uboNameInShader), uboBufferIndex);
}


void Shader::init(const char *vsSource, const char *fsSource, const char* vsFilename, const char* fsFilename)
{
	if (inited)
		return;

	inited = true;

	shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

	if (vsSource == NULL || fsSource == NULL)
	{
		throw Exception("Either a vertex shader or a fragment shader couldn't be found.");
		return;
	}

	glShaderSource(shader_vp, 1, &vsSource, 0);
	glCompileShader(shader_vp);
	validateShader(shader_vp, vsSource, vsFilename);

	glShaderSource(shader_fp, 1, &fsSource, 0);
	glCompileShader(shader_fp);
	validateShader(shader_fp, fsSource, fsFilename);

	shaderID = glCreateProgram();
	glAttachShader(shaderID, shader_vp);
	glAttachShader(shaderID, shader_fp);

	//TODO this can change
	glBindAttribLocation(shaderID, 0, "in_position"); //vertex position
	glBindAttribLocation(shaderID, 1, "in_color"); //vertex color

	glLinkProgram(shaderID);
	validateProgram(shaderID);
}

GLuint Shader::getShaderID() const
{
	return shaderID;
}

GLuint Shader::getUniformLocation(const char* uniformName) const
{
	GLuint returnVar = glGetUniformLocation(shaderID, uniformName);
	if (INVALID_UNIFORM_LOCATION == returnVar)
	{
		throw Exception(string("Error getting uniform ") + uniformName);
	}
}

GLuint Shader::getUniformBlockLocation(const char* uniformName) const
{
	return glGetUniformBlockIndex(shaderID, uniformName);
}

void Shader::enable()
{
	glUseProgram(shaderID);
}

void Shader::disable()
{
	glUseProgram(0);
}

void Shader::glUniformVector3(GLuint location, const Vector3 &vec)
{
	glUniform3f(location, vec.x, vec.y, vec.z);
}

Shader::~Shader()
{
	glDeleteProgram(shaderID);
}

Shader* Shader::newShaderFromOneFile(string filename)
{
	Shader* shader = new Shader();
	string filepath = filename;

	cout << "loading shader \"" << filepath << "\"...\n";

	string originalSource = Math::textFileRead(filepath);
	std::vector<string> parts = Math::splitString(originalSource, "@");
	Math::splitString(originalSource, "@");

	string versionString;
	string vertexString;
	string fragmentString;

	if (parts.size() > 4 || parts.size() < 3)
	{
		throw Exception(string("Error loading shader \"") + filepath + "\": Shader has too many parts");
		return NULL;
	}

	for (unsigned int i = 0; i < 3; i++)
	{
		string firstWord = Math::firstWord(parts[i]);
		if (firstWord == "#version")
			versionString = Math::firstLine(parts[i]);
		else if (firstWord == "vertex")
			vertexString = Math::splitString(parts.at(i), "x\n", 1)[1];
		else if (firstWord == "fragment")
			fragmentString = Math::splitString(parts.at(i), "t\n", 1)[1];
	}

	if (versionString.empty())
	{
		throw Exception(
				"ERROR: No version specified in shader \"" + string(filepath)
						+ "\".");
		return NULL;
	}
	if (fragmentString.empty())
	{
		throw Exception(
				"ERROR: No fragment shader specified in shader \""
						+ string(filepath) + "\".");
		return NULL;
	}
	if (vertexString.empty())
	{
		throw Exception(
				"ERROR: No vertex shader specified in shader \""
						+ string(filename) + "\".");
		return NULL;
	}

	vertexString = versionString + "\n" + vertexString;
	fragmentString = versionString + "\n" + fragmentString + "}";

	shader->init(vertexString.c_str(), fragmentString.c_str(), filepath.c_str(), filepath.c_str());

	return shader;
}

}
