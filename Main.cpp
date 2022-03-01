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

void Generate2DShape(GLfloat LargestZ);
void ProcessVertices(std::string axis);

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
	GLfloat xmax = MaxX(vertices, verticesSize, vertexDataSize);
	std::cout << xmax << " is the maximum x value in the data \n\n\n"; // DEBUGGING LINE

	GLfloat ymax = MaxY(vertices, verticesSize, vertexDataSize);
	std::cout << ymax << " is the maximum y value in the data \n\n\n"; // DEBUGGING LINE

	GLfloat zmax = MaxZ(vertices, verticesSize, vertexDataSize);
	std::cout << zmax << " is the maximum z value in the data \n\n\n"; // DEBUGGING LINE

	GLfloat xmin = MinX(vertices, verticesSize, vertexDataSize);
	std::cout << xmin << " is the minimum x value in the data \n\n\n"; // DEBUGGING LINE

	GLfloat ymin = MinY(vertices, verticesSize, vertexDataSize);
	std::cout << ymin << " is the minimum y value in the data \n\n\n"; // DEBUGGING LINE

	GLfloat zmin = MinZ(vertices, verticesSize, vertexDataSize);
	std::cout << zmin << " is the minimum Z value in the data \n\n\n"; // DEBUGGING LINE
	


	Generate2DShape(zmax);

	//Allocate space for an array
	GLfloat vertices_2d_array[33];
	GLuint indices_2d[3];
	
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0); //This links the coordinates
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); //This links the colors
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); //This links texture coordinates
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // This links normals.

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	VAO VAO2;
	VAO2.Bind();
	VBO VBO2(vertices_2d_array, sizeof(vertices_2d_array));
	EBO EBO2(indices_2d, sizeof(indices_2d));
	
	VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0); //This links the coordinates
	VAO2.LinkAttrib(VBO2, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float))); //This links the colors
	VAO2.LinkAttrib(VBO2, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float))); //This links texture coordinates
	VAO2.LinkAttrib(VBO2, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float))); // This links normals.

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

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	
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

void Generate2DShape(GLfloat LargestZ)
{
	//each row of the array has 11 elements
	// (x, y, z) (r, g, b), (s, t), (xnorm, ynorm, znorm)
	int counter = 0;
	int vertexCount = 0;
	//This function does some stuff oh god
	ProcessVertices("z");
	//for (auto x : vertices_2d) {
		//
	//}
}
	
//This function will process the vertices and only return the frontmost unique ones.

void ProcessVertices(std::string axis)
{
	std::cout << "Processing Vertices\n\n\n\n";
	//For storing the current vertex coordinates.
	GLfloat currentVertexCoordinates[4] = {};
	std::vector<GLfloat> vertices_mutable{};
	//Get the size of the array
	int size = static_cast<int>(sizeof(vertices) / sizeof(GLfloat));

	//create a vector that we can modify to not destroy the 3D object.
	for (auto x : vertices) {
		vertices_mutable.push_back(x);
	}

	int counter = 0;
	int loopCount = 0;
	//maintains a list of rows to erase.
	//go up to the coordinates before the last.
	//fix iteration ( no hardcode) 
	for (int i = 0; i < vertices_mutable.size(); i += vertexDataSize) { //iterate through our vector and grab the vertex coordinates.
		currentVertexCoordinates[0] = vertices_mutable[i]; //store x
		currentVertexCoordinates[1] = vertices_mutable[i+1]; //store y
		currentVertexCoordinates[2] = vertices_mutable[i+2]; //store z
		currentVertexCoordinates[3] = i; //store its position.
		bool found = false;
		//debug line
		std::cout << "Current: " << currentVertexCoordinates[0] << " " << currentVertexCoordinates[1] << " " << currentVertexCoordinates[2] << std::endl;
		
		for (int j = 0; j < vertices_mutable.size(); j += vertexDataSize) { //iterate through our vector
			//check x y and that it is not itself.
			if (currentVertexCoordinates[0] == vertices_mutable.at(j) && currentVertexCoordinates[1] == vertices_mutable.at(j + 1) && j != currentVertexCoordinates[3]) {
				//If the current is greater, flag the other for erase.
				if (float(currentVertexCoordinates[2]) >= float(vertices_mutable.at(j + 2))) {
					std::cout << "(ERASE FOUND) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 2] << std::endl;
					std::cout << vertices_mutable.at(j) << " " << vertices_mutable.at(j + 1) << " " << vertices_mutable.at(j + 2) << std::endl;
					//erase elements j thru j + 10
					vertices_mutable.erase(vertices_mutable.begin() + j, vertices_mutable.begin() + j + 11);
					//if we erase a point behind the current position, we need to update the location of the current position
					if (j < currentVertexCoordinates[3]) {
						currentVertexCoordinates[3] -= 11;
					}
				}
				//else, remove the current
				else {
					std::cout << "(ERASE CURRENT) The z value of current is " << currentVertexCoordinates[2] << " and the z value of j is " << vertices_mutable[j + 3] << std::endl;
					vertices_mutable.erase(vertices_mutable.begin() + i, vertices_mutable.begin() + i + 11);
				}
				//if found, we need to check the vertices again because the positions have shifted.
				j = 0;
			}
		}
		std::cout << "\n\n Vertices remaining at this iteration \n\n";
		std::cout << "Size of the array is: " << vertices_mutable.size() << std::endl;
		for (auto x : vertices_mutable) {
			std::cout << x << " ";
			++counter;
			if (counter == 11) {
				counter = 0;
				std::cout << std::endl;
			}
		}
		//check xyz against all other xyz
		//if match, take the max of z
		//if no match, push to vector.
		
	}
	std::cout << "\n\n The final list of vertices is: " << std::endl;
	int help = 0;
	for (auto x : vertices_mutable) {
		vertices_2d.push_back(x);
		std::cout << x << " ";
		++help;
		if (help == 11) {
			help = 0;
			std::cout << std::endl;
		}
	}
}

/*
* Function Name: Process Vertices
* Description: Takes in an axis (+z, -z, +x, -x, +y, -y), a boolean and a list of vertices. The axis indicates which axis to transform based on.
*			   The boolean will determine whether or not the transformed object is flattened to that axis or not.
*			   The list of vertices determines what vertices the transformation will be applied to.
* Inputs: Axis, Alignment, Vertices
* Output: 2D Vertices
*/
/*
void ProcessVertices(std::string axis, bool axis_alignment, GLfloat vertexData[]) {
	if (axis == "+x") {
		//TransformPosX();
	}
	else if (axis == "-x") {
		//TransformNegX();
	}
	else if (axis == "+y") {
		//TransformPosY()
	}
	else if (axis == "-y") {
		//TransformNegY()
	}
	else if (axis == "+z") {
		//TransformPosZ()
	}
	else if (axis == "-z") {
		//TransformNegZ()
	}
	else {
		std::cout << "Invalid axis input, should be: +z +y +x -z -y -x" << std::endl;
		exit(-1);
	}
}
*/