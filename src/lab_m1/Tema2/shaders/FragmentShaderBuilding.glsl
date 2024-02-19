#version 330

uniform sampler2D texture1;

// Output
layout(location = 0) out vec4 out_color;

in vec2 texcoord;

void main()
{
    // TODO(student): Write pixel out color
    vec4 colour1 = texture2D(texture1, texcoord);

    out_color = colour1;

}
