#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 pos_worldspace = model[3].xyz;
    vec3 camera_right = {view[0][0], view[1][0], view[2][0]};
    vec3 camera_up = {view[0][1], view[1][1], view[2][1]};
    model[3].xyz = pos_worldspace + camera_right * aPos.x + camera_up * aPos.y;
    //mat4 inverseViewMat = inverse(view);
    //inverseViewMat[3].xyz = vec3(0, 0, -1);
    //mat4 mvMat = view * inverseViewMat;
    //gl_Position = projection * mvMat * model * vec4(aPos, 1.0);
    gl_Position = projection * view * model * vec4(1.0);
}
