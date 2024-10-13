#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
out vec3 ourColor;
void main()
{
   gl_Position = vec4(aPos, 1.0);
   ourColor = aColor;
   //vertexColor = vec4(aPos.x+0.5, aPos.y+0.5, 0.0, 1.0);
}