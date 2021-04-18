/**
* frame.cpp
* Adaption of cylinder.pp that was created by David Ogle in 2015 and updated and tidied up
* by Iain Martin in November 2017.
*/

#include "frame.h"
const float PI = 3.141592653589f;  /* pi */

#include <iostream>

using namespace glm;
using namespace std;


/**
 * IM: Constrcutor with no parameters which creates a white frame
 */
Frame::Frame() : Frame(vec3(1.f, 1.f, 1.f))
{

}


Frame::Frame(vec3 c) : colour(c)
{
	this->radius = 1.0f;
	this->length = 1.0f;

	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;

	// hard-coded number of vertices around the circle
	// To change this value you will need to generalise the vertex number and offsets in
	// functions defineVertices() and makeFrame(). It has already been done in drawFrame().
	this->definition = 6;
	numberOfvertices = definition * 4 + 2; //number of verticies in the Frame
}

Frame::~Frame()
{
}

void Frame::makeFrame()
{
	defineVertices();

	GLuint pindices[406]; //204 //201
	for (int i = 0; i < 7; i++)
	{
		pindices[i] = i;
	}
	pindices[7] = 1;
	for (int i = 8; i < 15; i++)
	{
		pindices[i] = i - 1;
	}
	pindices[15] = 8;

	for (int i = 204; i < 404; i++)
	{
		pindices[i] = i - 2;
	}
	pindices[404] = 202;
	pindices[405] = 203;
	this->isize = (sizeof(pindices) / sizeof(*pindices));
	glGenBuffers(1, &this->frameElementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->frameElementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, isize * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
//based on
//https://www.opengl.org/discussion_boards/showthread.php/167115-Creating-cylinder
void Frame::defineVertices()
{
	vec3 vertices[402];
	vec3 normals[402];
	vec3 colour[402];

	//number of pVertieces is total points * 3;
	GLfloat halfLength = this->length / 2;

	//define vertex at the center/top of the cylider
	vertices[0] = vec3(0, halfLength, 0);
	normals[0] = vec3(0.0, 1.0, 0.0);
	colour[0] = this->colour;

	//for every point around the circle
	for (int i = 1; i < this->definition + 1; i++)
	{
		GLfloat theta = (2 * PI) / this->definition * i;

		GLfloat x = radius * cos(theta);
		GLfloat y = halfLength;
		GLfloat z = radius * sin(theta);

		vertices[i] = vec3(x, y, z);
		normals[i] = vec3(0.0, 1.0, 0.0);
		colour[i] = this->colour;
	}
	vertices[7] = vec3(0, -halfLength, 0);
	normals[7] = vec3(0.0, -1.0, 0.0);
	colour[7] = this->colour;

	//for every point around the circle
	for (int i = 8; i < (this->definition * 2) + 2; i++)
	{
		GLfloat theta = (2 * PI) / this->definition * (i - 8);

		GLfloat x = radius * cos(theta);
		GLfloat y = -halfLength;
		GLfloat z = radius * sin(theta);

		vertices[i] = vec3(x, y, z);
		normals[i] = vec3(0.0, -1.0, 0.0);
		colour[i] = this->colour;
	}

	/* Create the vertex buffer for the frame */
	glGenBuffers(1, &this->frameBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, this->frameBufferObject);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec3) * numberOfvertices), &vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &this->frameNormals);
	glBindBuffer(GL_ARRAY_BUFFER, frameNormals);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec3) * numberOfvertices), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &this->frameColours);
	glBindBuffer(GL_ARRAY_BUFFER, frameColours);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(vec3) * numberOfvertices), &colour[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Frame::drawFrame(int drawmode)
{
	/* Bind the vertes positions */
	glEnableVertexAttribArray(attribute_v_coord);
	glBindBuffer(GL_ARRAY_BUFFER, frameBufferObject);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the colours */
	glEnableVertexAttribArray(attribute_v_colours);
	glBindBuffer(GL_ARRAY_BUFFER, this->frameColours);
	glVertexAttribPointer(attribute_v_colours, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind the normals */
	glEnableVertexAttribArray(attribute_v_normal);
	glBindBuffer(GL_ARRAY_BUFFER, frameNormals);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, numberOfvertices);
	}
	else
	{
		int numfanvertices = definition + 2;
		int numsidevertices = numfanvertices * 2;
		int side_offset = definition * 2 + 2;
		// Draw the frame using filled triangles
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->frameElementbuffer);

		// Draw the top lid
		glDrawElements(GL_TRIANGLE_FAN, numfanvertices, GL_UNSIGNED_INT, (GLvoid*)0);

		// Draw the bottom lid
		glDrawElements(GL_TRIANGLE_FAN, numfanvertices, GL_UNSIGNED_INT, (GLvoid*)(numfanvertices * sizeof(GLuint)));

		// Draw the sides
		glDrawElements(GL_TRIANGLE_STRIP, side_offset, GL_UNSIGNED_INT, (GLvoid*)(numsidevertices * sizeof(GLuint)));
	}
}