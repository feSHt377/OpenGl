#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <map>

class opengl{
	public:
		//成员变量
		const int defaul_width = 800;
		const int defaul_height = 600;

		//成员方法
		GLFWwindow* init(int width, int height);
		GLFWwindow* init(void);//重载，输入默认长宽
		
		void processInput(GLFWwindow* window);
		int isKeyPressing(int key_enum);

};







