#version 330 core

in vec2 uv;
out vec4 fragColor;

uniform vec3 uCameraPos;   // Example uniform: camera position
uniform float uFov;        // Example uniform: field of view

void main()
{
    // For now, output a gradient. Replace with ray tracing logic!
    fragColor = vec4(uv, 0.5 + 0.5 * sin(uFov), 1.0);
}
