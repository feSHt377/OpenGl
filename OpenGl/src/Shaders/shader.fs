#version 460 core

out vec4 FragColor;

//片段着色器（像素着色器）
// uniform vec4 inputColor; 
in vec3 objectColor;//物体本色





vec3 lightColor = vec3(1.0);
float ambientStrength = 1.0;//环境光照强度

void main()
{

    //FragColor = inputColor; //= vec4(1.0f, 0.5f, 0.2f, 1.0f);
    
    

    vec3 ambient = ambientStrength * lightColor;//环境光

    vec3 finalObjectColor = ambient * objectColor;//最终计算得此像素的颜色

    FragColor = vec4(finalObjectColor, 1.0);
    
    
} 