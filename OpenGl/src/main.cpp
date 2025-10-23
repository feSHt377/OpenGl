#pragma once
#include <Windows.h>
#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unordered_map>
#include <map>

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <random>
#include <ctime>

#define STB_IMAGE_IMPLEMENTATION  // 关键！激活stb_image的实现
#include "Header/stb_image.h"


#include "Header/file.h"

#include "Header/kbEvent.h"
#include "Header/Shader.h"
#include "Header/Model.h"

//#include "Header/openGl.h"s

int screenWidth = 1280;
int screenHeight = 1024;

bool needUpdateProjection = true;//是否需要更新投影矩阵


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);//摄像机位置
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);//摄像机目标位置
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);//摄像机方向（从目标指向摄像机位置的向量）
//方向向量(Direction Vector)并不是最好的名字，因为它实际上指向从它到目标向量的相反方向
glm::vec3 cameraFront = -cameraDirection;//摄像机前向量（与摄像机方向相反的向量）(面向目标物体的向量)

glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);//世界空间中的上向量
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));//摄像机右向量（右手坐标系）
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);//摄像机上向量（正交化后的上向量）
//格拉姆—施密特正交化(Gram-Schmidt Process)
float pitch = 0.0f;//俯仰角
float yaw = -90.0f;//偏航角

float cameraSpeed = 2.0f; //摄像机移动速度 单位/s
float cameraSensitivity = 1.0f;//视角移动灵敏度 角度/s

float cameraSpeedBoost = 3.0f;//摄像机加速倍数

glm::vec3 lightDefaultPos(2.2f, 1.0f, -2.0f);

glm::vec3 lightPos = lightDefaultPos;
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);



glm::vec3 defaultDirectionalLightDir(-0.2f, -1.0f, -0.3f);//方向（平行）光默认方向 我们将方向定义为从光源出发的方向，你可以很容易看到光的方向朝下

float defaultSpotLightCutOff = 12.5f;//聚光灯默认内切角
float defaultSpotLightOuterCutOff = 17.5f;//聚光灯默认外切角
bool spotLightOn = false;//聚光灯开关
float spotLightCd = 0.2f;//聚光灯开关cd ,期间不可操作开关
float spotLightLastToggleTime = 0.0f;//聚光灯上次切换时间


float lightCubeSpeedBoost = 0.7f;//光源移动加速倍数
float lightCubeRoteR = 1.0f;//光源绕Y轴旋转半径



//帧速率控制
float maxFPS = 240;
float delay = 1000/maxFPS;
float frameLastTime = 0;
float framePerSecond = 240;
float frameTime = 0;

//鼠标数据记录相关
float lastX = 0;
float lastY = 0;
bool mouseInit = false;
float mouseSensitivity = 0.05f;

bool mouseUse = false;

GLFWwindow* window;//全局窗口指针


std::string getTimeString() {
	time_t nowtime;
	time(&nowtime); // 获取1970年1月1日0点0分0秒到现在经过的秒数
	tm p;
	localtime_s(&p , &nowtime); // 将秒数转换为本地时间
	//printf("%04d:%02d:%02d %02d:%02d:%02d\n", p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
	return std::to_string(p.tm_year + 1900) + ":" + std::to_string(p.tm_mon + 1) + ":" + std::to_string(p.tm_mday) + " " + std::to_string(p.tm_hour) + ":" + std::to_string(p.tm_min) + ":" + std::to_string(p.tm_sec);
}

void log(std::string str) {
	std::cout << "[" << getTimeString() << "][INFO]" << str << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	screenWidth = width;
	screenHeight = height;
	needUpdateProjection = true;//通知需要更新投影矩阵
	//std::cout << "\r width: " << width << " height : " << height;
	//printf("\nwd : %d , ht : %d" , width , height);
}



//检查OpenGL是否错误的函数
//已弃用
bool checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		log("glError:" + glErr);
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

unsigned int loadTexture(std::string path ) {
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)//灰度图
			format = GL_RED;
		else if (nrComponents == 3)//RGB图
			format = GL_RGB;
		else if (nrComponents == 4)//RGBA图
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);// 生成纹理
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);// 设置纹理环绕方式 此处为重复
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// 设置纹理环绕方式 此处为重复
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);// 设置纹理过滤方式 此处为线性过滤 使用mipmap
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);// 设置纹理过滤方式 此处为线性过滤

		stbi_image_free(data);
	}
	else
	{	

		log("Texture failed to load at path: "+ path);
		stbi_image_free(data);
	}

	return textureID;// 返回纹理ID

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {// 鼠标移动回调函数
	//std::cout << "Mouse move to x : " << xpos << " y : " << ypos << std::endl;

	if (!mouseInit) {// 初始化鼠标位置,防止第一次移动时产生巨大偏移
		lastX = xpos;
		lastY = ypos;
		mouseInit = true;
		return;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // 反转y轴，因为y坐标是从窗口顶部开始的
	lastX = xpos;
	lastY = ypos;

	float yawOffset = xoffset * cameraSensitivity * mouseSensitivity;
	float pitchOffset = yoffset * cameraSensitivity * mouseSensitivity;

	yaw += yawOffset;
	pitch += pitchOffset;

	// 限制俯仰角的范围，防止翻转
	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}
}


/*

void readStringFromFile(const char* index , char* string_index) {
	//std::map<int, std::string> fileContexts;
	std::string fullContent; // 用于存储完整的文件内容
	int lineNumber = 1; // 行号从1开始

	FILE* fp = fopen(index, "r");

	// 读取文件内容
	char buffer[256]; // 缓冲区用于存储每行内容
	while (fgets(buffer, sizeof(buffer), fp) != nullptr) {
		// 将C风格字符串转换为std::string并存入map
		//fileContexts[lineNumber] = std::string(buffer);
		fullContent += buffer;
		lineNumber++;
	}
	strcpy(string_index , fullContent.c_str());


}
*/



 int init(GLFWwindow** ppwindow , int width, int height , std::string str) {


	// 初始化GLFW
	if (!glfwInit()) {
		return -1; // 初始化失败直接退出
	}

	// 配置GLFW上下文
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // 设置OpenGL主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6); // 设置OpenGL次版本号  
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 使用核心模式

	GLFWwindow* window = glfwCreateWindow(width, height, str.c_str() , NULL, NULL);
	if (window == NULL )
	{
		//std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	*ppwindow = window; 

	return 1;

}

int init(GLFWwindow** windex) {
	return init(windex , 800, 600 , "defalut");
}



void processCameraMove() {
	float cameraToTargetDistence = glm::length(cameraPos - cameraTarget);
	//log("Camera distence :" + std::to_string(cameraToTargetDistence));
	//log("camera z : " + std::to_string(cameraPos.z));
	float cameraRealSpeed = cameraSpeed * frameTime;
	float cameraRealSensitivity = cameraSensitivity * frameTime;

	if (isKeyPressing(GLFW_KEY_CAPS_LOCK)) {
		cameraRealSpeed *= cameraSpeedBoost;
		//log("is key CAPS_LOCK pressing , speed boost x" + std::to_string(cameraSpeedBoost));
	}



	//计算摄像机位置
	if (isKeyPressing(GLFW_KEY_W)) {
		cameraPos += cameraRealSpeed * cameraFront;
		log("is key W pressing");
	}
	if (isKeyPressing(GLFW_KEY_S)) {
		cameraPos -= cameraRealSpeed * cameraFront;
		log("is key S pressing");
	}
	if (isKeyPressing(GLFW_KEY_A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraRealSpeed;
		log("is key A pressing");

	}
	if (isKeyPressing(GLFW_KEY_D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraRealSpeed;
		log("is key D pressing");
	}
	if (isKeyPressing(GLFW_KEY_SPACE)) {
		cameraPos += cameraRealSpeed * cameraUp;
		log("is key SPACE pressing");
	}
	if (isKeyPressing(GLFW_KEY_LEFT_SHIFT)) {
		cameraPos += cameraRealSpeed * ( - cameraUp );
		log("is key LEFT_SHIFT pressing");
	}



	// 记录摄像机旋转
	if (isKeyPressing(GLFW_KEY_UP)) {
		pitch += cameraRealSensitivity;
		log("is key UP pressing");
	}
	if (isKeyPressing(GLFW_KEY_DOWN)) {
		pitch -= cameraRealSensitivity;
		log("is key DOWN pressing");
	}
	if (isKeyPressing(GLFW_KEY_LEFT)) {
		yaw -= cameraRealSensitivity;
		log("is key LEFT pressing");
	}
	if (isKeyPressing(GLFW_KEY_RIGHT)) {
		yaw += cameraRealSensitivity;
		log("is key RIGHT pressing");
	}

	if (isKeyPressing(GLFW_KEY_DELETE)) {
		cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
		cameraDirection = glm::normalize(cameraPos - cameraTarget);
		cameraFront = -cameraDirection;
		pitch = 0.0f;
		yaw = -90.0f;
		log("is key DELETE pressing");
	}
		
	// 限制俯仰角的范围，防止翻转
	if (pitch > 89.0f){
		pitch = 89.0f;
	}	
	if(pitch < -89.0f){
		pitch = -89.0f;
	}

	if (isKeyPressing(GLFW_KEY_KP_8)) {
		lightCubeRoteR += 0.01f;
		log("is key KP_8 pressing");
	}

	if (isKeyPressing(GLFW_KEY_KP_2)) {
		lightCubeRoteR -= 0.01f;
		log("is key KP_9 pressing");
	}

	if (isKeyPressing(GLFW_KEY_F) && glfwGetTime()-spotLightLastToggleTime > spotLightCd) {
		spotLightOn = !spotLightOn;
		log("is key F pressing , spotLightOn : " + std::to_string(spotLightOn));
		spotLightLastToggleTime = glfwGetTime();
	}
}


int main(int argc, char* args[]) {

	init(&window , screenWidth, screenHeight , "firstCube");//传入指针window的指针，直接传输window指针，实际上传输的是此指针的副本，不会影响主函数的指针，所以使用传输指针的指针的方法

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		log("Failed to initialize GLAD");
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);




	float vertices[] = {// 附带法线以及纹理坐标的立方体顶点数据
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





	

	



	

	Shader cubeShader("src/Shaders/ModelShader.vs", "src/Shaders/ModelShader.fs");  // 立方体着色器（顶点+片段）
	Shader lightShader("src/Shaders/shader.vs" ,"src/Shaders/lightShader.fs");                     // 光源着色器（片段）	

	
	//unsigned int VAOID;
	//glGenVertexArrays(1, &VAOID);
	//// 1. 绑定VAO
	//glBindVertexArray(VAOID);
	//// 2. 把顶点数组复制到缓冲中供OpenGL使用
	

	////glBindVertexArray(0);//解绑vao，确定当前vbo状态并锁定VAO配置


	////纹理操作
	//unsigned int textureID = loadTexture("src/Textures/container2.png");//材质贴图
	//unsigned int textureSpecularID = loadTexture("src/Textures/container2_specular.png");//高光贴图



	//
	////vbo的最后修改工作

	//	// 3. 设置顶点属性指针
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));//纹理坐标属性
	//glEnableVertexAttribArray(2);

	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//法线属性
	//glEnableVertexAttribArray(3);

	////glVertexAttribPointer(
	////	1,                 // 属性索引1 (颜色)
	////	3,                 // vec3 (r,g,b)
	////	GL_FLOAT,          // 浮点类型
	////	GL_FALSE,          // 不标准化
	////	8 * sizeof(float), // 相同的步长
	////	(void*)(3 * sizeof(float)) // 跳过前3个float(位置)
	////);
	////glEnableVertexAttribArray(1);


	////glVertexAttribPointer(
	////	2,                 // 属性索引(纹理)
	////	2,                 // vec2
	////	GL_FLOAT,          // 浮点类型
	////	GL_FALSE,          // 不标准化
	////	8 * sizeof(float), // 相同的步长
	////	(void*)(6 * sizeof(float)) // 跳过前6个float(位置)
	////);
	////glEnableVertexAttribArray(2);
	//

	//unsigned int indices[] = {
	//	// 前面（每个面由2个三角形组成）
	//		0, 1, 2,   // 第一个三角形
	//		3, 4, 5,   // 第二个三角形（注意：原始数据中第3/4个顶点是重复的）

	//		// 后面
	//		6, 7, 8,
	//		9, 10, 11,

	//		// 左面
	//		12, 13, 14,
	//		15, 16, 17,

	//		// 右面
	//		18, 19, 20,
	//		21, 22, 23,

	//		// 下面
	//		24, 25, 26,
	//		27, 28, 29,

	//		// 上面
	//		30, 31, 32,
	//		33, 34, 35
	//};

	//// 2. 创建EBO
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);





	glEnable(GL_DEPTH_TEST);//开启深度测试 全局

	

	//glBindVertexArray(0);//解绑vao，确定当前vbo状态并锁定VAO配置




	//glm::radians() ->  将角度值转为弧度值
	glm::mat4 model = glm::mat4(1.0f);//初始化单位矩阵
	model = glm::rotate(model, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));//绕z轴旋转90度

	glm::mat4 view = glm::mat4(1.0f);//初始化单位矩阵
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));//沿z轴负方向平移5个单位（相当于移动摄像机到z轴正方向）

	glm::mat4 projection = glm::mat4(1.0f);//初始化单位矩阵
	projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//透视投影矩阵
	//参数1：视野角度（fov） 参数2：宽高比 参数3：近裁剪面 参数4：远裁剪面
	/*  glm::perspective用来生成透视投影矩阵。各个参数的含义如下：
		•	45.0f：视野角度（FOV, Field of View），单位是度。这里是45度，表示摄像机上下能看到的最大夹角。FOV越大，看到的范围越广，但物体会显得更小、更远。
		•(float)screenWidth / (float)screenHeight：宽高比（Aspect Ratio），通常是窗口的宽度除以高度。保证画面不变形。
		•	0.1f：近平面（Near Plane），摄像机前方多近的距离开始渲染。小于这个距离的物体不会被渲染。
		•	100.0f：远平面（Far Plane），摄像机前方多远的距离停止渲染。超过这个距离的物体不会被渲染。
		简单理解：
		•	45.0f：你能看到的“广度”
		•	0.1f：离你最近能看到多近
		•	100.0f：离你最远能看到多远
		这些参数共同决定了你在3D场景中能看到的空间范围。*/

	glm::mat4 transform = glm::mat4(1.0f);//初始化单位矩阵

	unsigned int VBOID;// 存放顶点缓冲对象的id
	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);//通过id启动VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //只需绑定VBO（数据已存在，无需再次上传）


	unsigned int lightVAOID;//存放light vao id
	glGenVertexArrays(1, &lightVAOID);//生成light vao id
	glBindVertexArray(lightVAOID);//绑定light vao


	glBindBuffer(GL_ARRAY_BUFFER, VBOID);//绑定VBO（数据已存在，无需再次上传）

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //只需绑定VBO（数据已存在，无需再次上传）

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(3);
	//光源立方体只需要位置属性即可

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);//绑定EBO（数据已存在，无需再次上传）
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);//传入索引数据

	//// 设置矩阵
	//lightShader.setMat4("model" , model);
	//lightShader.setMat4("view", view);
	//lightShader.setMat4("projection", projection);
	
	glBindVertexArray(0);//解绑light vao
	glBindBuffer(GL_ARRAY_BUFFER, 0);//解绑VBO






	Model ourModel("src/Textures/3D/sofa/Divan_sofa California N090925.obj");
	

	glfwSwapInterval(1);//开启垂直同步

	float rota = 20;
	float scale = 2;
	float scale_forward = 1;

	int verticesCount = sizeof(vertices) / sizeof(int);
	//int eboS = sizeof(indices) / sizeof(int);


	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//隐藏并锁定鼠标
	glfwSetCursorPosCallback(window, mouse_callback);
	while (!glfwWindowShouldClose(window))
	{	
		//glm::vec3 lightColor(1.0f * sin(glfwGetTime()), 1.0f, 1.0f * cos(glfwGetTime()));

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

		cameraFront = glm::normalize(front);//更新相机视线向量

		float frameStartTime = glfwGetTime();

		frameTime = glfwGetTime() - frameLastTime;
		framePerSecond = 1 / frameTime;
		frameLastTime = glfwGetTime();

		//log(std::to_string(framePerSecond) + "  FrameTime : " + std::to_string(frameTime) + " delay :" + std::to_string(delay));
		if (needUpdateProjection) {//在窗口分辨率改变后更新透视矩阵
			projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//透视投影矩阵
			needUpdateProjection = false;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区


		glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // 设置清屏颜色
		glClear(GL_COLOR_BUFFER_BIT);          // 清除颜色缓冲区




		//glUseProgram(shaderProgram);//激活程序

		cubeShader.use();//激活程序
		
		


		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//视图矩阵(摄像机位置，目标位置，摄像机上向量)

		//glUniform1i(isLightLoc, 0);//设置是否为光源对象,此处默认为否0

		model = glm::mat4(1.0f);//初始化单位矩阵

		model = glm::translate(model,  glm::vec3(-7.5f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));//绕z轴旋转
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));//绕y轴旋转
		//model = glm::scale(model, glm::vec3(20.0F));//缩放
		// 设置矩阵
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("transform", transform);
		//cubeShader.setVec3("color_offset", glm::vec3(0.9f, 0.2f, 0.0f));//设置颜色偏移量(由于传入的顶点数据不含颜色数据，所以此项决定了物体颜色)


		cubeShader.setVec3("viewPos", cameraPos);//设置观察位置
		//cubeShader.setVec3("lightColor", glm::vec3(1.0f));//设置光源颜色

		//由于物体（material）本身不发光，其颜色完全依赖于光照，因此物体颜色通常通过材质属性来定义，而不是直接设置一个固定的颜色值。
		//cubeShader.setVec3("material.ambientReflectance", 1.0f, 0.5f, 0.31f);//设置主立方体材质属性 环境光反射系数（物体对环境光的反射能力）

		


		//cubeShader.setInt("material.diffuseTextureID", 0);//设置漫反射贴图采样器为纹理单元0
		//cubeShader.setInt("material.specularTextureID", 1);//设置高光贴图采样器为纹理单元1
		////cubeShader.setVec3("material.diffuseReflectance", 1.0f, 0.5f, 0.31f);//设置主立方体材质属性 漫反射系数（物体对直射光的颜色和强度）决定物体的主色调（如红色物体吸收蓝绿光，反射红光）。
		////cubeShader.setVec3("material.specularReflectance", 0.5f, 0.5f, 0.5f);//设置主立方体材质属性 镜面反射系数（高光的颜色和强度） 通常与物体材质的真实颜色无关（如金属的高光颜色接近金属本身，塑料的高光多为白色）。
		//cubeShader.setFloat("material.shininess", 16.0f);//设置主立方体材质属性 高光锐度（控制高光的集中程度，值越大越尖锐）


		cubeShader.setVec3("pointlight[0].position", lightPos);// 设置光源位置
		cubeShader.setVec3("pointlight[0].ambient", 0.3f, 0.3f, 0.3f);// 设置光源的环境强度
		cubeShader.setVec3("pointlight[0].diffuse", 0.5f, 0.5f, 0.5f); // 设置光源的漫反射强度
		cubeShader.setVec3("pointlight[0].specular", 1.0f, 1.0f, 1.0f);// 设置光源的镜面反射强度
		cubeShader.setInt("pointlight[0].enabled", 1);//启用点光源0
		//常数设置阶段
		cubeShader.setFloat("pointlight[0].constant", 1.0f);// 设置光源的衰减常数项
		cubeShader.setFloat("pointlight[0].linear", 0.09f);// 设置光源的线性衰减项
		cubeShader.setFloat("pointlight[0].quadratic", 0.032f);// 设置光源的二次衰减项 
		//此处的衰减系数为常用值，覆盖了50个单位的有效光照范围



		cubeShader.setVec3("dirlight.direction", defaultDirectionalLightDir);//设置方向光方向
		cubeShader.setVec3("dirlight.ambient", 0.2f, 0.2f, 0.2f);// 设置光源的环境强度
		cubeShader.setVec3("dirlight.diffuse", 0.5f, 0.5f, 0.5f); // 设置光源的漫反射强度

		if (spotLightOn) {//开启聚光灯

			cubeShader.setVec3("spotlight.position", cameraPos);// 设置光源位置
			cubeShader.setVec3("spotlight.direction", cameraFront);// 设置光源方向

			cubeShader.setFloat("spotlight.cutOff", defaultSpotLightCutOff);// 设置光源的内切角
			cubeShader.setFloat("spotlight.outerCutOff", defaultSpotLightOuterCutOff);// 设置光源的外切角

			cubeShader.setVec3("spotlight.ambient", 0.3f, 0.3f, 0.3f);// 设置光源的环境强度
			cubeShader.setVec3("spotlight.diffuse", 0.7f, 0.7f, 0.7f); // 设置光源的漫反射强度
			cubeShader.setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);// 设置光源的镜面反射强度
			cubeShader.setInt("spotlight.enabled", 1);//启用聚光灯
			//常数设置阶段
			cubeShader.setFloat("spotlight.constant", 1.0f);// 设置光源的衰减常数项
			cubeShader.setFloat("spotlight.linear", 0.09f);// 设置光源的线性衰减项
			cubeShader.setFloat("spotlight.quadratic", 0.032f);// 设置光源的二次衰减项 
		}
		else {
			cubeShader.setInt("spotlight.enabled", 0);//关闭聚光灯
		}
		
		ourModel.Draw(cubeShader);


		//glDrawArrays(GL_TRIANGLES, 0, 3);

		
		// 绑定纹理
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureID);

		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, textureSpecularID);


		// 绑定VAO（解锁VAO配置，并从中读取所有顶点属性配置）
		//opengl的core模式强制要求使用vao，若绘制时vao绑定失败或未绑定会拒绝绘制任何内容
		//glBindVertexArray(VAOID);



		// 绘制三角形
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesCount); // 从0开始，绘制3个顶点

		//绘制材质三角形
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//for (unsigned int i = 0; i < 10; i++)
		//{
		//	glm::mat4 model = glm::mat4(1.0f);
		//	model = glm::translate(model, cubePositions[i]);
		//	float angle = 20.0f * i;
		//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//	cubeShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);

		//	//glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量
		//}

		//glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量

		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, verticesCount);
		
		//checkOpenGLError();

		lightShader.use();//激活light程序
		
		

		model = glm::mat4(1.0f);//初始化单位矩阵
		model = glm::translate(model, lightPos);//平移到light位置
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));//缩小
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));//更新light的transform矩阵
		
		// 设置矩阵
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		lightShader.setVec3("color_offset", glm::vec3(1.0f));//设置颜色偏移量(由于传入的顶点数据不含颜色数据，所以此项决定了物体颜色)
		

		lightPos = glm::vec3(lightDefaultPos.x * sin(glfwGetTime()*lightCubeSpeedBoost) * lightCubeRoteR, lightDefaultPos.y, lightDefaultPos.z * cos(glfwGetTime() * lightCubeSpeedBoost )*lightCubeRoteR );//让光源围绕原点旋转

		
		lightShader.setMat4("transform" , transform);//暂无用处的transform矩阵
		//checkOpenGLError();
		
		//glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量
		glBindVertexArray(lightVAOID);//绑定light vao
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);//解绑 
		
		//float renderTime = glfwGetTime() - frameStartTime;
		//Sleep(delay - renderTime * 1000);//延时，控制帧率

		processInput(window);
		processCameraMove();

		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

	/*
	printf("argc : %d \n" , argc);

	printf("return: %d \n", adm());
	*/

	glfwTerminate();
	//system("pause");
	
	return 1;

}