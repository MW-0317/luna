#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    //mat4 inverseViewMat = inverse(view);
    //inverseViewMat[3].xyz = vec3(0, 0, -1);
    //mat4 mvMat = view * inverseViewMat;
    //gl_Position = projection * mvMat * model * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
