#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <map>

class opengl{
	public:
		//��Ա����
		const int defaul_width = 800;
		const int defaul_height = 600;

		//��Ա����
		GLFWwindow* init(int width, int height);
		GLFWwindow* init(void);//���أ�����Ĭ�ϳ���
		
		void processInput(GLFWwindow* window);
		int isKeyPressing(int key_enum);

};







