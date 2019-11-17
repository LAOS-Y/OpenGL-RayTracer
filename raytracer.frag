#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D tex_specular;
uniform sampler2D tex_diffuse;
uniform sampler2D tex_ambient;


void main()
{
    FragColor = mix(texture(tex_specular, TexCoord) + texture(tex_diffuse, TexCoord),
                    texture(tex_ambient, TexCoord),
                    0.2);
    
    FragColor = sqrt(FragColor);

    // FragColor = sqrt(texture(tex_specular, TexCoord) + texture(tex_diffuse, TexCoord) + texture(tex_ambient, TexCoord));
}