#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdint>


#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

const uint32_t k_screen_width = 800;
const uint32_t k_screen_height = 800;

float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = (float) k_screen_width/2 ;
float lastY = (float)k_screen_height/2;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 cubePositions[]{
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 6.0f, -20.0f),
	glm::vec3(2.0f, -2.0f, -2.0f),
	glm::vec3(-4.0f, -6.0f, -10.0f),
	glm::vec3(3.0f, -1.0f, -4.0f),
	glm::vec3(-2.0f, 3.0f, 8.0f),
	glm::vec3(2.0f, -2.0f, -3.0f),
	glm::vec3(1.0f, 2.0f, -2.0f),
	glm::vec3(1.0f, 0.0f, -2.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f)
};

//Called when the window is resized.
void onChangeFrameBufferSize(GLFWwindow* window, const int32_t width, const int32_t height ) {
	glViewport(0, 0, width, height);

}

void handleInput(GLFWwindow* window, const float dt) {


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.handleKeyboard(Camera::Movement::Forward, dt);

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.handleKeyboard(Camera::Movement::Backward, dt);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.handleKeyboard(Camera::Movement::Left, dt);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.handleKeyboard(Camera::Movement::Right, dt);
}

void onScroll(GLFWwindow* window, double xoffset, double yoffset) {
	camera.handleMouseScroll(yoffset);

}

void onMouse(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	camera.handleMouseMovement(xoffset, yoffset);
}

uint32_t createVertexData(uint32_t* VBO, uint32_t *EBO) {
	float vertices[] = {
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,  //Front
		 0.5f, -0.5f, 0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f,  0.5f, 0.5f,		0.0f, 1.0f,

		 0.5f, -0.5f, 0.5f,		0.0f, 0.0f,  //Right
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f, 0.5f, 0.5f,		0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,  //Back
		-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,  //Left
		-0.5f,  0.5f, 0.5f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,  //Bottom
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,
		 0.5f, -0.5f, 0.5f,		1.0f, 1.0f,

		-0.5f,  0.5f, 0.5f,		0.0f, 0.0f,  //Top
		 0.5f,  0.5f, 0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,
	};

	uint32_t indices[] = { 0, 1, 2,		0, 2, 3,
						   4, 5, 6,		4, 6, 7,
						   8, 9, 10,	8, 10, 11,
						   12, 13, 14,	12, 14, 15,
						   16, 17, 18,	16, 18, 19,
						   20, 21, 22,	20, 22, 23};


	uint32_t VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, VBO);
	
	glGenBuffers(1, EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);


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

void render(const uint32_t VAO, const Shader& shader, const uint32_t texture) {
	glClearColor(0.2f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.use();
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);


	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.getFOV()), (float)k_screen_width / (float)k_screen_height, 0.1f, 100.0f);

	
	shader.set("view", camera.getViewMatrix());
	shader.set("projection", projection);

	for (uint32_t i = 0; i < 10; i++) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f + (20.0f * i);
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.0f));

		shader.set("model", model);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

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
	
	window = glfwCreateWindow(k_screen_height, k_screen_height, "Window", NULL, NULL);

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

	glfwSetCursorPosCallback(window, onMouse);
	glfwSetScrollCallback(window, onScroll);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	uint32_t VBO, EBO;
	uint32_t VAO = createVertexData(&VBO, &EBO);

	Shader shader("../tests/vertex.vs", "../tests/fragment.fs");

	uint32_t text = createTexture("../tests/texture_1.jpg");

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);


	shader.use();
	shader.set("texture", 0);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		handleInput(window, deltaTime);
		render(VAO, shader, text);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteTextures(1, &text);

	glfwTerminate();
	return 0;
}