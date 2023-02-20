#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in mat4 color;
in vec3 normal;
in vec3 fragPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{

    float ambientStrength = 0.3;
    float specularStrength = 0.1f;
    vec3 objectColor = vec3(color[0][0],color[0][1],color[0][2]);

    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos- fragPos); 

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 
    
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);


    //FragColor = vec4(color[0][0],color[0][1],color[0][2],1.0f);
}