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

    uniform sampler2D defaultTexture;
    uniform samplerCube CM_defaultTexture;
    uniform Camera camera;
    uniform float mixAmount;

    out vec4 FragColor;

    void main()
    {
        vec4 color = vec4(0.0);
        vec4 defaultTextureCol = texture(defaultTexture, texCoord);

        vec3 I = normalize(fPosition - camera.position);
        vec3 R = reflect(-I, normalize(fNormal));
        vec4 cubeMapTextureCol = texture(CM_defaultTexture, R);

        color = mix(defaultTextureCol, cubeMapTextureCol, mixAmount);
        if (color.a <= 0.01)
            discard;
        FragColor = color;
    }
}
