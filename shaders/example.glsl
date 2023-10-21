VS
{
    #version 460 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec3 aNormal;
    layout (location = 3) in vec2 aTexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 fPosition;
    out vec3 fColor;
    out vec3 fNormal;
    out vec2 texCoord;

    void main()
    {
        texCoord = aTexCoord;
        fColor = aColor;
        vec4 tempPos = model * vec4(aPos, 1.0);
        fPosition = tempPos.xyz;
        mat4 normal_matrix = transpose(inverse(model));
        fNormal = normalize((normal_matrix * vec4(aNormal, 0.0)).xyz);
        gl_Position = projection * view * tempPos;
    }
}

FS
{
    #version 460 core

    struct Camera
    {
        vec3 position;
    };

    in vec3 fPosition;
    in vec3 fColor;
    in vec3 fNormal;
    in vec2 texCoord;

    //uniform sampler2D defaultTexture;
    uniform samplerCube CM_defaultTexture;
    uniform Camera camera;

    out vec4 FragColor;

    void main()
    {
        vec3 color = vec3(0.0);
        vec3 defaultTextureCol = vec3(1.0); //texture(defaultTexture, texCoord).xyz;

        vec3 I = normalize(fPosition - camera.position);
        vec3 R = reflect(-I, normalize(fNormal));
        vec3 cubeMapTextureCol = texture(CM_defaultTexture, R).xyz;

        color = cubeMapTextureCol;
        // if (all(equal(cubeMapTextureCol, vec3(0.0))))
        //     color = defaultTextureCol;
        // else if (all(equal(defaultTextureCol, vec3(0.0))))
        //     color = cubeMapTextureCol;
        FragColor = vec4(color, 1.0);
    }
}
