#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 fragPos;
out vec3 normal;

uniform mat4 transform;//变换矩阵
uniform mat4 model;//模型矩阵
uniform mat4 view;//观察矩阵​
uniform mat4 projection;//投影矩阵

void main()
{
    TexCoord = aTexCoords;
    normal = aNormal;//mat3(transpose(inverse(model))) * aNormal;

    
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);

    fragPos = vec3( model * vec4(aPos, 1.0) );
 
}