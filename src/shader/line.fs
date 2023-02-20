#version 330 core
out vec4 FragColor;

in vec3 fColor;
struct {
    vec3 position;
    vec3 color;
} PointLight;
//uniform PointLight light;
void main()
{
    FragColor = vec4(fColor, 1.0);   
}