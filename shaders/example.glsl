VS
{
    #version 460 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec2 texCoord;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
}

FS
{
    #version 460 core

    in vec2 texCoord;

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.5, 1.0, 1.0);
    }
}
