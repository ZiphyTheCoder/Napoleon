#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "Soldier.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "Shader.h"
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::vec2 getMouseWorldPos(GLFWwindow* window, int screenWidth, int screenHeight, float worldWidth, float worldHeight, glm::vec2 cameraPos);
glm::vec3 moveToPosition(glm::vec2 pos1, glm::vec2 pos2, float deltaTime, float scalar);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Soldiers

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	Shader myShader("3.3.shader.vs", "3.3.shader.fs");

	bool unitSelected = false;

	Soldier np;
	np.isSelected = false;
	std::vector<float> vert = np.loadRect(0.5f, 0.8f);
	np.color = glm::vec4(1.0f);
	np.TexSetup("E:\\cpp\\WW2Strat\\Stradegy\\StandingTest.png");

	// Render Setup

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), vert.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	



	glm::vec2 cameraPos = glm::vec2(0.0f);
	glm::vec2 oldPos = glm::vec2(0.1f);

	np.position = glm::vec2(0.0f);
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window)) {
		// input
		// -----
		processInput(window);

		static glm::vec2 mouseWorldPos = glm::vec2(0.0f);

		// render
		// ------
		glClearColor(0.1f, 0.05f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		glm::vec4 greenTint = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		glm::mat4 trans = glm::mat4(1.0f);

		unsigned int transformLoc = glGetUniformLocation(myShader.ID, "transform");
		unsigned int colorLoc = glGetUniformLocation(myShader.ID, "greenColor");

		// Frame rate independent movement
		float deltaTime = 0.016f; // Replace with actual time calculation


		// Handle selection with a state variable to prevent multiple selections per click
		static bool wasMousePressed = false;
		static bool lastClickWasSelection = false;
		bool isMousePressed = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS;

		glm::vec2 worldPos;

		worldPos = glm::vec2(
			(np.position.x * (10.0f / 11.0f)) + cameraPos.x,
			(np.position.y * (10.0f / 11.0f)) + cameraPos.y
		);

		if (isMousePressed && !wasMousePressed)
		{
			mouseWorldPos = getMouseWorldPos(window, SCR_WIDTH, SCR_HEIGHT, 10.0f, 10.0f, cameraPos);
		}

		if (isMousePressed && !wasMousePressed) {
			// Mouse was just pressed
			if (mouseWorldPos.x >= (worldPos.x - 0.125f) && mouseWorldPos.x <= (worldPos.x + 0.125f) &&
				mouseWorldPos.y >= worldPos.y && mouseWorldPos.y <= worldPos.y + 0.8f) 
			{
					np.isSelected = true;
					unitSelected = true;

					np.color = greenTint;
					glUniform4f(colorLoc, 1, np.color.x, np.color.y, np.color.z);


				std::cout << "Has been selected" << std::endl;

				lastClickWasSelection = true;
			}
			else {
				lastClickWasSelection = false;
			}
		}

		wasMousePressed = isMousePressed;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			np.velocity = glm::vec2(0, 0);
			np.isSelected = false;
			unitSelected = false;
			np.color = glm::vec4(0.0f);
			glUniform4f(colorLoc, 1, np.color.x, np.color.y, np.color.z);

		}








		if (unitSelected)
		{

			np.color = greenTint;
			glUniform4f(colorLoc, 1, np.color.x, np.color.y, np.color.z);


			if (!lastClickWasSelection)
			{
				float dist;
				dist = glm::distance(np.position, mouseWorldPos);

				if (dist > 0.005f)
				{
					np.velocity = moveToPosition(np.position, mouseWorldPos, 0.016f, 0.4f);
					np.position += np.velocity;
				}
				else {
					np.velocity = glm::vec2(0.0f);
				}
				trans = glm::translate(trans, glm::vec3(np.position, 0.0f));
				glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			}
		}
		else {
			np.velocity = glm::vec2(0, 0);
			np.position += np.velocity;
			trans = glm::translate(trans, glm::vec3(np.position, 0.0f));

			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			np.color = glm::vec4(0.0f);
			glUniform4f(colorLoc, 1, np.color.x, np.color.y, np.color.z);
		}
	

		myShader.use();
		np.Bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);






		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}








	np.Delete();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

glm::vec2 getMouseWorldPos(GLFWwindow* window, int screenWidth, int screenHeight, float worldWidth, float worldHeight, glm::vec2 cameraPos) {
	double xpos, ypos;
	float nDCx, nDCy;
	glm::vec2 worldPos;

	// Always get current cursor position
	glfwGetCursorPos(window, &xpos, &ypos);

	// Convert screen coordinates to normalized device coordinates
	nDCx = (2.0f * xpos) / screenWidth - 1.0f;
	nDCy = 1.0f - (2.0f * ypos) / screenHeight;

	// Convert to world coordinates
	worldPos.x = nDCx * (worldWidth / 11.0f) + cameraPos.x;
	worldPos.y = nDCy * (worldHeight / 11.0f) + cameraPos.y;

	return worldPos;
}

glm::vec3 moveToPosition(glm::vec2 pos1, glm::vec2 pos2, float deltaTime, float scalar)
{
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;

	glm::vec2 direction = glm::normalize(glm::vec2(dx, dy)); // Normalize to get unit direction
	glm::vec2 velocity = direction * scalar * deltaTime; // Scale by speed and deltaTime

	return glm::vec3(velocity, 0.0f);
}
