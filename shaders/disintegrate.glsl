VS
{
    #version 460 core

    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform sampler2D defaultTexture;
    uniform sampler2D noiseTexture;

    out vec2 texCoord;

    void main()
    {
        texCoord = aTexCoord;
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
}

FS
{
    #version 460 core

    in vec2 texCoord;
    layout(std430, binding = 3) volatile buffer test
    {
        uint n;
        uint data_SSBO[];
    };

    uniform float time;
    uniform float deltatime;
    uniform sampler2D defaultTexture;
    uniform sampler2D noiseTexture;
    uniform int width;
    uniform int height;

    out vec4 FragColor;

    vec2 getPixelCoordinates()
    {
        //(gl_Position.xyz / gl_Position.w).xy * 0.5 + 0.5)
        //* vec2(width, height);
        return gl_FragCoord.xy;
    }

    void main()
    {
        vec4 noiseColor = texture2D(noiseTexture, texCoord);
        if (noiseColor.r < mod((time) / 5, 1.0))
            discard;
        if (noiseColor.r < mod((time + deltatime) / 5, 1.0))
        {
            vec2 pixelCoordinates = getPixelCoordinates();
            data_SSBO[n] = ( int(pixelCoordinates.x) << 16) + ( int(pixelCoordinates.y) & 65535);
            atomicAdd(n, 1);
        }
        FragColor = texture2D(defaultTexture, texCoord);
        //FragColor = vec4(1.0, 0.5, 1.0, 1.0);
    }
}
