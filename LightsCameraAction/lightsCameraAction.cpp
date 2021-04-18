/*ligthCamerAction.cpp
  Main Program File*/


/* Link to static libraries */
#ifdef _DEBUG
#pragma comment(lib, "glfw3D.lib")
#pragma comment(lib, "glloadD.lib")
#else
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glload.lib")
#endif
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>
#include <stack>
#include <vector>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

// Include headers for our objects
#include "sphere.h"
#include "cube.h"
#include "cylinder.h"
#include "frame.h"
#include "chair.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using namespace glm;

unsigned int loadCubemap(vector<string> faces);

/* Define buffer object indices */
GLuint elementbuffer;

GLuint program[2];	/* Identifier for the shader prgoram */
GLuint current_program = 0;
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */

GLuint colourmode;	/* Index of a uniform to switch the colour mode in the vertex shader
					  I've included this to show you how to pass in an unsigned integer into
					  your vertex shader. */
GLuint emitmode;
GLuint attenuationmode;
GLuint texturemode;

/* Position and view globals */
GLfloat angle_x, angle_inc_x, x, model_scale, z, y, vx, vy, vz, carriage_spin, carriage_spin_inc;
GLfloat angle_y, angle_inc_y, angle_z, angle_inc_z;
GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons
GLuint numlats, numlongs;	//Define the resolution of the sphere object
GLfloat speed;				// movement increment

GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID[2], viewID[2], projectionID[2], lightposID[2], normalmatrixID[2];
GLuint colourmodeID[2], emitmodeID[2], attenuationmodeID[2], texturemodeID[2], fogmodeID[2];

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/
GLuint numspherevertices;
GLuint numhemispherevertices;

GLuint fogmode;


/* Global instances of our objects */
Sphere aSphere;
Cube aCube;
Cylinder aCylinder;
Frame aFrame;
Chair aChair;

/* Define textureID*/
GLuint texID;

// Define fogmode strings to output to screen
string fog_mode_desc[] = { "off", "linear", "exp", "exp2" };


/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	/* Set the object transformation controls to their initial values */
	speed = 0.01f;
	x = 0;
	y = 0;
	z = 0;
	vx = 0; vx = 0, vz = 0;
	light_x = 0; light_y = 0.5; light_z = 1;
	angle_x = angle_y = 0; angle_z = -4;
	angle_inc_x = angle_inc_y = 0;
	angle_inc_z = 0.01;
	model_scale = 1.f;
	aspect_ratio = 1.3333f;
	colourmode = 0; emitmode = 0; texturemode = 0; fogmode = 0;
	attenuationmode = 1; // Attenuation is on by default
	numlats = 40;		// Number of latitudes in our sphere
	numlongs = 40;		// Number of longitudes in our sphere
	carriage_spin = 0;
	carriage_spin_inc = 0.01;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	/* Load and build the vertex and fragment shaders */
	try
	{
		program[0] = glw->LoadShader("lightsCameraAction.vert", "lightsCameraAction.frag");
		program[1] = glw->LoadShader("skyboxShader.vert", "skyboxShader.frag");
	}
	catch (exception &e)
	{
		cout << "Caught exception: " << e.what() << endl;
		cin.ignore();
		exit(0);
	}

	for (int i = 0; i < 2; i++)
	{
		/* Define uniforms to send to vertex shader */
		modelID[i] = glGetUniformLocation(program[i], "model");
		colourmodeID[i] = glGetUniformLocation(program[i], "colourmode");
		emitmodeID[i] = glGetUniformLocation(program[i], "emitmode");
		attenuationmodeID[i] = glGetUniformLocation(program[i], "attenuationmode");
		viewID[i] = glGetUniformLocation(program[i], "view");
		projectionID[i] = glGetUniformLocation(program[i], "projection");
		lightposID[i] = glGetUniformLocation(program[i], "lightpos");
		normalmatrixID[i] = glGetUniformLocation(program[i], "normalmatrix");
		texturemodeID[i] = glGetUniformLocation(program[i], "texturemode");
		fogmodeID[i] = glGetUniformLocation(program[i], "fogmode");
	}

	current_program = 0;

	/* create our sphere and cube objects */
	aSphere.makeSphere(numlats, numlongs);
	aCube.makeCube();
	aCylinder.makeCylinder();
	aFrame.makeFrame();
	aChair.makeChair();

	// Image parameters
	int width, height, nrChannels;

	// load an image using the stb_image.h library 
	// Create an OpenGL texture from the image we have just loaded
	// Be careful with the GL_RGB parameter as this may need to be changed if your image is not 24-bit
	// If unsure then look at the nrChannels returned 
	unsigned char* data = stbi_load("Textures//metalpattern.jpg", &width, &height, &nrChannels, 0);

	// check for an error during the load process
	if (data)
	{
		int pixel_format = 0;
		if (nrChannels == 3)
			pixel_format = GL_RGB;
		else
			pixel_format = GL_RGBA;

		// Create the texture, passing in the pointer to the loaded image pixel data
		glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, width, height, 0, pixel_format, GL_UNSIGNED_BYTE, data);

		// Generate Mip Maps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("stb_image  loading error:");
		exit(0);
	}
	stbi_image_free(data);

	/* Define the texture behaviour parameters */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	//Code for SkyBox taken and adapted from: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	vector<string> faces
	{
		"Textures//posx.jpg",
		"Textures//negx.jpg",
		"Textures//posy.jpg",
		"Textures//negy.jpg",
		"Textures//posz.jpg",
		"Textures//negz.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);


	// This is the location of the texture object (TEXTURE0), i.e. tex1 will be the name
	// of the sampler in the fragment shader
	int loc = glGetUniformLocation(program[0], "tex1");
	if (loc >= 0) glUniform1i(loc, 0);
	int loc2 = glGetUniformLocation(program[1], "cubemapTexture");
	if (loc2 >= 0) glUniform1i(loc2, 0);


}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
   class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	/* Make the compiled shader program current */
	glUseProgram(program[current_program]);

	// Define our model transformation in a stack and 
	// push the identity matrix onto the stack
	stack<mat4> model;
	model.push(mat4(1.0f));

	// Define the normal matrix
	mat3 normalmatrix;

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	mat4 projection = perspective(radians(30.0f), aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	mat4 view = lookAt(
		vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// Apply rotations to the view position. This wil get appleid to the whole scene
	view = rotate(view, -radians(vx), vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	view = rotate(view, -radians(vy), vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	view = rotate(view, -radians(vz), vec3(0, 0, 1));

	//Code for SkyBox taken and adapted from: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	glDepthMask(GL_FALSE);
	//current_program = 1;
	glUseProgram(program[1]);
	model.push(model.top());
	{

		glUniformMatrix4fv(modelID[1], 1, GL_FALSE, &(model.top()[0][0]));
		glUniformMatrix4fv(projectionID[1], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(viewID[1], 1, GL_FALSE, &view[0][0]);
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[1], 1, GL_FALSE, &normalmatrix[0][0]);

		aCube.drawCube(0);
	}
	model.pop();
	glDepthMask(GL_TRUE);
	glUseProgram(program[0]);

	model.top() = scale(model.top(), vec3(model_scale, model_scale, model_scale));//scale equally in all axis

	// Define the light position and transform by the view matrix
	vec4 lightpos = view * vec4(light_x, light_y, light_z, 1.0);

	// Send our projection and view uniforms to the currently bound shader
	// I do that here because they are the same for all objects
	glUniform1ui(colourmodeID[current_program], colourmode);
	glUniform1ui(attenuationmodeID[current_program], attenuationmode);
	glUniformMatrix4fv(viewID[current_program], 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionID[current_program], 1, GL_FALSE, &projection[0][0]);
	glUniform4fv(lightposID[current_program], 1, value_ptr(lightpos));
	glUniform1ui(texturemodeID[current_program], texturemode);
	glUniform1ui(fogmodeID[current_program], fogmode);

	/* Draw a small sphere in the lightsource position to visually represent the light source */
	model.push(model.top());
	{
		model.top() = translate(model.top(), vec3(light_x, light_y, light_z));
		model.top() = scale(model.top(), vec3(0.05f, 0.05f, 0.05f)); // make a small sphere
																	 // Recalculate the normal matrix and send the model and normal matrices to the vertex shader																							// Recalculate the normal matrix and send to the vertex shader																								// Recalculate the normal matrix and send to the vertex shader																								// Recalculate the normal matrix and send to the vertex shader																						// Recalculate the normal matrix and send to the vertex shader
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our lightposition sphere  with emit mode on*/
		emitmode = 1;
		glUniform1ui(emitmodeID[current_program], emitmode);
		aSphere.drawSphere(drawmode);
		emitmode = 0;
		glUniform1ui(emitmodeID[current_program], emitmode);
	}
	model.pop();

	/*Draws a cylinder for the front left stand*/
	model.push(model.top());
	{
		// Define the model transformations for the cylinder
		model.top() = translate(model.top(), vec3(x - 0.1, y - 0.45, z + 0.38));
		model.top() = rotate(model.top(), -radians(15.0f), glm::vec3(1, 0, 1));
		model.top() = scale(model.top(), vec3(0.025f, 1.0f, 0.025f));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our cylinder*/
		aCylinder.drawCylinder(drawmode);
	}
	model.pop();

	/*Draws a cylinder for the front right stand*/
	model.push(model.top());
	{
		// Define the model transformations for the cylinder
		model.top() = translate(model.top(), vec3(x + 0.14, y - 0.45, z + 0.38));
		model.top() = rotate(model.top(), -radians(13.0f), glm::vec3(1, 0, 0));
		model.top() = rotate(model.top(), radians(15.0f), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(0.025f, 1.0f, 0.025f));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our cylinder*/
		aCylinder.drawCylinder(drawmode);
	}
	model.pop();

	/*Draws a cylinder for the back left stand*/
	model.push(model.top());
	{
		// Define the model transformations for the cylinder
		model.top() = translate(model.top(), vec3(x - 0.1, y - 0.45, z - 0.38));
		model.top() = rotate(model.top(), radians(15.0f), glm::vec3(1, 0, 0));
		model.top() = rotate(model.top(), -radians(13.0f), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(0.025f, 1.0f, 0.025f));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our cylinder*/
		aCylinder.drawCylinder(drawmode);
	}
	model.pop();

	/*Draws a cylinder for the back right stand*/
	model.push(model.top());
	{
		// Define the model transformations for the cylinder
		model.top() = translate(model.top(), vec3(x + 0.14, y - 0.45, z - 0.38));
		model.top() = rotate(model.top(), radians(15.0f), glm::vec3(1, 0, 0));
		model.top() = rotate(model.top(), radians(15.0f), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(0.025f, 1.0f, 0.025f));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our cylinder*/
		aCylinder.drawCylinder(drawmode);
	}
	model.pop();

	// Define the global model transformations (rotate and scale). Note, we're not modifying thel ight source position
	model.top() = rotate(model.top(), -radians(angle_x), glm::vec3(1, 0, 0)); //rotating in clockwise direction around x-axis
	model.top() = rotate(model.top(), -radians(angle_y), glm::vec3(0, 1, 0)); //rotating in clockwise direction around y-axis
	model.top() = rotate(model.top(), -radians(angle_z), glm::vec3(0, 0, 1)); //rotating in clockwise direction around z-axis

	/*Draws the ferris wheel frame*/
	model.push(model.top());
	{
		// Define the model transformations for the frame
		model.top() = translate(model.top(), vec3(x, y, z));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(1, 0, 0));
		model.top() = scale(model.top(), vec3(0.8f, 0.6f, 0.8f));
		

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our frame*/
		aFrame.drawFrame(1);
	}
	model.pop();

	/*Draws the front frame sphere*/
	model.push(model.top());
	{
		// Define the model transformations for the sphere
		model.top() = translate(model.top(), vec3(x, y, z + 0.3));
		model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		colourmode = 1;
		glUniform1ui(colourmodeID[current_program], colourmode);
		/* Draw our sphere*/
		aSphere.drawSphere(drawmode);
		colourmode = 0;
		glUniform1ui(colourmodeID[current_program], colourmode);
	}
	model.pop();

	/*Draws the back frame sphere*/
	model.push(model.top());
	{
		// Define the model transformations for the cylinder
		model.top() = translate(model.top(), vec3(x, y, z - 0.3));
		model.top() = scale(model.top(), vec3(0.1f, 0.1f, 0.1f));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		colourmode = 1;
		glUniform1ui(colourmodeID[current_program], colourmode);
		/* Draw our sphere*/
		aSphere.drawSphere(drawmode);
		colourmode = 0;
		glUniform1ui(colourmodeID[current_program], colourmode);
	}
	model.pop();

	/*Draws the cylinder connecting the spheres*/
	model.push(model.top());
	{
		// Define the model transformations for the cylinder
		model.top() = translate(model.top(), vec3(x, y, z));
		model.top() = scale(model.top(), vec3(0.025f, 0.025f, 0.45f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(1, 0, 0));

		// Send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// Recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* Draw our cylinder*/
		aCylinder.drawCylinder(drawmode);
	}
	model.pop();

	/*Draws chair 1*/
	model.push(model.top());
	{
		// define the model transformations for the chair
		model.top() = translate(model.top(), vec3(x + 0.8, y, z));
		model.top() = rotate(model.top(), radians(carriage_spin), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(1.0f, 1.0f, 1.0f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(0, 1, 0));

		// send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* draw our chair*/
		aChair.drawChair(drawmode);
	}
	model.pop();

	/*Draws chair 2*/
	model.push(model.top());
	{
		// define the model transformations for the chair
		model.top() = translate(model.top(), vec3(x - 0.8, y, z));
		model.top() = rotate(model.top(), radians(carriage_spin), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(1.0f, 1.0f, 1.0f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(0, 1, 0));

		// send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* draw our chair*/
		aChair.drawChair(drawmode);
	}
	model.pop();

	/*Draws chair 3*/
	model.push(model.top());
	{
		// define the model transformations for the chair
		model.top() = translate(model.top(), vec3(x - 0.4, y + 0.7, z));
		model.top() = rotate(model.top(), radians(carriage_spin), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(1.0f, 1.0f, 1.0f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(0, 1, 0));

		// send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* draw our chair*/
		aChair.drawChair(drawmode);
	}
	model.pop();

	/*Draws chair 4*/
	model.push(model.top());
	{
		// define the model transformations for the chair
		model.top() = translate(model.top(), vec3(x + 0.4, y + 0.7, z));
		model.top() = rotate(model.top(), radians(carriage_spin), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(1.0f, 1.0f, 1.0f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(0, 1, 0));

		// send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* draw our chair*/
		aChair.drawChair(drawmode);
	}
	model.pop();

	/*Draws chair 5*/
	model.push(model.top());
	{
		// define the model transformations for the chair
		model.top() = translate(model.top(), vec3(x - 0.4, y - 0.7, z));
		model.top() = rotate(model.top(), radians(carriage_spin), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(1.0f, 1.0f, 1.0f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(0, 1, 0));

		// send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* draw our chair*/
		aChair.drawChair(drawmode);
	}
	model.pop();

	/*Draws chair 6*/
	model.push(model.top());
	{
		// define the model transformations for the chair
		model.top() = translate(model.top(), vec3(x + 0.4, y - 0.7, z));
		model.top() = rotate(model.top(), radians(carriage_spin), glm::vec3(0, 0, 1));
		model.top() = scale(model.top(), vec3(1.0f, 1.0f, 1.0f));
		model.top() = rotate(model.top(), radians(90.0f), glm::vec3(0, 1, 0));

		// send the model uniform and normal matrix to the currently bound shader,
		glUniformMatrix4fv(modelID[current_program], 1, GL_FALSE, &(model.top()[0][0]));

		// recalculate the normal matrix and send to the vertex shader
		normalmatrix = transpose(inverse(mat3(view * model.top())));
		glUniformMatrix3fv(normalmatrixID[current_program], 1, GL_FALSE, &normalmatrix[0][0]);

		/* draw our chair*/
		aChair.drawChair(drawmode);
	}
	model.pop();
	
	glDisableVertexAttribArray(0);
	glUseProgram(program[current_program]);

	/* Modify our animation variables */
	angle_x += angle_inc_x;
	angle_y += angle_inc_y;
	angle_z += angle_inc_z;
	carriage_spin += carriage_spin_inc;
}

//Code for SkyBox taken and adapted from: https://learnopengl.com/Advanced-OpenGL/Cubemaps
unsigned int loadCubemap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
	//Key to spin the wheel clockwise
	if (key == 'Q')
	{
		angle_inc_z += speed;
		carriage_spin_inc += speed;
	}

	//Key to spin the wheel anti-clockwise
	if (key == 'W') {
		angle_inc_z -= speed;
		carriage_spin_inc -= speed;
	}

	//Keys to scale the model
	if (key == 'A') model_scale -= speed / 0.5f;
	if (key == 'S') model_scale += speed / 0.5f;

	//Keys to control light movement
	if (key == '1') light_x -= speed;
	if (key == '2') light_x += speed;
	if (key == '3') light_y -= speed;
	if (key == '4') light_y += speed;
	if (key == '5') light_z -= speed;
	if (key == '6') light_z += speed;

	//Keys to control camera movement
	if ((key == '7') && (vx > -5)) vx -= 1.f;
	if ((key == '8') && (vx < 2)) vx += 1.f;
	if (key == '9') vy -= 1.f;
	if (key == '0') vy += 1.f;

	//Key to alter fog levels
	if (key == 'F' && action != GLFW_PRESS)
	{
		fogmode == 3 ? fogmode = 0 : fogmode++;
		//cout << "Fogmode: " << fog_mode_desc[fogmode] << endl;
	}
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Position light example");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	cout << "KEY CONTROLS" << endl;
	cout << "ESC - Close Window" << endl;
	cout << "" << endl;
	cout << "WHEEL SPIN CONTROLS" << endl;
	cout << "Q - Spin Wheel Clockwise" << endl;
	cout << "W - Spin Wheel Anti-Clockwise" << endl;
	cout << "" << endl;
	cout << "SCALE CONTROLS" << endl;
	cout << "A - Scale Down" << endl;
	cout << "S - Scale Up" << endl;
	cout << "" << endl;
	cout << "LIGHT MOVEMENT CONTROLS" << endl;
	cout << "1 - Left Light Movement" << endl;
	cout << "2 - Right Light Movement" << endl;
	cout << "3 - Down Light Movement" << endl;
	cout << "4 - Up Light Movement" << endl;
	cout << "5 - Forward Light Movement" << endl;
	cout << "6 - Backward Light Movement" << endl;
	cout << "" << endl;
	cout << "CAMERA CONTROLS" << endl;
	cout << "7 - Up Camera Movement" << endl;
	cout << "8 - Down Camera Movement" << endl;
	cout << "9 - Left Camera Movement" << endl;
	cout << "0 - Right Camera Movement" << endl;
	cout << "" << endl;
	cout << "FOG MODE CONTROLS" << endl;
	cout << "F - Changing the Fog Settings" << endl;
	cout << "" << endl;

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	/* Output the OpenGL vendor and version */
	glw->DisplayVersion();

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}

