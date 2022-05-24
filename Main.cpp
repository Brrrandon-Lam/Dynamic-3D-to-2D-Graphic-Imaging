#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <vector>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include"objLoader.hpp"

//Function Declaration
//These function take in an array and return a specific value from the vertex data
GLfloat MinX(GLfloat vertexData[], size_t verticesSize, int numComponents); //Return the minimum X value from vertex data 
GLfloat MinY(GLfloat vertexData[], size_t verticesSize, int numComponents); //return the minimum Y value from vertex data
GLfloat MinZ(GLfloat vertexData[], size_t verticesSize, int numComponents); //Return the minimum Z value from vertex data

GLfloat MaxX(GLfloat vertexData[], size_t verticesSize, int numComponents); //Return the maximum X value from the vertex data
GLfloat MaxY(GLfloat vertexData[], size_t verticesSize, int numComponents); //Return the maximum Y value from the vertex data
GLfloat MaxZ(GLfloat vertexData[], size_t verticesSize, int numComponents); //Return the maximum Z value from the vertex data

bool draw_3d = true;
bool axis_align = true; //If you want to axis align the new set of vertices.

void TransformPosX(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents);
void TransformPosY(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents);
void TransformPosZ(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents);

void TransformNegX(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents);
void TransformNegY(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents);
void TransformNegZ(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents);



const unsigned int width = 1600;
const unsigned int height = 900;
const unsigned int vertexDataSize = 11;



// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side
	 
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
};

//create a dynamic container for our 2d representation.
std::vector<GLfloat> vertices_2d;

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // Bottom side
	0, 2, 3, // Bottom side
	4, 6, 5, // Left side
	7, 9, 8, // Non-facing side
	10, 12, 11, // Right side
	13, 15, 14 // Facing side
};

GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 1600 by 900 (width by height) pixels, naming it "OpenGL Tutorial"
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Tutorial", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 1600, y = 900
	glViewport(0, 0, width, height);

	//Find the greatest or smallest value in the list of vertices.

	size_t verticesSize = sizeof(vertices) / sizeof(vertices[0]); //Calculates the size of the vertex data

	//Each of these functions takes in a list of vertices, the size of the list of vertices, and the size of an independent vertex.
	

	TransformPosX("+x", true, vertices, verticesSize, vertexDataSize);
	//TransformPosY("+y", true, vertices, verticesSize, vertexDataSize);
	//TransformPosZ("+z", true, vertices, verticesSize, vertexDataSize);

	//TransformNegX("-x", true, vertices, verticesSize, vertexDataSize);
	//TransformNegY("-y", false, vertices, verticesSize, vertexDataSize);
	//TransformNegZ("-z", true, vertices, verticesSize, vertexDataSize);

	//GLfloat xmax = MaxX(vertices, verticesSize, vertexDataSize);
	//std::cout << xmax << " is the maximum x value in the data \n\n\n"; // DEBUGGING LINE

	//GLfloat ymax = MaxY(vertices, verticesSize, vertexDataSize);
	//std::cout << ymax << " is the maximum y value in the data \n\n\n"; // DEBUGGING LINE

	//GLfloat zmax = MaxZ(vertices, verticesSize, vertexDataSize);
	//std::cout << zmax << " is the maximum z value in the data \n\n\n"; // DEBUGGING LINE

	//GLfloat xmin = MinX(vertices, verticesSize, vertexDataSize);
	//std::cout << xmin << " is the minimum x value in the data \n\n\n"; // DEBUGGING LINE

	//GLfloat ymin = MinY(vertices, verticesSize, vertexDataSize);
	//std::cout << ymin << " is the minimum y value in the data \n\n\n"; // DEBUGGING LINE

	//GLfloat zmin = MinZ(vertices, verticesSize, vertexDataSize);
	//std::cout << zmin << " is the minimum Z value in the data \n\n\n"; // DEBUGGING LINE

	struct InfoOBJ getInfoTest = loadOBJ("./mjbcube.obj");
	GLfloat* testLoad = getInfoTest.list;
	GLuint* INDICIES_TEST = new GLuint[getInfoTest.size];

	VAO VAO_TEST;
	VAO_TEST.Bind();

	VBO VBO_TEST(testLoad, sizeof(testLoad));

	EBO EBO_TEST(INDICIES_TEST, sizeof(INDICIES_TEST));
	std::cout << "Created VAO_TEST with " << sizeof(testLoad) * getInfoTest.size << " vertices and " << sizeof(INDICIES_TEST) * getInfoTest.size << " indices." << std::endl;

	VAO_TEST.LinkAttrib(VBO_TEST, 0, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)0); //This links the coordinates
	VAO_TEST.LinkAttrib(VBO_TEST, 1, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(3 * sizeof(float))); //This links the colors
	VAO_TEST.LinkAttrib(VBO_TEST, 2, 2, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(6 * sizeof(float))); //This links texture coordinates
	VAO_TEST.LinkAttrib(VBO_TEST, 3, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(8 * sizeof(float))); // This links normals.

	VAO_TEST.Unbind();
	VBO_TEST.Unbind();
	EBO_TEST.Unbind();

	//Allocate space for an array
	GLfloat vertices_2d_array[55];
	GLuint indices_2d[6];
	
	//copy contents over
	
	for(int i = 0; i < vertices_2d.size(); i++) {
		vertices_2d_array[i] = vertices_2d[i];
	}

	//generate inorder indices.
	int numIndices = vertices_2d.size() / vertexDataSize;
	//GLuint* indices_2d = new GLuint[numIndices];
	for (int i = 0; i < numIndices; ++i) {
		indices_2d[i] = i;
	}

	// Generates Shader object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));
	std::cout << "Created VAO with " << sizeof(vertices) << " vertices and " << sizeof(indices) << " indices." << std::endl;
	
	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)0); //This links the coordinates
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(3 * sizeof(float))); //This links the colors
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(6 * sizeof(float))); //This links texture coordinates
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(8 * sizeof(float))); // This links normals.

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(vertices_2d_array, sizeof(vertices_2d_array));
	EBO EBO2(indices_2d, sizeof(indices_2d));
	
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)0); //This links the coordinates
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(3 * sizeof(float))); //This links the colors
	VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(6 * sizeof(float))); //This links texture coordinates
	VAO2.LinkAttrib(VBO2, 3, 3, GL_FLOAT, vertexDataSize * sizeof(float), (void*)(8 * sizeof(float))); // This links normals.

	VAO2.Unbind();
	VBO2.Unbind();
	EBO2.Unbind();

	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();



	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);

	glm::vec3 testPos = glm::vec3(5.0f, 5.0f, 5.0f);
	glm::mat4 testModel = glm::mat4(1.0f);
	testModel = glm::translate(testModel, testPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model_test"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model_test"), 1, GL_FALSE, glm::value_ptr(testModel));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//Note that if using a PNG use RGBA. JPG only has RGB channels.
	Texture zosukeTex("zosuke.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	zosukeTex.texUnit(shaderProgram, "tex0", 0);



	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		const float time = static_cast<float>(glfwGetTime());
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glDrawBuffer(GL_BACK);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Handles camera inputs
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		// Tells OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		// Export the camMatrix to the Vertex Shader of the pyramid
		camera.Matrix(shaderProgram, "camMatrix");
		// Binds texture so that is appears in rendering
		zosukeTex.Bind();
		// Bind the VAO so OpenGL knows to use it
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			VAO2.Bind(); 	
			glDrawElements(GL_TRIANGLES, sizeof(indices_2d) / sizeof(int), GL_UNSIGNED_INT, 0);
		}
		else {
			VAO1.Bind();
			// Draw primitives, number of indices, datatype of indices, index of indices
			glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		}

		VAO_TEST.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(INDICIES_TEST) / sizeof(int), GL_UNSIGNED_INT, 0);

		// Tells OpenGL which Shader Program we want to use
		lightShader.Activate();
		// Export the camMatrix to the Vertex Shader of the light cube
		camera.Matrix(lightShader, "camMatrix");
		// Bind the VAO so OpenGL knows to use it
		lightVAO.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	VAO2.Delete();
	VBO2.Delete();
	EBO2.Delete();
	VBO_TEST.Delete();
	VAO_TEST.Delete();
	EBO_TEST.Delete();
	zosukeTex.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}

GLfloat MaxZ(GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	size_t counter = 0;
	GLfloat zmax = -1000.f;
	for (size_t x = 0; x < verticesSize; x++) {
		if (counter == 2) {
			if (((float)zmax < (float)vertexData[x])) { //If we are at the z coordinate then compare against largest.
				zmax = vertexData[x];
			}
		}
		counter += 1;
		if (counter == numComponents) {
			counter = 0;
		}
		continue;
	}
	return zmax;
}

GLfloat MaxY(GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	size_t counter = 0;
	GLfloat ymax = -1000.f;
	for (size_t x = 0; x < verticesSize; x++) {
		if (counter == 1) {
			if (((float)ymax < (float)vertexData[x])) { //If we are at the z coordinate then compare against largest.
				ymax = vertexData[x];
			}
		}
		counter += 1;
		if (counter == numComponents) {
			counter = 0;
		}
		continue;
	}
	return ymax;
}

GLfloat MaxX(GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	size_t counter = 0;
	GLfloat xmax = -1000.f;
	for (size_t x = 0; x < verticesSize; x++) {
		if (counter == 0) {
			if (((float)xmax < (float)vertexData[x])) { //If we are at the z coordinate then compare against largest.
				xmax = vertexData[x];
			}
		}
		counter += 1;
		if (counter == numComponents) {
			counter = 0;
		}
		continue;
	}
	return xmax;
}

GLfloat MinZ(GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	size_t counter = 0;
	GLfloat zmin = 1000.f;
	for (size_t x = 0; x < verticesSize; x++) {

		if (counter == 2) {
			if (((float)zmin > (float)vertexData[x])) { //If we are at the z coordinate then compare against largest.
				//std::cout << "Larger found" << std::endl;
				zmin = vertexData[x];
				//std::cout << "The current highest z value is: " << largest << std::endl;
			}
		}
		counter += 1;
		if (counter == numComponents) {
			counter = 0;
		}
		continue;
	}
	return zmin;
}

GLfloat MinY(GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	size_t counter = 0;
	GLfloat ymin = 1000.f;
	for (size_t x = 0; x < verticesSize; x++) {

		if (counter == 1) {
			//std::cout << "Counter is: " << counter << std::endl;
			//std::cout << "Comparing " << ymin << " and " << vertexData[x] << std::endl;
			if (((float)ymin > (float)vertexData[x])) { //If we are at the z coordinate then compare against largest.
				//std::cout << "Larger found" << std::endl;
				ymin = vertexData[x];
				//std::cout << "The current highest z value is: " << largest << std::endl;
			}
		}
		counter += 1;
		if (counter == numComponents) {
			//std::cout << "End of current vertex data" << std::endl;
			counter = 0;
		}
		continue;
	}
	return ymin;
}

GLfloat MinX(GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	size_t counter = 0;
	GLfloat xmin = 1000.f;
	for (size_t x = 0; x < verticesSize; x++) {

		if (counter == 0) {
			//std::cout << "Counter is: " << counter << std::endl;
			//std::cout << "Comparing " << xmin << " and " << vertexData[x] << std::endl;
			if (((float)xmin > (float)vertexData[x])) { //If we are at the z coordinate then compare against largest.
				//std::cout << "Larger found" << std::endl;
				xmin = vertexData[x];
				//std::cout << "The current highest z value is: " << largest << std::endl;
			}
		}
		counter += 1;
		if (counter == numComponents) {
			//std::cout << "End of current vertex data" << std::endl;
			counter = 0;
		}
		continue;
	}
	return xmin;
}

void TransformPosY(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	std::cout << "SIZEOF VERTEXDATA IS " << verticesSize << std::endl; //DEBUG LINE
	//create a vector that we can modify to not destroy the 3D object.
	for (size_t i = 0; i < verticesSize; ++i) {
		vertices_mutable.push_back(vertexData[i]);
	}
	int counter = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	for (size_t i = 0; i < vertices_mutable.size(); i += numComponents) { //iterate through our vector and grab the vertex coordinates.
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i + 1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i + 2]; //store z
		currentVertexCoordinates[3] = i; //store its position.

		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << std::endl; //Debug Line

		for (size_t j = 0; j < vertices_mutable.size(); j += numComponents) { //iterate through our vector
			//check x y and that it is not itself.
			if (currentVertexCoordinates[0] == vertices_mutable.at(j) && currentVertexCoordinates[2] == vertices_mutable.at(j + 2) && j != currentVertexCoordinates[3]) {
				//If the current is greater, flag the other for erase.
				if (float(currentVertexCoordinates[1]) >= float(vertices_mutable[j+1])) {
					//std::cout << "(ERASE FOUND) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 2] << std::endl; //Debug Line
					//std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					if (j != 0) {
						vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + numComponents);
					}
					else {
						vertices_mutable.erase(vertices_mutable.begin(), vertices_mutable.begin() + numComponents);
					}
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < currentVertexCoordinates[3]) {
						currentVertexCoordinates[3] -= numComponents;
					}
				}
				//else, remove the current
				else {
					//std::cout << "(ERASE CURRENT) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 3] << std::endl; //Debug Line
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + numComponents);
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
		}
	}
	//If we want to axis align
	size_t counter3 = 0;
	size_t iterationCounter = 0;
	if (axis_align == true) {
		GLfloat ymax = MaxY(vertices, verticesSize, vertexDataSize);
		for (auto x : vertices_mutable) {
			if (counter3 == 1) {
				vertices_mutable[counter3 + (11 * iterationCounter)] = ymax;
			}
			++counter3;
			if ((int)counter3 == numComponents) {
				++iterationCounter;
				counter3 = 0;
			}
		}
	}
	/**** DEBUGGING BLOCK ****/
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int counter2 = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++counter2;
		if (counter2 == numComponents) {
			counter2 = 0;
			std::cout << std::endl;
		}
	}
}

void TransformNegY(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	std::cout << "SIZEOF VERTEXDATA IS " << verticesSize << std::endl; //DEBUG LINE
	//create a vector that we can modify to not destroy the 3D object.
	for (size_t i = 0; i < verticesSize; ++i) {
		vertices_mutable.push_back(vertexData[i]);
	}
	int counter = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	for (size_t i = 0; i < vertices_mutable.size(); i += numComponents) { //iterate through our vector and grab the vertex coordinates.
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i + 1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i + 2]; //store z
		currentVertexCoordinates[3] = (int)i; //store its position.

		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << std::endl; //Debug Line

		for (size_t j = 0; j < vertices_mutable.size(); j += numComponents) { //iterate through our vector
			//check x y and that it is not itself.
			if (currentVertexCoordinates[0] == vertices_mutable.at(j) && currentVertexCoordinates[2] == vertices_mutable.at(j + 2) && j != currentVertexCoordinates[3]) {
				//If the current is greater, flag the other for erase.
				if (float(currentVertexCoordinates[1]) <= float(vertices_mutable[j + 1])) {
					//std::cout << "(ERASE FOUND) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 2] << std::endl; //Debug Line
					//std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + numComponents);
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < currentVertexCoordinates[3]) {
						currentVertexCoordinates[3] -= numComponents;
					}
				}
				//else, remove the current
				else {
					//std::cout << "(ERASE CURRENT) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 3] << std::endl; //Debug Line
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + numComponents);
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
		}
	}
	//If we want to axis align
	size_t counter3 = 0;
	size_t iterationCounter = 0;
	if (axis_align == true) {
		GLfloat ymin = MinY(vertices, verticesSize, vertexDataSize);
		for (auto x : vertices_mutable) {
			if (counter3 == 1) {
				vertices_mutable[counter3 + (numComponents * iterationCounter)] = ymin;
			}
			++counter3;
			if (counter3 == numComponents) {
				++iterationCounter;
				counter3 = 0;
			}
		}
	}
	/**** DEBUGGING BLOCK ****/
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int counter2 = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++counter2;
		if (counter2 == numComponents) {
			counter2 = 0;
			std::cout << std::endl;
		}
	}
}

void TransformPosZ(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	/****** PROCESS VERTICES *****/

	std::cout << "Processing Vertices\n\n\n\n"; //Debug Line
	//For storing the current vertex coordinates.
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	//Get the size of the array

	//std::cout << "SIZEOF VERTEXDATA IS " << verticesSize << std::endl; //DEBUG LINE

	//create a vector that we can modify to not destroy the 3D object.
	for (size_t i = 0; i < verticesSize; ++i) {
		vertices_mutable.push_back(vertexData[i]);
	}

	int counter = 0;
	int loopCount = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	for (size_t i = 0; i < vertices_mutable.size(); i += numComponents) { //iterate through our vector and grab the vertex coordinates.
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i+1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i+2]; //store z
		currentVertexCoordinates[3] = i; //store its position.
		bool found = false;
		
		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << std::endl; //Debug Line
		
		for (size_t j = 0; j < vertices_mutable.size(); j += numComponents) { //iterate through our vector
			//check x y and that it is not itself.
			if (currentVertexCoordinates[0] == vertices_mutable.at(j) && currentVertexCoordinates[1] == vertices_mutable.at(j + 1) && j != currentVertexCoordinates[3]) {
				//If the current is greater, flag the other for erase.
				if (float(currentVertexCoordinates[2]) >= float(vertices_mutable.at(j + 2))) {
					//std::cout << "(ERASE FOUND) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 2] << std::endl; //Debug Line
					//std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + numComponents);
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < currentVertexCoordinates[3]) {
						currentVertexCoordinates[3] -= numComponents;
					}
				}
				//else, remove the current
				else {
					//std::cout << "(ERASE CURRENT) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 3] << std::endl; //Debug Line
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + numComponents);
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
		}
	}
	//If we want to axis align
	size_t counter3 = 0;
	size_t iterationCounter = 0;
	if (axis_align == true) {
		GLfloat zmax = MaxZ(vertices, verticesSize, vertexDataSize);
		for (auto x : vertices_mutable) {
			if (counter3 == 2) {
				vertices_mutable[counter3 + (numComponents *iterationCounter)] = zmax;
			}
			++counter3;
			if (counter3 == numComponents) {
				++iterationCounter;
				counter3 = 0;
			}
		}
	}
	/**** DEBUGGING BLOCK ****/
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int counter2 = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++counter2;
		if (counter2 == numComponents) {
			counter2 = 0;
			std::cout << std::endl;
		}
	}
}

void TransformNegZ(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	/****** PROCESS VERTICES *****/

	std::cout << "Processing Vertices\n\n\n\n"; //Debug Line
	//For storing the current vertex coordinates.
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	//Get the size of the array

	//std::cout << "SIZEOF VERTEXDATA IS " << verticesSize << std::endl; //DEBUG LINE

	//create a vector that we can modify to not destroy the 3D object.
	for (size_t i = 0; i < verticesSize; ++i) {
		vertices_mutable.push_back(vertexData[i]);
	}

	int counter = 0;
	int loopCount = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	for (size_t i = 0; i < vertices_mutable.size(); i += numComponents) { //iterate through our vector and grab the vertex coordinates.
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i + 1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i + 2]; //store z
		currentVertexCoordinates[3] = i; //store its position.
		bool found = false;

		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << std::endl; //Debug Line

		for (size_t j = 0; j < vertices_mutable.size(); j += numComponents) { //iterate through our vector
			//check x y and that it is not itself.
			if (currentVertexCoordinates[0] == vertices_mutable.at(j) && currentVertexCoordinates[1] == vertices_mutable.at(j + 1) && j != currentVertexCoordinates[3]) {
				//If the current is greater, flag the other for erase.
				if (float(currentVertexCoordinates[2]) <= float(vertices_mutable.at(j + 2))) {
					//std::cout << "(ERASE FOUND) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 2] << std::endl; //Debug Line
					//std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + numComponents);
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < currentVertexCoordinates[3]) {
						currentVertexCoordinates[3] -= numComponents;
					}
				}
				//else, remove the current
				else {
					//std::cout << "(ERASE CURRENT) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 3] << std::endl; //Debug Line
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + numComponents);
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
		}
	}
	//If we want to axis align
	size_t counter3 = 0;
	size_t iterationCounter = 0;
	if (axis_align == true) {
		GLfloat zmin = MinZ(vertices, verticesSize, vertexDataSize);
		for (auto x : vertices_mutable) {
			if (counter3 == 2) {
				vertices_mutable[counter3 + (numComponents * iterationCounter)] = zmin;
			}
			++counter3;
			if (counter3 == numComponents) {
				++iterationCounter;
				counter3 = 0;
			}
		}
	}
	/**** DEBUGGING BLOCK ****/
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int counter2 = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++counter2;
		if (counter2 == numComponents) {
			counter2 = 0;
			std::cout << std::endl;
		}
	}
}

void TransformNegX(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	std::cout << "SIZEOF VERTEXDATA IS " << verticesSize << std::endl; //DEBUG LINE
	//create a vector that we can modify to not destroy the 3D object.
	for (size_t i = 0; i < verticesSize; ++i) {
		vertices_mutable.push_back(vertexData[i]);
	}
	int counter = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	for (size_t i = 0; i < vertices_mutable.size(); i += numComponents) { //iterate through our vector and grab the vertex coordinates.
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i + 1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i + 2]; //store z
		currentVertexCoordinates[3] = (int)i; //store its position.

		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << std::endl; //Debug Line

		for (size_t j = 0; j < vertices_mutable.size(); j += numComponents) { //iterate through our vector
			//check x y and that it is not itself.
			if (currentVertexCoordinates[1] == vertices_mutable.at(j + 1) && currentVertexCoordinates[2] == vertices_mutable.at(j + 2) && j != currentVertexCoordinates[3]) {
				//If the current is greater, flag the other for erase.
				if (float(currentVertexCoordinates[0]) <= float(vertices_mutable[j])) {
					//std::cout << "(ERASE FOUND) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 2] << std::endl; //Debug Line
					//std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + numComponents);
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < currentVertexCoordinates[3]) {
						currentVertexCoordinates[3] -= numComponents;
					}
				}
				//else, remove the current
				else {
					//std::cout << "(ERASE CURRENT) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 3] << std::endl; //Debug Line
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + numComponents);
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
		}
	}
	//If we want to axis align
	size_t counter3 = 0;
	size_t iterationCounter = 0;
	if (axis_align == true) {
		GLfloat xmin = MinX(vertices, verticesSize, vertexDataSize);
		for (auto x : vertices_mutable) {
			if (counter3 == 0) {
				vertices_mutable[counter3 + (numComponents * iterationCounter)] = xmin;
			}
			++counter3;
			if (counter3 == numComponents) {
				++iterationCounter;
				counter3 = 0;
			}
		}
	}
	/**** DEBUGGING BLOCK ****/
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int counter2 = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++counter2;
		if (counter2 == numComponents) {
			counter2 = 0;
			std::cout << std::endl;
		}
	}
}

void TransformPosX(std::string axis, bool axis_align, GLfloat vertexData[], size_t verticesSize, int numComponents)
{
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	std::cout << "SIZEOF VERTEXDATA IS " << verticesSize << std::endl; //DEBUG LINE
	//create a vector that we can modify to not destroy the 3D object.
	for (size_t i = 0; i < verticesSize; ++i) {
		vertices_mutable.push_back(vertexData[i]);
		if (i % numComponents == 0 && i != 0) {
			std::cout << std::endl;
		}
		std::cout << vertices_mutable[i] << " ";
		
	}
	int counter = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	for (size_t i = 0; i < vertices_mutable.size(); i += numComponents) { //iterate through our vector and grab the vertex coordinates.
		//TODO: convert to struct (3 floats + int)
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i+1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i+2]; //store z
		currentVertexCoordinates[3] = i; //store its position.

		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << "\n" << std::endl; //Debug Line

		std::cout << " -- CURRENT VERTEX DATA -- " << std::endl;
		for (size_t x = 0; x < vertices_mutable.size(); ++x) {
			if (x % numComponents == 0 && x != 0) {
				std::cout << std::endl;
			}
			std::cout << vertices_mutable[x] << " ";
		}
		std::cout << std::endl;
		for (size_t j = 0; j < vertices_mutable.size(); j += numComponents) { //iterate through our vector
			//If we find an overlapping y and z, and find that the positions in the vertex array are not the same, compare for removal.
			std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << " with position " << currentVertexCoordinates[3] << std::endl; //Debug Line
			std::cout << "Comparison: " << vertices_mutable[j] << " " << vertices_mutable[j + 1] << " " << vertices_mutable[j + 2] << " with position " << j << std::endl; //Debug Line
			if (((float)currentVertexCoordinates[1] == (float)vertices_mutable[j+1]) && ((float)currentVertexCoordinates[2] == (float)vertices_mutable[j+2]) && (j != currentVertexCoordinates[3])) {
				//If the current is greater, flag the other for erase.
				//subtract, fabs, small tolerance
				if (float(currentVertexCoordinates[0]) >= float(vertices_mutable[j])) {
					std::cout << "j is " << j << std::endl;
					std::cout << "(ERASE FOUND) The x value of current is " << currentVertexCoordinates[0] << " and the x value of j is " << vertices_mutable[j] << std::endl; //Debug Line
					//std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + numComponents);
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < (size_t)currentVertexCoordinates[3]) {
						std::cout << "Old coordinates are " << currentVertexCoordinates[3] << std::endl;
						currentVertexCoordinates[3] -= numComponents;
						std::cout << "New coordinates are: " << currentVertexCoordinates[3] << std::endl;
					}
				}
				//else, remove the current
				else  {
					std::cout << "j is " << j << std::endl;
					std::cout << "(ERASE CURRENT) The x value of current is " << currentVertexCoordinates[0] << " and the x found is " << vertices_mutable[j] << std::endl; //Debug Line
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + numComponents);
					i -= numComponents;
					break;
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
			else {
				std::cout << "Skip" << std::endl;
			}
		}
		std::cout << "FINISHED OUTER LOOP" << std::endl;
	}
	//If we want to axis align
	size_t counter3 = 0;
	size_t iterationCounter = 0;
	if (axis_align == true) {
		GLfloat xmax = MaxX(vertices, verticesSize, vertexDataSize);
		for (auto x : vertices_mutable) {
			if (counter3 == 0) {
				vertices_mutable[counter3 + (numComponents * iterationCounter)] = xmax;
			}
			++counter3;
			if (counter3 == numComponents) {
				++iterationCounter;
				counter3 = 0;
			}
		}
	}
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int counter2 = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++counter2;
		if (counter2 == numComponents) {
			counter2 = 0;
			std::cout << std::endl;
		}
	}
}