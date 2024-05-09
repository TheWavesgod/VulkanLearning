#version 450

layout(location = 0) in vec3 fragCol;

layout(location = 0) out vec4 OutColour;            // Final output colour (must also have location)   

void main()
{
    OutColour = vec4(fragCol, 1.0);
}