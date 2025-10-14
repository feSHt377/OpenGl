
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform vec3 Pos_offset; //统一变量，记录顶点偏移
uniform vec3 color_offset; //统一变量，记录颜色偏移

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos.x + Pos_offset.x, aPos.y + Pos_offset.y , aPos.z + Pos_offset.z, 1.0);
    // gl_Position = vec4(aPos, 1.0);
    ourColor = aColor + color_offset;
    TexCoord = aTexCoord;
}
