/////  
/////  Main function; for now most of the code is here
/////
#include "maininc.h"
#include "ioshader.h"
#include "stb_image.h"


///  Constants. Should probably load this from a config file or smth...
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

const char vertexShaderFile[] = "shaders/vertexShader.vshr";

const char fragmentShaderFile[] = "shaders/fragmentShader.fshr";

const glm::vec3 rotationAxis = glm::vec3(0.0, 0.0, 1.0);
//-----

//change window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//handling keyboard input
void processInput(GLFWwindow *window, glm::mat4& trans, float rotVal);

int main(int arc, char** args)
{
	//// Load and initalize glfw and glad
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	///initalize window
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);

	if (window == NULL) {

		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;

	}


	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	///load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	/// build and compile our shader program
	/// ------------------------------------

	int vshrhandle = compileShader(vertexShaderFile, GL_VERTEX_SHADER);
	int fshrhandle = compileShader(fragmentShaderFile, GL_FRAGMENT_SHADER);
	int shaderProgram = linkShader(vshrhandle, fshrhandle);
	cleanShader(vshrhandle);
	cleanShader(fshrhandle);


	/// set up vertex data (and buffer(s)) and configure vertex attributes
	/// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2, 0, 2, 3
	};



	//// Load all the needed data into a VAO
	//   ------------------------------------
	// VAO is for storing function calls, 
	// VBO is for storing vertices (position, color, texture coords...)
	// EBO is for storing indices of the vertices to be drawn

	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object to store following commands
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	//// Load textures
	//// -------------

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	/// Load image amd bind it? to a gl texture
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("assets/container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		//how gl stores it        //img format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);


	/// Second texture  (should probably turn this into a function?..)
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("assets/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	//Varibles for loading into uniforms(better turn this into a class)
	float img_opacity = 0.3f;
	//...

	///MATH. Mostly matrix operations (should pobably be wrapped in a function or a class to manage whatever's needed)

	const float moveVal = 0.01f;
	glm::mat4 trans = glm::mat4(1);								//rotation axis;
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	/// Load uniforms
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture2"), 1);
	glUniform1f(glGetUniformLocation(shaderProgram, "img_opacity"), img_opacity);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "trans_mat"), 1, GL_FALSE, glm::value_ptr(trans));

	//// Main draw loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window, trans, moveVal);

		//RENDER:
		glClearColor(0.4f, 0.1f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);	
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "trans_mat"), 1, GL_FALSE, glm::value_ptr(trans));

		//texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//bind buffers	
		glBindVertexArray(VAO);
		//draw triangle using EBO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//swap double buffer
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	///Clean-up
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, glm::mat4& trans, float moveVal) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	/// -- Input to change some uniforms --
	else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(-moveVal, 0, 0));
	}
	else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(moveVal, 0, 0));
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(0, moveVal, 0));
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		trans = glm::translate(trans, glm::vec3(0, -moveVal, 0));
	}
}