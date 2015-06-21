#pragma once

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#else
#include <GLES2/gl2.h>
#endif

class SimpleRenderer
{
public:
	SimpleRenderer();
	~SimpleRenderer();
	void Draw();
	void UpdateWindowSize(GLsizei width, GLsizei height);
	void Init();

private:
	GLuint mProgram;
	GLsizei mWindowWidth;
	GLsizei mWindowHeight;

	GLint mPositionAttribLocation;
	GLint mColorAttribLocation;

	GLint mModelUniformLocation;
	GLint mViewUniformLocation;
	GLint mProjUniformLocation;

	GLuint mVertexPositionBuffer;
	GLuint mVertexColorBuffer;
	GLuint mIndexBuffer;

	int mDrawCount;
};
