VS
{
    #version 460 core

    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 velocity;
    layout (location = 2) in vec3 acceleration;
    layout (location = 3) in float life;
    layout (location = 4) in uint alive;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    //uniform vec4 colorBegin;
    //uniform vec4 colorEnd;
    //uniform vec4 lifeTime;
    //uniform uint maxParticles;

    void main()
    {
        gl_Position = projection * view * model * vec4(position, 1.0);
    }
}

FS
{
    #version 460 core

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.5, 1.0, 1.0);
    }
}
