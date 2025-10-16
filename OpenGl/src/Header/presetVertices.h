#pragma once


float cubeVertices[] = {
	// positions       
	-1.0f, -1.0f, -1.0f,  
	 1.0f, -1.0f, -1.0f,  
	 1.0f,  1.0f, -1.0f,  
	 1.0f,  1.0f, -1.0f, 
	-1.0f,  1.0f, -1.0f, 
	-1.0f, -1.0f, -1.0f,  
	-1.0f, -1.0f,  1.0f, 
	 1.0f, -1.0f,  1.0f, 
	 1.0f,  1.0f,  1.0f
};

float cubeVerticesWithColor[] = {
	// positions         // colors
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-1.0f,  1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	 1.0f, -1.0f,  1.0f,  0.0f, 0.0f, 0.0f,
	 1.0f,  1.0f,  1.0f,  0.0f, 0.0f, 0.0f
};


float* getVertices(int& size, bool withColor = false) {
	if (withColor) {
		size = sizeof(cubeVerticesWithColor) / sizeof(float);
		return cubeVerticesWithColor;
	}
	else {
		size = sizeof(cubeVertices) / sizeof(float);
		return cubeVertices;
	}
}

