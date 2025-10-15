#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <system_error> // 用于获取系统错误信息

//#define arrLength(arr) (sizeof(arr) / sizeof(arr[0]))



void readStringFromFile(const char* filename, std::string* str) {
	std::ifstream file(filename);


	// 获取系统错误信息
	std::error_code ec(errno, std::generic_category());


	if (!file.is_open()) {
		std::cerr << "无法打开文件: " << filename 
		<< "\n错误信息: " << ec.message() << std::endl;
		return;
	}

	// 读取文件内容到 std::string
	*str = std::string((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	file.close();

}



