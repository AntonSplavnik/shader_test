
#version 330 core

layout(location = 0) in vec2 aPos;
out vec2 uv;

void main()
{
    uv = (aPos + 1.0) * 0.5;    // Map from [-1,1] to [0,1] for screen space UV
    gl_Position = vec4(aPos, 0.0, 1.0);
}
