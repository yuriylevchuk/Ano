#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;


float LinearizeDepth(float depth);

float zNear = 0.1; 
float zFar  = 100.0; 

void main() {
	//vec3 norm = normalize(Normal);
	//vec3 viewDir = normalize(viewPos - FragPos);
	

	//FragColor = vec4(result, 1.0f);
	//FragColor = texture(texture_diffuse1, TexCoords);

	//vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
	vec3 reflCol = texture(skybox, R).rgb;
	vec3 reflTex = texture(texture_reflection1, TexCoords).rgb;
	vec3 diffTex = texture(texture_diffuse1, TexCoords).rgb;
	vec3 result = diffTex + (reflTex * reflCol);
    FragColor = vec4(result, 1.0);
	//float depth = LinearizeDepth(gl_FragCoord.z) / zFar; 
	//FragColor = vec4(vec3(depth), 1.0f);
}

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;

	return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}