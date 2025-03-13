#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 greenColor;

void main()
{
    vec4 texColor = texture(ourTexture, TexCoord);
    if (texColor.a < 0.1)
        discard;
    FragColor = texColor + vec4(0.0, greenColor.g, 0.0, 0.0);
}