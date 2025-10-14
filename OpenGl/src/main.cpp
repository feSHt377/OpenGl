
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

//#include "Header/openGl.h"s


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
}

/*
int checkKeyStatus(int key_enum) {
	return keyStatus[key_enum];
}
*/


int isKeyPressing(int key_enum) {
	if (keyStatus[key_enum] == GLFW_PRESS) {
		return 1;
	}
	else {
		return -1;
	}

}


int main(int argc, char* args[]) {


	GLFWwindow* window ;
	init(&window);//传入指针window的指针，直接传输window指针，实际上传输的是此指针的副本，不会影响主函数的指针，所以使用传输指针的指针的方法

	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		log("Failed to initialize GLAD");
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	float vertices[] = {
		// 位置              // 颜色            // 纹理坐标
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // 左下
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f  // 左上（稍微右移形成平行四边形）
		 //0.7f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f   // 右上（稍微右移形成平行四边形）
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
	unsigned char* data = stbi_load("G:/Docs/Visual Studio 2022/Projects/OpenGl/OpenGl/src/Textures/sample.jpg", &width, &height, &nrChannels, 0);

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


	//// 2. 创建EBO
	//unsigned int indices[] = {
	//	0, 1, 2,  // 第一个三角形
	//	1, 3, 2   // 第二个三角形
	//};

	//unsigned int EBO;
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);




	glBindVertexArray(0);//解绑vao，确定当前vbo状态并锁定VAO配置

	

	
	
	
	//char* str = (char*)malloc(1);
	//int mem = 0;
	while (!glfwWindowShouldClose(window))
	{
		//Sleep(100);
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


		glUseProgram(shaderProgram);


		float timeValue = glfwGetTime();

		float timeToSin = sin(timeValue);


		int vertexColorLocation = glGetUniformLocation(shaderProgram, "color_offset");
		glUniform3f(vertexColorLocation , sin(timeValue), sin(timeValue +90) , sin(timeValue + 180));

		int vertexPosOffset = glGetUniformLocation(shaderProgram, "color_offset"); 
		glUniform3f(vertexPosOffset, 0.1f, 0.1f, 0.1f);
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 1.0f - greenValue, 1.0f);


		//glDrawArrays(GL_TRIANGLES, 0, 3);


		// 绑定纹理
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, textureID);


		// 绑定VAO（解锁VAO配置，并从中读取所有顶点属性配置）
		//opengl的core模式强制要求使用vao，若绘制时vao绑定失败或未绑定会拒绝绘制任何内容
		glBindVertexArray(VAOID);

		// 绘制三角形
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 3); // 从0开始，绘制3个顶点

		//绘制材质三角形
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		processInput(window);
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