#version 330 core

out vec4 FragColor; // Final color output

in vec3 Normal;  // The normal from the vertex shader
in vec2 TexCoord; // Texture coordinates from the vertex shader
in vec3 FragPos;  // The fragment position in world space
in vec3 color;    // Vertex color

uniform vec4 lightColor; // Light color
uniform vec3 lightPos;   // Light position
uniform vec3 camPos;     // Camera position

uniform sampler2D tex0; // Base texture

// Material properties
struct Material {
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// Phong lighting calculation
vec4 PhongLighting(Material mat)
{
    // Ambient lighting (simple constant term)
    vec3 ambient = 0.9 * mat.diffuse;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular lighting
    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);

    // Combine all components
    vec3 diffuse = diff * mat.diffuse;
    vec3 specular = spec * mat.specular;

    return vec4(ambient + diffuse + specular, 1.0) * texture(tex0, TexCoord);
}

void main()
{
    // Set material properties
    Material mat;
    mat.diffuse = vec3(1.0, 1.0, 1.0); // Example diffuse color
    mat.specular = vec3(1.0, 1.0, 1.0); // Example specular color
    mat.shininess = 32.0; // Shininess coefficient

    // Phong lighting calculation
    vec4 lighting = PhongLighting(mat);

    // Map Normal to RGB
    vec4 normalColor = vec4(Normal * 0.5 + 0.5, 1.0); // Convert Normal to RGB and add alpha

    // Blend Normal Color with Lighting
    vec4 FinalColor = lighting * normalColor;

    // Output the final color
    FragColor = FinalColor;
}
