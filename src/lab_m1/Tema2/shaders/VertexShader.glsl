#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform vec3 Color;
uniform int State;
uniform int Type;
uniform vec3 eye_position;
uniform vec3 light_position;


// Output
out vec3 f_color;
flat out int f_state;

vec3 deformation(vec3 vertex, int State) {
    
    // no deformation
    if(State == 3 || State == 0) {
		return vertex;
	}

    // center of the tank
    vec3 center_tank = vec3(0.0f, 1.0f, 0.0f);
    // distance to center 
    vec3 distanceToCenter = abs(vertex - center_tank);

    // deformation of the tank
    if(distanceToCenter.x < 1.1f) {
		return vertex + (3 - State) * vec3(-0.4f, -0.2f, -0.3f);
	}
    
    return vertex;
}

void main()
{   
    float material_kd = 0.4;
    float material_ks = 0.4;
    int material_shininess = 30;

    // world position
    vec3 world_position = (Model * vec4(v_position,1)).xyz;
    // world normal
    vec3 world_normal = normalize( mat3(Model) * v_normal);

    vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(light_position - world_position);
	vec3 H = normalize(L + V);

    // how shiny is object without any source of light
    float emisive = 0.35f - 0.00125f * Time;
    // how much light is reflected directly from object
    float ambient = 0.25f;
    // how much light is reflected diffusely
    float diffuse = material_kd * max(dot(world_normal, L), 0);
    // how much light is reflected specularly
    float specular = material_ks * pow(max(dot(world_normal, H), 0), material_shininess);

    float dist = length(light_position - world_position);
    float k = 0.05f - 0.00009f * Time;
    float att = 1 / (1 + k * dist + k * k * dist * dist);

    vec3 coloration = Color;

    // check if we are the sky
    if(Type == 1) {
        coloration = Color -  Time * vec3(0.00525f, 0.0075f, 0.0057f);
    }
    // check if we are the ground
    if(Type == 2) {
		coloration = Color -  Time * vec3(0.003f, 0.0036f, 0.0009f);
	}
    // check if we are the stars
    if(Type == 3) {
        emisive = 0.2f + 0.005 * Time;
    }

    // compute color
    vec3 final_color = coloration * (emisive + ambient + att * (diffuse + specular));
    
    // send color and state to fragment shader
    f_color = final_color;
    f_state = State;


    // deformation of the tank
    vec3 deformed_position = deformation(v_position, State);

    if(Type == 3) {
		deformed_position = v_position + sin(Time) * 3.0f + cos(Time) * 3.0f;
	}

    // compute gl_Position
    gl_Position = Projection * View * Model * vec4(deformed_position, 1.0);

}
