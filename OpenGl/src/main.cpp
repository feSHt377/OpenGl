
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

#include "Header/file.h"
#define STB_IMAGE_IMPLEMENTATION
#include "Header/stb_image.h"
#include "Header/kbEvent.h"

//#include "Header/openGl.h"s

int screenWidth = 1280;
int screenHeight = 1024;

bool needUpdateProjection = true;//是否需要更新投影矩阵


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);//摄像机位置
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

float cameraSpeed = 0.05f;
float cameraSensitivity = 0.1f;

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
	std::cout << "\r width: " << width << " height : " << height;
	//printf("\nwd : %d , ht : %d" , width , height);
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


void processCameraMove() {
	float cameraToTargetDistence = glm::length(cameraPos - cameraTarget);
	log("Camera distence :"+std::to_string(cameraToTargetDistence));
	log("camera z : "+std::to_string(cameraPos.z));
	//计算摄像机位置
	if (isKeyPressing(GLFW_KEY_W)) {
		cameraPos += cameraSpeed * cameraFront;
		log("is key W pressing");
	}
	if (isKeyPressing(GLFW_KEY_S)) {
		cameraPos -= cameraSpeed * cameraFront;
		log("is key S pressing");
	}
	if (isKeyPressing(GLFW_KEY_A)) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		log("is key A pressing");
		
	}
	if (isKeyPressing(GLFW_KEY_D)) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		log("is key D pressing");
	}

	// 记录摄像机旋转
	if (isKeyPressing(GLFW_KEY_UP)) {
		pitch += cameraSensitivity;
		log("is key UP pressing");
	}
	if (isKeyPressing(GLFW_KEY_DOWN)) {
		pitch -= cameraSensitivity;
		log("is key DOWN pressing");
	}
	if (isKeyPressing(GLFW_KEY_LEFT)) {
		yaw -= cameraSensitivity;
		log("is key LEFT pressing");
	}
	if (isKeyPressing(GLFW_KEY_RIGHT)) {
		yaw += cameraSensitivity;
		log("is key RIGHT pressing");
	}
}


int main(int argc, char* args[]) {

	

	GLFWwindow* window ;
	init(&window , screenWidth, screenHeight , "firstCube");//传入指针window的指针，直接传输window指针，实际上传输的是此指针的副本，不会影响主函数的指针，所以使用传输指针的指针的方法

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		log("Failed to initialize GLAD");
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	//float vertices[] = {
	//	// 位置              // 颜色            // 纹理坐标
	//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
	//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
	//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
	//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	//};

	float vertices[] = {
		// 位置              // 颜色              // 纹理坐标
			// 前面 (红色面)
		-1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // 左下前 0
		1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // 右下前 1
		1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // 右上前 2
		-1.0f,  1.0f,  1.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // 左上前 3

		// 后面 (绿色面)
		-1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 左下后 4
		1.0f, -1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 右下后 5
		1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 右上后 6
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 左上后 7

		// 左面 (蓝色面)
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 左下后 8
		-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // 左下前 9
		-1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 左上前 10
		-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // 左上后 11

		// 右面 (黄色面)
		1.0f, -1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // 右下前 12
		1.0f, -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // 右下后 13
		1.0f,  1.0f, -1.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // 右上后 14
		1.0f,  1.0f,  1.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // 右上前 15

		// 上面 (青色面)
		-1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,  // 左上前 16
		1.0f,  1.0f,  1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // 右上前 17
		1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f,  // 右上后 18
		-1.0f,  1.0f, -1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f,  // 左上后 19

		// 下面 (紫色面)
		-1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // 左下后 20
		1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // 右下后 21
		1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // 右下前 22
		-1.0f, -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f   // 左下前 23
	};

	unsigned int VBOID;// 存放顶点缓冲对象的id
	glGenBuffers(1, &VBOID);
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);//通过id启动VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //传入顶点数据



	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);//创建顶点着色器id



	std::string source;
	readStringFromFile("G:/Docs/Visual Studio 2022/Projects/OpenGl/OpenGl/src/Shaders/shader.vs", &source);
	const char* vertexShaderSource = source.c_str();

	//                 着色器id  源码数量  源码字符串地址   未知
	glShaderSource(vertexShaderID, 1, &vertexShaderSource, NULL);//传入顶点着色器源码 
	glCompileShader(vertexShaderID);//开始编译


	//readStringFromFile("G:/Docs/Visual Studio 2022/Projects/OpenGl/OpenGl/src/Shaders/shader.fs", &source);
	readStringFromFile("G:/Docs/Visual Studio 2022/Projects/OpenGl/OpenGl/src/Shaders/TextureShader.fs", &source);
	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);//创建片段着色器id

	const char* fragmentShaderSource = source.c_str();
	//                 着色器id  源码数量  源码字符串地址   未知
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, NULL);//传入片段着色器源码 
	glCompileShader(fragmentShaderID);//开始编译

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();




	glAttachShader(shaderProgram, vertexShaderID);// 将顶点着色器附加到着色器程序
	glAttachShader(shaderProgram, fragmentShaderID);// 将片段着色器附加到着色器程序
	glLinkProgram(shaderProgram);// 链接所有附加的着色器，生成最终可执行程序




	unsigned int VAOID;
	glGenVertexArrays(1, &VAOID);
	// 1. 绑定VAO
	glBindVertexArray(VAOID);
	// 2. 把顶点数组复制到缓冲中供OpenGL使用
	glBindBuffer(GL_ARRAY_BUFFER, VBOID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 只需绑定VBO（数据已存在，无需再次上传）
	



	//glBindVertexArray(0);//解绑vao，确定当前vbo状态并锁定VAO配置


	
	unsigned int textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);//需翻转 Y 轴（OpenGL纹理坐标系与图片默认方向相反）!!!!!!!!!!!!
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char* data = stbi_load("G:/Docs/Visual Studio 2022/Projects/OpenGl/OpenGl/src/Textures/test.png", &width, &height, &nrChannels, 0);

	if (data) {
		/*
		void glTexImage2D(
			GLenum target,       // 1. 纹理目标（绑定的是哪种纹理）
			GLint level,         // 2. Mipmap 级别（当前上传的是第几层纹理）
			GLint internalFormat,// 3. GPU 内部存储的纹理格式（如何压缩/存储像素）
			GLsizei width,       // 4. 纹理的像素宽度
			GLsizei height,      // 5. 纹理的像素高度
			GLint border,        // 6. 纹理边框宽度（现代OpenGL固定为0）​​现代OpenGL中必须传0​​，因为边框功能已被废弃。
			GLenum format,       // 7. 上传数据的“通道布局”（原始数据的格式）
			GLenum type,         // 8. 上传数据的“像素类型”（每个通道的字节大小）
			const void *data     // 9. 原始像素数据的指针（比如stbi_load返回的数组）
		);
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		log("Texture has been loaded.");
	}
	else
	{
		log("Failed to load texture!!");
	}

	stbi_image_free(data);//free

	glDeleteShader(vertexShaderID);//free
	glDeleteShader(fragmentShaderID);//free

	
	//vbo的最后修改工作

		// 3. 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,                 // 属性索引1 (颜色)
		3,                 // vec3 (r,g,b)
		GL_FLOAT,          // 浮点类型
		GL_FALSE,          // 不标准化
		8 * sizeof(float), // 相同的步长
		(void*)(3 * sizeof(float)) // 跳过前3个float(位置)
	);
	glEnableVertexAttribArray(1);


	glVertexAttribPointer(
		2,                 // 属性索引
		2,                 // vec2
		GL_FLOAT,          // 浮点类型
		GL_FALSE,          // 不标准化
		8 * sizeof(float), // 相同的步长
		(void*)(6 * sizeof(float)) // 跳过前6个float(位置)
	);
	glEnableVertexAttribArray(2);


	/*
	float vertices[] = {
		// 位置              // 颜色            // 纹理坐标
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 左下
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
		-0.3f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // 左上（稍微右移形成平行四边形）
		 0.7f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f   // 右上（稍微右移形成平行四边形）
	};
	*/


	// 2. 创建EBO
	unsigned int indices[] = {
		// 前面 (红色)
		0, 1, 2,   // 第一个三角形
		2, 3, 0,    // 第二个三角形

		// 后面 (绿色)
		4, 5, 6,   // 第一个三角形
		6, 7, 4,    // 第二个三角形

		// 左面 (蓝色)
		8, 9, 10,  // 第一个三角形
		10, 11, 8,  // 第二个三角形

		// 右面 (黄色)
		12, 13, 14, // 第一个三角形
		14, 15, 12, // 第二个三角形

		// 上面 (青色)
		16, 17, 18, // 第一个三角形
		18, 19, 16, // 第二个三角形

		// 下面 (紫色)
		20, 21, 22, // 第一个三角形
		22, 23, 20  // 第二个三角形
	};

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	//glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);





	glEnable(GL_DEPTH_TEST);//开启深度测试

	glBindVertexArray(0);//解绑vao，确定当前vbo状态并锁定VAO配置

	unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");//获取model矩阵位置
	unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");//获取view矩阵位置
	unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");//获取projection矩阵位置
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");//获取transform矩阵位置



	//glm::radians() ->  将角度值转为弧度值
	glm::mat4 model = glm::mat4(1.0f);//初始化单位矩阵
	model = glm::rotate(model, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));//绕y轴旋转-55度

	glm::mat4 view = glm::mat4(1.0f);//初始化单位矩阵
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));//沿z轴负方向平移5个单位（相当于移动摄像机到z轴正方向）

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




	//char* str = (char*)malloc(1);
	//int mem = 0;


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
	};//多个立方体位置


	float rota = 20;
	float scale = 2;
	float scale_forward = 1;

	int verticesCount = sizeof(vertices) / sizeof(int);
	int eboS = sizeof(indices) / sizeof(int);

	//帧速率控制
	float maxFPS = 240;
	float delay = 1000/maxFPS;
	float frameLastTime = 0;
	float framePerSecond = 0;
	float frameTime = 0;
	while (!glfwWindowShouldClose(window))
	{	
		frameTime = glfwGetTime() - frameLastTime;
		framePerSecond = 1 / frameTime;
		frameLastTime = glfwGetTime();

		//log("is key press:" + std::to_string(isKeyPressing(GLFW_KEY_W)) );

		/*if (framePerSecond > maxFPS) {
			delay = (1000 / maxFPS - frameTime * 1000)/2;
		}*/

		//log(std::to_string(framePerSecond) + "  FrameTime : " + std::to_string(frameTime) + " delay :" + std::to_string(delay));
		if (needUpdateProjection) {
			projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);//透视投影矩阵
			needUpdateProjection = false;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色缓冲区和深度缓冲区

		Sleep(delay);//延时，控制帧率
		//if (mem <= 1024 * 1024 * 1024) {
		//	mem += 1024 * 1024 * 10;
		//	str = (char*)realloc(str , mem);
		//	//memset(str, 1, sizeof(str)-1);
		//	log(std::to_string(mem));
		//	log("try to get 200mb mem");
		//}

		// 清屏（重要！）
		glClearColor(0.0f, 0.5f, 0.0f, 1.0f); // 设置清屏颜色
		glClear(GL_COLOR_BUFFER_BIT);          // 清除颜色缓冲区


		glUseProgram(shaderProgram);//激活程序
		


		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);//视图矩阵(摄像机位置，目标位置，摄像机上向量)


		// 设置矩阵
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		float timeValue = glfwGetTime();

		float timeToSin = sin(timeValue);


		int vertexColorLocation = glGetUniformLocation(shaderProgram, "color_offset");
		//glUniform3f(vertexColorLocation , sin(timeValue), sin(timeValue +90) , sin(timeValue + 180));

		int vertexPosOffset = glGetUniformLocation(shaderProgram, "Pos_offset"); 
		//glUniform3f(vertexPosOffset, 0.1f, 0.1f, 0.1f);
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 1.0f - greenValue, 1.0f);


		if (scale >= 1) {
			scale_forward = -1;
		}
		if(scale <= 0.3f ){
			scale_forward = 1;
		}
		//scale += scale_forward * 0.01f;
		glm::mat4 trans = glm::mat4(1.0f);
		//旋转角为 rota 第二个参数为旋转轴，绕y轴旋转
		//trans = glm::rotate(trans, (float)glfwGetTime() * glm::radians(rota), glm::vec3(0.0, 1.0, 0.0));//每次渲染都绕y轴旋转
		
		trans = glm::rotate(trans, glm::radians(20.0f), glm::vec3(1.0, 0.0, 0.0));//绕x轴旋转20度，固定

		trans = glm::scale(trans, glm::vec3(scale, scale, scale));//缩放




		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));



		//glDrawArrays(GL_TRIANGLES, 0, 3);

		
		// 绑定纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);


		// 绑定VAO（解锁VAO配置，并从中读取所有顶点属性配置）
		//opengl的core模式强制要求使用vao，若绘制时vao绑定失败或未绑定会拒绝绘制任何内容
		glBindVertexArray(VAOID);

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
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//	glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量
		//}

		//glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量

		
		
		glDrawElements(GL_TRIANGLES, eboS, GL_UNSIGNED_INT, 0); //eboS为索引数量

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