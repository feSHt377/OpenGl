#version 460 core
out vec4 FragColor;

//片段着色器（像素着色器）
// uniform vec4 inputColor; 
in vec3 ourColor;

void main()
{
    //FragColor = inputColor; //= vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = vec4(ourColor, 1.0);
} 