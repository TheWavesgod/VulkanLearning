#version 450

layout(location = 0) in vec3 fragColour;            // Interpolated colour from vertex (location must metch)

layout(location = 0) out vec4 OutColour;            // Final output colour (must also have location)   

void main()
{
    OutColour = vec4(fragColour, 1.0);
}