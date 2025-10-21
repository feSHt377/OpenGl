#pragma once

#include <conio.h>
//#include <thread>




/*
	@feSHt3
	int1 :: key_enum
	int2 :: key_status	->  GLFW_RELEASE
						->  GLFW_PRESS
*/
std::unordered_map<int, int> keyStatus;

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	keyStatus[GLFW_KEY_W] = glfwGetKey(window, GLFW_KEY_W);
	keyStatus[GLFW_KEY_A] = glfwGetKey(window, GLFW_KEY_A);
	keyStatus[GLFW_KEY_S] = glfwGetKey(window, GLFW_KEY_S);
	keyStatus[GLFW_KEY_D] = glfwGetKey(window, GLFW_KEY_D);
	keyStatus[GLFW_KEY_UP] = glfwGetKey(window, GLFW_KEY_UP);
	keyStatus[GLFW_KEY_DOWN] = glfwGetKey(window, GLFW_KEY_DOWN);
	keyStatus[GLFW_KEY_LEFT] = glfwGetKey(window, GLFW_KEY_LEFT);
	keyStatus[GLFW_KEY_RIGHT] = glfwGetKey(window, GLFW_KEY_RIGHT);
	keyStatus[GLFW_KEY_SPACE] = glfwGetKey(window, GLFW_KEY_SPACE);
	keyStatus[GLFW_KEY_LEFT_SHIFT] = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	keyStatus[GLFW_KEY_DELETE] = glfwGetKey(window, GLFW_KEY_DELETE);
	keyStatus[GLFW_KEY_CAPS_LOCK] = glfwGetKey(window, GLFW_KEY_CAPS_LOCK);
	keyStatus[GLFW_KEY_1] = glfwGetKey(window, GLFW_KEY_1);
	keyStatus[GLFW_KEY_KP_8] = glfwGetKey(window, GLFW_KEY_KP_8);
	keyStatus[GLFW_KEY_KP_2] = glfwGetKey(window, GLFW_KEY_KP_2);
	keyStatus[GLFW_KEY_F] = glfwGetKey(window, GLFW_KEY_F);
}

/*
int checkKeyStatus(int key_enum) {
	return keyStatus[key_enum];
}
*/




bool isKeyPressing(int key_enum) {
	if (keyStatus[key_enum] == GLFW_PRESS) {
		return true;
	}
	else {
		return false;
	}

}



