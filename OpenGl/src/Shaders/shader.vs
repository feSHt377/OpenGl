//顶点着色器
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;//顶点颜色
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;//法线向量

uniform vec3 Pos_offset; //统一变量，记录顶点偏移
uniform vec3 color_offset; //统一变量，记录颜色偏移'


uniform mat4 transform;//变换矩阵
uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵​
uniform mat4 projection;//投影矩阵








out vec3 objectColor;
out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;//片段坐标


void main()
{

    
    gl_Position = projection * view * model * transform * vec4(aPos.x + Pos_offset.x, aPos.y + Pos_offset.y , aPos.z + Pos_offset.z, 1.0);
    // gl_Position = vec4(aPos, 1.0);
    // aColor = vec3(0.4,0.4,0.4);
    // if(aColor == vec3(0.0)){
    //     ourColor = vec3(1.0);
    // }else{
    //     ourColor = aColor + color_offset;
    // }
    objectColor = aColor + color_offset;
    TexCoord = aTexCoord;
    normal = aNormal;
    fragPos = vec3( model * vec4(aPos, 1.0) );
    
}
