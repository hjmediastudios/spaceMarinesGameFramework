#pragma once

#include "../Prerequisites.hpp"
#include "../Math.hpp"

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

namespace SpaceMarines
{

using namespace std;

class Shader
{
public:
	static Shader* newShaderFromTwoFiles(const char *vsFilename, const char *fsFilename);
	static Shader* newShaderFromOneFile(string filename);

	virtual ~Shader();
	void init(const char *vsSource, const char *fsSource, const char* vsFilename, const char* fsFilename);
	static void validateShader(GLuint shader, const char* shaderSourceText, const char* filename);
	static void validateProgram(GLuint program);
	GLuint getShaderID() const;
	GLuint getUniformLocation(const char* uniformName) const;
	GLuint getUniformBlockLocation(const char* uniformName) const;
	void enable();
	void disable();
	static void glUniformVector3(GLuint location, const Vector3 &vec);
	void bindUniformBlock(const char *uboNameInShader, GLuint uboBufferIndex);
protected:
	Shader();
	bool inited;
	unsigned int shader_vp;
	unsigned int shader_fp;
	unsigned int shaderID;
};

} /* namespace BrikEngine */

