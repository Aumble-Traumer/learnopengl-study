#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <vector>
#include "header/shader.h"
#include "header/kit.h"
#include "header/camera.h"
using namespace std;
using namespace glm;

float screenWidth, screenHeight;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

Camera camera;
float lastX = 400;
float lastY = 200;
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

vec3 lightPos(1.2f, 1.0f, 2.0f);
vec3 boxPos(1.0f, 0.5f, 0.0f);
vec3 lightColor(1.0f);

int main() {
	init(3, 3);
	GLFWwindow* window = createWindow("LearnOpenGL", 800, 600);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Error: Failed to initialize GLAD" << endl;
		return -1;
	}
	screenWidth = 800, screenHeight = 600;
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	float boxVertex[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	float cubeVertex[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	vector<string> faces{
		"resource/texture/right.jpg",
		"resource/texture/left.jpg",
		"resource/texture/top.jpg",
		"resource/texture/bottom.jpg",
		"resource/texture/front.jpg",
		"resource/texture/back.jpg",
	};
	unsigned int VBO, boxVAO, lightVAO, skyboxVAO, skyboxVBO;

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertex), boxVertex, GL_STATIC_DRAW);
	glBindVertexArray(boxVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glGenVertexArrays(1, &lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertex), boxVertex, GL_STATIC_DRAW);
	glBindVertexArray(lightVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &skyboxVBO);
	glGenVertexArrays(1, &skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), cubeVertex, GL_STATIC_DRAW);
	glBindVertexArray(skyboxVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int texture1 = createTexture("resource/texture/container2.png", false);
	unsigned int texture2 = createTexture("resource/texture/container2_specular.png", false);
	unsigned int skyboxTexture = createCubeTexture(false, faces);

	shader boxShader("boxVer.shader", "boxFrag.shader");
	shader lightShader("boxVer.shader", "lightboxFrag.shader");
	shader skyboxShader("CubeVer.shader", "CubeFrag.shader");

	Material boxMate;
	Light boxLight;
	FLight flight;
	flight.cutOff = cos(radians(12.5f));
	flight.outerCutOff = cos(radians(17.5f));

	boxMate.ambient = vec3(1.0,0.5,0.31);
	boxMate.diffuse(boxShader, 0, "material.diffuse");
	boxMate.specular(boxShader, 1, "material.diffuse");
	boxMate.shininess = 32;

	boxLight.ambient = vec3(0.2);
	boxLight.diffuse = vec3(0.5);
	boxLight.specular = vec3(1.0);

	mat4 model = mat4(1.0f), projection = mat4(1.0f), view = mat4(1.0f);
	//model = rotate(model, radians(-55.0f), vec3(1.0f, 0.0f, 0.0f));
	view = translate(view, vec3(0.0f, 0.0f, -3.0f));
	projection = perspective(radians(45.0f), screenWidth / screenHeight, 0.1f, 100.0f);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.38f, 0.44f, 0.77f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.GetViewMatrix();
		projection = perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		boxLight.diffuse = lightColor * vec3(0.5f);
		boxLight.ambient = boxLight.diffuse * vec3(0.2f);

		model = translate(model, lightPos);
		model = scale(model, vec3(0.2f));
		lightShader.use();
		lightShader.setUniform("model", model);
		lightShader.setUniform("view", view);
		lightShader.setUniform("projection", projection);
		lightShader.setUniform("LightColor", lightColor);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = mat4(1.0f);
		boxShader.use();
		boxShader.setUniform("light.ambient", boxLight.ambient);
		boxShader.setUniform("light.diffuse", boxLight.diffuse);
		boxShader.setUniform("light.specular", boxLight.specular);
		boxShader.setUniform("lightColor", lightColor);
		boxShader.setUniform("lightPos", lightPos);
		boxShader.setUniform("viewPos", camera.Position);
		boxShader.setUniform("model", model);
		boxShader.setUniform("view", view);
		boxShader.setUniform("projection", projection);
		boxShader.setUniform("material.ambient", boxMate.ambient);
		//已进行修改：boxMate.diffuse -> boxMate.diffuse(&boxShader,0,"material.diffuse");
		boxMate.diffuse(boxShader, 0, "material.diffuse");
		boxMate.specular(boxShader, 1, "material.specular");
		boxShader.setUniform("material.shininess", boxMate.shininess);
		//手电筒
		//boxShader.setUniform("flight.position", flight.position);
		//boxShader.setUniform("flight.direction", flight.direction);
		//boxShader.setUniform("flight.cutOff", flight.cutOff);
		//boxShader.setUniform("flight.outerCutOff", flight.outerCutOff);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

		glBindVertexArray(boxVAO);
		for (unsigned int i = 0; i < 10; i++){
			mat4 boxModel = mat4(1.0f);
			boxModel = glm::translate(boxModel, cubePositions[i]);
			//float angle = tan(glfwGetTime()) * sin(glfwGetTime()) * (i + 1) * glfwGetTime() + glfwGetTime();
			float angle = sin(glfwGetTime());
			boxModel = glm::rotate(boxModel, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			boxShader.setUniform("model", boxModel);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glDepthMask(GL_FALSE);
		skyboxShader.use();
		skyboxShader.setUniform("view", view);
		skyboxShader.setUniform("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
