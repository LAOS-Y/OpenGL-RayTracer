#version 430 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D tex_specular[2];
uniform sampler2D tex_diffuse[2];
uniform sampler2D tex_ambient[2];


// uniform sampler2D tex_specular[1];
// uniform sampler2D tex_diffuse[1];
// uniform sampler2D tex_ambient[1];


void main()
{
    vec4 specular = (texture(tex_specular[0], TexCoord) + texture(tex_specular[1], TexCoord)) / 2;
    vec4 diffuse = (texture(tex_diffuse[0], TexCoord) + texture(tex_diffuse[1], TexCoord)) / 2;
    vec4 ambient = (texture(tex_ambient[0], TexCoord) + texture(tex_ambient[1], TexCoord)) / 2;
    
    // vec4 specular0 = texture(tex_specular[0], TexCoord);
    // vec4 diffuse0 = texture(tex_diffuse[0], TexCoord);
    // vec4 ambient0 = texture(tex_ambient[0], TexCoord);
    
    // vec4 specular1 = texture(tex_specular[1], TexCoord);
    // vec4 diffuse1 = texture(tex_diffuse[1], TexCoord);
    // vec4 ambient1 = texture(tex_ambient[1], TexCoord);

    FragColor = mix(specular + diffuse,
                    ambient,
                    0.2);

    // FragColor = ambient0;

    FragColor = sqrt(FragColor);
}