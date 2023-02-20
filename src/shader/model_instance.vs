#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;
layout (location = 7) in mat4 aColor;

out vec2 TexCoords;
out mat4 color;
out vec3 normal;
out vec3 fragPos;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 
    color = aColor;
    normal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;  
    fragPos = vec3(aInstanceMatrix * vec4(aPos, 1.0));
    //gl_Position = projection * view * vec4(0,aPos.y,0,1.0f); 
}