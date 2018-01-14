#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>

#include <shader.h>


#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

//Called when the window is resized.
void onChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height ) {
	glViewport(0, 0, width, height);

}

void handleInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

uint32_t createVertexData(uint32_t* VBO, uint32_t *EBO) {
	float vertices[] = { 0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
					     0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
						 -0.5f,-0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
						 -0.5, 0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	0.0f, 1.0f };

	uint32_t indices[] = { 0, 3, 1,
						   1, 3, 2};


	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return VAO;
}

uint32_t createTexture(const char* path) {

	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;

	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else {
		std::cout << "Failed load texture: " << path << std::endl;
	}

	return texture;
}

void render(const uint32_t VAO, const Shader& shader, const uint32_t texture1, const uint32_t texture2) {
	glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main(int argc, char* argv[]) {

	if (!glfwInit()) {
		std::cout << "Error Initializing GLFW" << std::endl;
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	
	window = glfwCreateWindow(800, 600, "Window", NULL, NULL);

	if (!window) {
		std::cout << "Error creating window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Try to connect to OPENGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Error Initializing GLAD" << std::endl;
		return -1;
	}

	//Setting callback when the window is resized
	glfwSetFramebufferSizeCallback(window, onChangeFrameBufferSize);
	uint32_t VBO, EBO;
	uint32_t VAO = createVertexData(&VBO, &EBO);

	Shader shader("../tests/vertex.vs", "../tests/fragment.fs");

	uint32_t text1 = createTexture("../tests/texture_1.jpg");
	uint32_t text2 = createTexture("../tests/texture_2.jpg");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader.use();
	shader.set("texture1", 0);
	shader.set("texture2", 1);

	while (!glfwWindowShouldClose(window)) {
		handleInput(window);
		render(VAO, shader, text1, text2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &text1);
	glDeleteTextures(1, &text2);

	glfwTerminate();
	return 0;
}