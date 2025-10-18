#version 460 core

out vec4 FragColor;

//片段着色器（像素着色器）
// uniform vec4 inputColor; 
in vec3 objectColor;
in vec3 normal;

void main()
{
    // FragColor = vec4(1.0);//always white
    FragColor = vec4(objectColor, 1.0);
} 