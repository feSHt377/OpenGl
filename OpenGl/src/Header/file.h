#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <system_error> // ���ڻ�ȡϵͳ������Ϣ

//#define arrLength(arr) (sizeof(arr) / sizeof(arr[0]))



void readStringFromFile(const char* filename, std::string* str) {
	std::ifstream file(filename);


	// ��ȡϵͳ������Ϣ
	std::error_code ec(errno, std::generic_category());


	if (!file.is_open()) {
		std::cerr << "�޷����ļ�: " << filename 
		<< "\n������Ϣ: " << ec.message() << std::endl;
		return;
	}

	// ��ȡ�ļ����ݵ� std::string
	*str = std::string((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	file.close();

}



