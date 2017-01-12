#version 330 core                        
           
out vec4 final_color; 

uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);

uniform sampler2D tex_diffuse;
uniform sampler2D tex_specular;

uniform bool use_texture = false;

in VS_OUT
{
	vec3 N;
	vec3 L;
	vec3 V;
	vec3 D;
	vec2 T;
} fs_in;

const float k0 = 1.0;   //constant attenuation
const float k1 = 0.09;  //linear attenuation
const float k2 = 0.032; //quadratic attenuation
		           
const float spot_inner_cutoff = 0.79; //.6	
const float spot_outer_cutoff = 0.80;
	                    
void main(void) 
{  
	vec3 N = normalize(fs_in.N);
	vec3 L = normalize(fs_in.L);
	vec3 V = normalize(fs_in.V);
	vec3 H = normalize(L + V);

	float theta = dot(-L, normalize(fs_in.D));
	float diffuse = clamp(dot(N, L), 0, 1);
	float specular = pow(clamp(dot(N, H), 0, 1), 32);

	float d = length(fs_in.L);

	float epsilon = spot_outer_cutoff - spot_inner_cutoff;
	float intensity = clamp((theta - spot_outer_cutoff) / epsilon, 0, 1);
	float attenuation = 1.0 / (k0 + (k1 * d) + (k2 * d * d));

	if(use_texture == false) {
		diffuse *= attenuation * intensity;
		specular *= attenuation * intensity;

		vec3 ambientColor = vec3(0.1);
		vec3 diffuseColor = lightColor * diffuse;
		vec3 specularColor = lightColor * specular;
	
		final_color = vec4(ambientColor + diffuseColor + specularColor, 1.0);
	}
	else {
		//final_color = diffuse * texture(tex_diffuse, fs_in.T) + specular * texture(tex_specular, fs_in.T);
		
		float diffuse_power = diffuse * intensity * attenuation/**/;
		vec3 diffuse_color = lightColor * diffuse_power * vec3(texture(tex_diffuse, fs_in.T));

		float specular_power = specular * intensity * attenuation/**/;
		vec3 specular_color = lightColor * specular_power * vec3(texture(tex_specular, fs_in.T));

		final_color = vec4(diffuse_color + specular_color, 1.0);
	}
}                                                                 