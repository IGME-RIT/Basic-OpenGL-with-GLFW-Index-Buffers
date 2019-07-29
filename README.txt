Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the Index Buffer Tutorial!
Prerequesites: Two Triangles

In the previous tutorial, we drew a square by drawing 
2 triangles with 6 total vertices. The problem was, we
only should need 4 vertices. Two of the vertices were duplicates,
which means 2 vertices were passed to the vertex shader that
shouldn't have needed to be passed.

For one quad, its no big deal,
but if we try to draw a model with 10,000 quads, we might accidentally
process 60,000 vertices instead of the 40,000 necessary vertices.
The question is: How do we re-use vertices that were processed in the
vertex shader, and then vertices in more than one polygon?
By doing so, we can use 4 vertices to make 2 polygons (to make the quad).

The answer: Index Buffers!

	shape quad;
	quad.vertices = {
		// POSITION					    //COLOR				
		glm::vec3(-0.5f, -0.5f, 0.0f),	glm::vec3(1.0f, 0.0f, 0.0f), // Vertex 0
		glm::vec3(0.5, -0.5f, 0.0f),	glm::vec3(0.0f, 1.0f, 0.0f), // Vertex 1
		glm::vec3(-0.5f, 0.5f, 0.0f),  	glm::vec3(0.0f, 0.0f, 1.0f), // Vertex 2
		glm::vec3(0.5f, 0.5f, 0.0f),  	glm::vec3(1.0f, 0.0f, 0.0f)  // Vertex 3
	};

	quad.indicies = {
		0,1,2,	//Triangle 1: Vertex 0, Vertex 1, and Vertex 2
		2,1,3	//Triangle 2: Vertex 2, Vertex 1, and Vertex 3
	};
	
Here we create a vertex buffer with four vertices, and each of those vertices
gets processed one time in the vertex shader. After the vertex buffer calculates,
each vertex, the rasterizer takes the result of each vertex, 
and uses the index buffer to connect each vertex to other vertices, depending on 
what is in the index buffer. Prior to now, without an index buffer, the rasterizer
would automatically connect vertices 0, 1, and 2 together, then it would connect
vertices 3, 4, and 5 together, and so on. Our index buffer changes that

When we make our index buffer:
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLushort)*quad.indicies.size(),
		&quad.indicies[0], 
		GL_STATIC_DRAW);
		
The size of our index buffer is the sizeof(short), 
multiplied by the number of indices. That tells us that
each index is only two bytes large. This means the maximum
number of vertices we can have (in this setup), is 65,000, because
that is the largest index that a 2-byte digit can hold.

If we want to save memory, we can reduce each index to one byte,
then we could only have 255 vertices, or we can expand the size of 
each index to four bytes, allowing us to have 2 billion vertices in 
one vertex buffer. None of that needs to happen now, but it would be
nice to make a note of this if it is ever needed

Common mistake:
One time I (personally) used 2-byte index buffers on Xbox One and
PlayStation 4, and I forgot about the 65,000 vertex rule. Most of
my models worked fine, but one day I had a character with 100,000 vertices,
and only part of the character was drawing on Xbox. On the other hand,
PlayStation 4 completely crashed on boot. It took me a month to finally
figure it out. Don't repeat the mistake

When used correctly, 2-byte index buffers can save a lot of memory, 
opposed to using 4-byte index buffers for every single object
