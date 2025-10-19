#version 460 core

out vec4 FragColor;

//片段着色器（像素着色器） 加载材质和光源结构体
// uniform vec4 inputColor; 
in vec3 objectColor;//物体本色
in vec3 normal;
in vec3 fragPos;//片段坐标 **
in vec2 TexCoord;

uniform vec3 viewPos;//观察坐标（摄像机坐标） 由cpu端直接传输
// uniform vec3 lightPos;//输入光源坐标 ** 由cpu端直接传输
// uniform vec3 lightColor;//光源颜色  由cpu端直接传输


// vec3 lightColor = vec3(1.0);
// float ambientStrength = 0.1;//环境光照强度
// float specularStrength = 0.5;//设置高光强度（镜面反射强度）
// int shininess = 32;//这个32是高光的反光度(Shininess)。一个物体的反光度越高，反射光的能力越强，散射得越少，高光点就会越小。在下面的图片里，你会看到不同反光度的视觉效果影响：

struct Material {
    sampler2D diffuseTextureID;   //漫反射贴图id
    sampler2D specularTextureID;  //镜面反射贴图id
    float shininess;           // 高光锐度（控制高光的集中程度，值越大越尖锐）
};

struct Light {
    vec3 position;//光源位置

    vec3 ambient;//环境光照强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//镜面反射强度
};//光源结构体



uniform Material material;//材质结构体

uniform Light light;//光源结构体

void main()
{
    vec3 norm = normalize(normal);//法线向量标准化
    vec3 lightDir = normalize(light.position - fragPos);//标准化 从片段到光源的向量

    vec3 viewDir = normalize(viewPos - fragPos);//标准化 从片段到摄像机的向量
    vec3 reflectDir = reflect(-lightDir, norm);

    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//计算高光
    float diff = max(dot(norm, lightDir), 0.0);// 漫反射强度

    vec3 ambient = light.ambient * texture(material.diffuseTextureID, TexCoord).rgb;//环境光 
    vec3 specular = light.specular * spec * texture(material.specularTextureID ,TexCoord).rgb ;// 镜面反射分量
    vec3 diffuse = light.diffuse * diff * texture(material.diffuseTextureID, TexCoord).rgb;// 漫反射分量

    
    vec3 finalObjectColor = (diffuse + ambient + specular) ;//* objectColor;//最终颜色

    FragColor = vec4(finalObjectColor, 1.0);//输出最终片段颜色

} 
