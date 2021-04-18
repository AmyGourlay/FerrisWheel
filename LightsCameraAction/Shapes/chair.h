/* chair.h
 Adapted from cube.h by Iain Martin
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Chair
{
public:
	Chair(bool useTexture = true);
	~Chair();

	void makeChair();
	void drawChair(int drawmode);

	// Define vertex buffer object names (e.g as globals)
	GLuint positionBufferObject;
	GLuint colourObject;
	GLuint normalsBufferObject;
	GLuint texCoordsObject;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;
	GLuint attribute_v_texcoord;

	int numvertices;
	bool enableTexture;

};

