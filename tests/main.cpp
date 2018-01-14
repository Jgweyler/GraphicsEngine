#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include <iostream>
#include <cstdint>

//Called when the window is resized.
void onChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height ) {
	glViewport(0, 0, width, height);

}

void handleInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

uint32_t createVertexData(uint32_t* VBO, uint32_t *EBO) {
	float vertices[] = { 0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f, 
					    -0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
						 0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f};

	uint32_t indices[] = { 0, 2, 1 };


	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	return VAO;
}


void render(uint32_t VAO, Shader& shader) {
	glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	while (!glfwWindowShouldClose(window)) {
		handleInput(window);
		render(VAO, shader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}