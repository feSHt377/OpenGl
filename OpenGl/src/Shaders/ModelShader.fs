#version 460 core

#define NR_POINT_LIGHTS 4//点光源数量

out vec4 FragColor;//输出片段颜色
//片段着色器（像素着色器） 加载材质和光源结构体
in vec3 objectColor;//物体本色
in vec3 normal;
in vec3 fragPos;//片段坐标 **
in vec2 TexCoord;




uniform sampler2D texture_diffuse1;

uniform sampler2D texture_specular1;

uniform sampler2D texture_normal1;

uniform sampler2D texture_height1;

// uniform vec3 material_ambient1;

struct Material {
    sampler2D diffuseTextureID;   //漫反射贴图id
    sampler2D specularTextureID;  //镜面反射贴图id
    float shininess;           // 高光锐度（控制高光的集中程度，值越大越尖锐）
};

struct pointLight {
    vec3 position;//光源位置

    vec3 ambient;//环境光照强度
    vec3 diffuse;//漫反射强度
    vec3 specular;//镜面反射强度

    float constant;//常量衰减系数
    float linear;//线性衰减系数
    float quadratic;//二次衰减系数

    int enabled;//1为启用
};//光源结构体

struct dirLight{
    vec3 direction; //光源方向
    vec3 ambient; //环境光照强度
    vec3 diffuse; //漫反射强度
    vec3 specular; //镜面反射强度
};//方向（平行）光结构体

struct spotLight {
    vec3 position; //光源位置
    vec3 direction; //光源方向
    float cutOff; //聚光灯的内切角
    float outerCutOff; //聚光灯的外切角

    vec3 ambient; //环境光照强度
    vec3 diffuse; //漫反射强度
    vec3 specular; //镜面反射强度
    float constant; //常量衰减系数
    float linear; //线性衰减系数
    float quadratic; //二次衰减系数

    int enabled; //1为启用
};//聚光灯结构体


uniform vec3 viewPos;//观察坐标（摄像机坐标） 由cpu端直接传输


uniform Material material;//材质结构体
uniform pointLight pointlight[NR_POINT_LIGHTS];//点光源结构体
uniform dirLight dirlight;//方向（平行）光结构体
uniform spotLight spotlight;//聚光灯结构体


vec3 CalcDirLight(dirLight light, vec3 normal, vec3 viewDir);//方向(平行)光计算
vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//点光源计算
vec3 CalcSpotLight(spotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);//聚光灯计算

void main()
{

    vec3 norm = normalize(normal);//法线向量标准化

    vec3 viewDir = normalize(viewPos - fragPos);//标准化 从片段到摄像机的向量
    // vec3 reflectDir = reflect(-lightDir, norm);

    
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//计算高光
    // float diff = max(dot(norm, lightDir), 0.0);// 漫反射强度


    // vec3 ambient = light.ambient * texture(material.diffuseTextureID, TexCoord).rgb;//环境光 

    // vec3 specular = light.specular * spec * texture(material.specularTextureID ,TexCoord).rgb ;// 镜面反射分量
    // vec3 diffuse = light.diffuse * diff * texture(material.diffuseTextureID, TexCoord).rgb;// 漫反射分量




    // vec3 finalObjectColor = (diffuse + ambient + specular) ;//* objectColor;//最终颜色
    // 第一阶段：定向光照
    // vec3 result = CalcDirLight(dirlight, norm, viewDir);
    vec3 result = vec3(0.0);

    // 第二阶段：点光源
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointlight[i].enabled == 1) {
            result += CalcPointLight(pointlight[i], norm, fragPos, viewDir); 
        }   
    }
    // 第三阶段：聚光
    if (spotlight.enabled == 1){//必须被启用
        result += CalcSpotLight(spotlight, norm, fragPos, viewDir);    
    }

    FragColor = vec4(result, 1.0);//输出最终片段颜色

    // FragColor =  texture(texture_diffuse1, TexCoord);

} 


vec3 CalcDirLight(dirLight light, vec3 normal, vec3 viewDir){//方向(平行)光计算
    vec3 lightDir = normalize(-light.direction);////标准化 从光源到片段的向量（特殊，因为通常将片段到光源视为“方向Direction”,此处因为是平行光，更习惯使用光源到片段的方向向量）
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);//反射光线方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoord));//环境光
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoord));//漫反射
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));//镜面反射
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){//点光源计算
    vec3 lightDir = normalize(light.position - fragPos);
    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);
    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);//反射光线方向
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);//计算高光
    // 合并结果
    vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoord));//环境光
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoord));//漫反射
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));//镜面反射

    float distance    = length(light.position - fragPos);//计算片段到光源的距离
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);//衰减系数

    ambient *= attenuation;//环境光衰减
    diffuse *= attenuation;//漫反射衰减
    specular *= attenuation;//镜面反射衰减

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(spotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){//聚光灯计算

    vec3 result = vec3(0.0);
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));//计算光线方向与聚光灯方向的夹角cos值

    float cutOff = cos(radians(light.cutOff));
    float outerCutOff = cos(radians(light.outerCutOff));
    if(theta > outerCutOff ) {//光线在聚光灯范围内 cos值越大角越小，theta只有在小于cutOff
    //函数内联判断聚光灯范围
        // 漫反射着色
        float diff = max(dot(normal, lightDir), 0.0);
        // 镜面光着色
        vec3 reflectDir = reflect(-lightDir, normal);//反射光线方向
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        // 合并结果
        vec3 ambient  = light.ambient  * vec3(texture(texture_diffuse1, TexCoord));//环境光
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(texture_diffuse1, TexCoord));//漫反射
        vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoord));//镜面反射

        float distance    = length(light.position - fragPos);//计算片段到光源的距离
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);//衰减系数

        float epsilon   = cutOff - outerCutOff;//过渡角度段
        float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0); // clamp函数，它把第一个参数约束(Clamp)在了0.0到1.0之间。这保证强度值不会在[0, 1]区间之外。

        ambient *= attenuation ;//环境光衰减
        diffuse *= attenuation * intensity;//漫反射衰减
        specular *= attenuation * intensity;//镜面反射衰减

        // ambient *=  intensity;//环境光衰减
        // diffuse *=  intensity;//漫反射衰减
        // specular *= intensity;//镜面反射衰减

        result = (ambient + diffuse + specular) * intensity;
    }

    return result;
}


