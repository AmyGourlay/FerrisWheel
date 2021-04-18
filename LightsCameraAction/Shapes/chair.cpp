/* chair.cpp 
 Adapted from cube.cpp by Iain Martin
*/

#include "chair.h"

using namespace std;

/* Define the vertex attributes for vertex positions and normals.*/
Chair::Chair(bool useTexture)
{
	attribute_v_coord = 0;
	attribute_v_colours = 1;
	attribute_v_normal = 2;
	attribute_v_texcoord = 3;
	numvertices = 24;
	enableTexture = useTexture;
}

Chair::~Chair()
{
}

/* Make a chair from hard-coded vertex positions and normals  */
void Chair::makeChair()
{
	/* Define vertices for a chair in 24 triangles */
	GLfloat vertexPositions[] =
	{
		-0.25f, 0.1f, -0.015f,
		-0.25f, -0.15f, -0.015f,
		0.25f, -0.15f, -0.015f,

		0.25f, -0.15f, -0.015f,
		0.25f, 0.1f, -0.015f,
		-0.25f, 0.1f, -0.015f,

		0.25f, -0.15f, -0.015f,
		0.25f, -0.15f, 0.025f,
		0.25f, 0.1f, -0.015f,

		0.25f, -0.15f, 0.025f,
		0.25f, 0.1f, 0.025f,
		0.25f, 0.1f, -0.015f,

		0.25f, -0.15f, 0.025f,
		-0.25f, -0.15f, 0.025f,
		0.25f, 0.1f, 0.025f,

		-0.25f, -0.15f, 0.025f,
		-0.25f, 0.1f, 0.025f,
		0.25f, 0.1f, 0.025f,

		-0.25f, -0.15f, 0.025f,
		-0.25f, -0.15f, -0.015f,
		-0.25f, 0.1f, 0.025f,

		-0.25f, -0.15f, -0.015f,
		-0.25f, 0.1f, -0.015f,
		-0.25f, 0.1f, 0.025f,

		-0.25f, -0.15f, 0.025f,
		0.25f, -0.15f, 0.025f,
		0.25f, -0.15f, -0.015f,

		0.25f, -0.15f, -0.015f,
		-0.25f, -0.15f, -0.015f,
		-0.25f, -0.15f, 0.025f,

		-0.25f, 0.1f, -0.015f,
		0.25f, 0.1f, -0.015f,
		0.25f, 0.1f, 0.025f,

		0.25f, 0.1f, 0.025f,
		-0.25f, 0.1f, 0.025f,
		-0.25f, 0.1f, -0.015f,



		-0.25f, -0.125f, 0.025f,
		-0.25f, -0.15f, 0.025f,
		0.25f, -0.15f, 0.025f,

		0.25f, -0.15f, 0.025f,
		0.25f, -0.125f, 0.025f,
		-0.25f, -0.125f, 0.025f,

		0.25f, -0.15f, 0.025f,
		0.25f, -0.15f, 0.25f,
		0.25f, -0.125f, 0.025f,

		0.25f, -0.15f, 0.25f,
		0.25f, -0.125f, 0.25f,
		0.25f, -0.125f, 0.025f,

		0.25f, -0.15f, 0.25f,
		-0.25f, -0.15f, 0.25f,
		0.25f, -0.125f, 0.25f,

		-0.25f, -0.15f, 0.25f,
		-0.25f, -0.125f, 0.25f,
		0.25f, -0.125f, 0.25f,

		-0.25f, -0.15f, 0.25f,
		-0.25f, -0.15f, 0.025f,
		-0.25f, -0.125f, 0.25f,

		-0.25f, -0.15f, 0.025f,
		-0.25f, -0.125f, 0.025f,
		-0.25f, -0.125f, 0.25f,

		-0.25f, -0.15f, 0.25f,
		0.25f, -0.15f, 0.25f,
		0.25f, -0.15f, 0.025f,

		0.25f, -0.15f, 0.025f,
		-0.25f, -0.15f, 0.025f,
		-0.25f, -0.15f, 0.25f,

		-0.25f, -0.125f, 0.025f,
		0.25f, -0.125f, 0.025f,
		0.25f, -0.125f, 0.25f,

		0.25f, -0.125f, 0.25f,
		-0.25f, -0.125f, 0.25f,
		-0.25f, -0.125f, 0.025f,


	};

	/* Manually specified colours for our chair */
	float vertexColours[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,



		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,

		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
	};

	/* Manually specified normals for our chair */
	GLfloat normals[] =
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,


		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};

	/* Manually specified texture coordinates for our chair */
	GLfloat texcoords[] =
	{
		// Face 0
		0, 1.f, 0, 0, 1.f, 0,
		1.f, 0, 1.f, 1.f, 0, 1.f,

		// Face 1
		1.0, 0.f, 0.f, 0.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

		// Face 2
		1.f, 0.f, 0.f, 0.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

		// Face 3
		1.f, 0.f, 0.f, 0.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

		// Face 4
		0.f, 0.f, 1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f, 0.f,

		// Face 5
		0.f, 1.f, 1.f, 1.f, 1.f, 0.f,
		1.f, 0.f, 0.f, 0.f, 0.f, 1.f,


		// Face 6
		0, 1.f, 0, 0, 1.f, 0,
		1.f, 0, 1.f, 1.f, 0, 1.f,

		// Face 7
		1.0, 0.f, 0.f, 0.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

		// Face 8
		1.f, 0.f, 0.f, 0.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

		// Face 9
		1.f, 0.f, 0.f, 0.f, 1.f, 1.f,
		0.f, 0.f, 0.f, 1.f, 1.f, 1.f,

		// Face 10
		0.f, 0.f, 1.f, 0.f, 1.f, 1.f,
		1.f, 1.f, 0.f, 1.f, 0.f, 0.f,

		// Face 11
		0.f, 1.f, 1.f, 1.f, 1.f, 0.f,
		1.f, 0.f, 0.f, 0.f, 0.f, 1.f
	};

	/* Create the vertex buffer for the chair*/
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the colours buffer for the chair*/
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the normals  buffer for the chair*/
	glGenBuffers(1, &normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create the texture coordinate  buffer for the chair*/
	if (enableTexture)
	{
		glGenBuffers(1, &texCoordsObject);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordsObject);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texcoords), texcoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


/* Draw the chair by bining the VBOs and drawing triangles */
void Chair::drawChair(int drawmode)
{
	/* Bind chair vertices. Note that this is in attribute index attribute_v_coord */
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(attribute_v_coord);
	glVertexAttribPointer(attribute_v_coord, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind chair colours. Note that this is in attribute index attribute_v_colours */
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(attribute_v_colours);
	glVertexAttribPointer(attribute_v_colours, 4, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind chair normals. Note that this is in attribute index attribute_v_normal */
	glEnableVertexAttribArray(attribute_v_normal);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(attribute_v_normal, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (enableTexture)
	{
		/* Bind chair texture coords. Note that this is in attribute index 3 */
		glEnableVertexAttribArray(attribute_v_texcoord);
		glBindBuffer(GL_ARRAY_BUFFER, texCoordsObject);
		glVertexAttribPointer(attribute_v_texcoord, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	glPointSize(3.f);
	glFrontFace(GL_CW);

	// Switch between filled and wireframe modes
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Draw points
	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, numvertices * 3);
	}
	else // Draw the chair in triangles
	{
		glDrawArrays(GL_TRIANGLES, 0, numvertices * 3);
	}
}