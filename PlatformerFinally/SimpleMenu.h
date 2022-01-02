#pragma once
#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <math.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"

class SimpleMenu {
public:
	std::string LargeTitle;
	std::string subHeader;

	std::string prompt1;

	GLuint fontTextureID;
	void renderText(ShaderProgram* program);

	SimpleMenu(GLuint* fontID) { fontTextureID = *fontID; }
};