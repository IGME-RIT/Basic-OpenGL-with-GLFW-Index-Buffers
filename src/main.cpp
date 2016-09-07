/*
Title: Index Buffers
File Name: Main.cpp
Copyright � 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "GL/glew.h"
#include "GLFW/glfw3.h"


// Vbo to store our square
GLuint vbo;

// Index buffer used to store the indices of our square
GLuint indexBuffer;


// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Index Buffering!", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	// Initializes the glew library
	glewInit();





	// A square rendered by making two triangles.
	// We could easily add 3 mort vertices to our vertex buffer, but then we would be repeating two of them.
	// Instead, we can use an index buffer.
	// An index buffer holds the indices in order of when they will be drawn

	// (-.5, .5)--(.5, .5)
	//	|				|
	//	|				|
	//	|				|
	// (-.5,-.5)--(.5, -.5)

	// In this example, using only vertices, our vertex buffer would look like this:

	float verticesOnlyExample[] = {
		-.5,  .5,
		 .5,  .5,
		-.5, -.5,
		-.5, -.5,
		 .5,  .5,
		 .5, -.5
	};


	// When using an index buffer, each vertex has an index, and we can reuse them with an index buffer

	// [0]------[1]
	//	|		 |
	//	|		 |
	//	|		 |
	// [2]------[3]

	float verticesWithIndex[] = {
		-.5,  .5,
		.5,  .5,
		-.5, -.5,
		.5, -.5
	};

	// Note: I'm using an unsigned int because it's impossible for there to be a negative index
	// Also, OpenGL only allows unsigned types for index buffers.
	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3
	};


	// Before we were sending 12 floats to the gpu.
	// After we are sending 8 floats and 6 ints.

	// Admittedly, this is slightly less effective when only rendering a 2d square, because only two vertices are reused.
	
	// This becomes much more useful when rendering in 3d.
	/*
			   +----+
			+----+  |
			|    |--+
			+----+
	*/ 
	// A cube has 6 sides, each made of 2 triangles for a total of 12 triangles.
	// If we were use vertices only for our cube, we would have to send 3x12=36 vertices to the gpu.
	// Thats 36x3=108 floats!

	// If we use indexing, we only need to send 8 vertices, or 24 floats.
	// Then we also send our index buffer, which is just 12x3=36 ints.
	// That's almost half as much information that we need to send!




	// Here we set up the vbo just like before:
	// Makes space for our vbo
	glGenBuffers(1, &vbo);

	// Bind vbo to GL_ARRAY_BUFFER to work on it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Buffer vertices into the bound vbo.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8, &verticesWithIndex[0], GL_STATIC_DRAW);

	// Unbind the vbo
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	// Here we do the same thing, except for our index buffer!

	// Makes space for our index buffer
	glGenBuffers(1, &indexBuffer);

	// We neeed to bind our index buffer to GL_ARRAY_BUFFER.
	glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);

	// Buffer indices into the index buffer.
	glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * 6, &indices[0], GL_STATIC_DRAW);

	// Unbind the index Buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);





	// Main Loop

	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen to black nothingness.
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.0, 0.0, 0.0, 0.0);
		



		// Start drawing geometry.


		// Bind the vbo we want to use.
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Set up our vertexAttribArray at attrib index 0.
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// At this point we are done using our vbo, so we can unbind it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);



		// Unlike the vertex buffer, OpenGL, doesn't use the index buffer with an attrib.
		// Instead, bind it to the GL_ELEMENT_ARRAY_BUFFER location to be used.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		


		// Enable attrib at index 0
		glEnableVertexAttribArray(0);

		// The actual draw call:
		// glDrawElements tells OpenGL to draw using the index buffer bound at GL_ELEMENT_ARRAY_BUFFER
		// GL_TRIANGLES is the draw mode, GL_UNSIGNED_INT is the type of our indices.
		// 6 is the number of indices to use, and 0 is the first index we want to draw at.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		// Disable attrib at index 0.
		glDisableVertexAttribArray(0);

		// Unbind the index buffer.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);






		// Remember to swap the backbuffer to the front so we can see it!
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}





	// Delete vbo and index buffer when done using them.
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &indexBuffer);


	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
