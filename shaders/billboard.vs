#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
    TexCoord = aTexCoord;
    
    vec3 pos_worldspace = model[3].xyz;
    vec3 camera_right = {view[0][0], view[1][0], view[2][0]};
    vec3 camera_up = {view[0][1], view[1][1], view[2][1]};
    gl_Position = projection * view * vec4(pos_worldspace + camera_right * aPos.x + camera_up * aPos.y, 1.0);
}
