#version 460 core

in vec2 TexCoord;

uniform sampler2D billboardTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(billboardTexture, TexCoord);
    //FragColor = vec4(TexCoord, 0.0, 0.0);
    //FragColor = vec4(1.0, 0.5, 1.0, 1.0);
}