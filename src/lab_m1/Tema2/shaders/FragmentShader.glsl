#version 330

// Input
in vec3 f_color;
flat in int f_state;

// Output
layout(location = 0) out vec4 out_color;


void main()
{   
    // compute color
    vec3 final_color = abs(f_color + (3 - f_state) * vec3(-0.03f, -0.05f, -0.04f));

    // pixel out color
	out_color = vec4(final_color, 1.0f);

}
