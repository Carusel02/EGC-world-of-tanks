#version 330

uniform sampler2D texture_generation;

// Output
layout(location = 0) out vec4 out_color;

in vec2 texcoord;

void main()
{
    // TODO(student): Write pixel out color
    vec4 color = texture2D(texture_generation, texcoord);

    out_color = color;

}
