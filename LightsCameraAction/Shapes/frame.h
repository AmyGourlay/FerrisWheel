/**
 * Adapted from Cylinder.h that was created by David Ogle in 2015 and updated and tidied up
 * by Iain Martin in November 2017.
 * Provided to the AC41001/AC51008 Graphics class to help debug their own cylinder objects or to
 * used in their assignment to provide another flexible
 */

#include "wrapper_glfw.h"
#include <glm/glm.hpp>

class Frame
{
private:
	glm::vec3 colour;
	GLfloat radius, length;
	GLuint definition;
	GLuint frameBufferObject, frameNormals, frameColours, frameElementbuffer;
	GLuint num_pvertices;
	GLuint isize;
	GLuint numberOfvertices;

	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_v_colours;

	void defineVertices();

public:
	Frame();
	Frame(glm::vec3 c);
	~Frame();
	void makeFrame();
	void drawFrame(int drawmode);
};

