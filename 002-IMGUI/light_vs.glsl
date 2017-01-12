#version 330 core                                
                                                 
in vec3 position;            
in vec3 normal; 
in vec2 texture;           
    
uniform mat4 proj;                                             
uniform mat4 view;
uniform mat4 model; 
             
out VS_OUT {
	vec3 N;
	vec3 L;
	vec3 V;
	vec3 D;
	vec2 T;
} vs_out;  

uniform vec3 light_position = vec3(3, 5, 3);   
uniform vec3 light_target = vec3(0.5, 0, 0.5);            
									         
void main(void) {  
	gl_Position = proj * view * model * vec4(position, 1.0);

	mat4 mv_matrix = view * model;

	// Light position in view-space
	vec3 light_pos = vec3(view * vec4(light_position, 1.0));

	// Calculate view-space coordinate
	vec4 P = mv_matrix * vec4(position, 1.0);

	// Calculate normal in view-space
	vs_out.N = mat3(transpose(inverse(mv_matrix))) * normal;

	// Calculate light vector
	vs_out.L = light_pos - P.xyz;

	// Calculate view vector
	vs_out.V = -P.xyz;
	
	// Calculate spot direction in view-space
	vec4 spot_dir = vec4(light_target - light_position, 0);
	vs_out.D = vec3(view * spot_dir);

	vs_out.T = texture;
}                                                
