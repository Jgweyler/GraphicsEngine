#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Default camera values
const float k_Yaw = -90.0f;
const float k_Pitch = 0.0f;
const float k_Speed = 2.5f;
const float k_Sensivity = 0.1f;
const float k_FOV = 45.0f;

class Camera {
public:
	//Options for camera movement
	enum class Movement {
		Forward = 0,
		Backward = 1,
		Left = 2,
		Right = 3,
	};

	
	Camera();  //Empty constructor
	~Camera(); // Destructor

	//Constructor with vectors
	Camera(const glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		   const float yaw = k_Yaw, const float pitch = k_Pitch);

	//Constructor with scalars
	Camera(const float posX, const float posY, const float posZ,
		   const float upX, const float upY, const float upZ,
		   const float yaw, const float pitch); 

	glm::mat4 getViewMatrix() const; //Returns current view matrix
	float getFOV() const;            //Returns current FOV
	glm::vec3 getPosition() const; 	 //Return current position 

	//Process Input from Keyboard
	void handleKeyboard(const Movement direction, const float deltaTime);
	//Process mouse movement
	void handleMouseMovement(const float xoffset, const float yoffset, const bool constrainPitch = true);
	//Process mouse scroll
	void handleMouseScroll(const float yoffset);

private:

	//Calculate front vector
	void updateCameraVectors();

	//Camera attributes
	glm::vec3 position_, front_, up_, right_, worldUp_;
	//Euler Angles
	float yaw_, pitch_;
	//Camera options
	float fov_;
};

#endif