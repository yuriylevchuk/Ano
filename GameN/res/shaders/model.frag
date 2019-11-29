#version 330 core

out vec4 FragColor;

struct DirectionLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
    float linear;
    float quadratic;  

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 2

//in vec3 FragPos;
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;
//uniform DirectionLight dirLight;
//uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float LinearizeDepth(float depth);

float zNear = 0.1; 
float zFar  = 100.0; 

void main() {
	//vec3 norm = normalize(Normal);
	//vec3 viewDir = normalize(viewPos - FragPos);
	
	//vec3 result = calcDirectionLight(dirLight, norm, viewDir);

	//for (int i = 0; i < NR_POINT_LIGHTS; i++) {
	//	result += calcPointLight(pointLights[i], norm, FragPos, viewDir);
	//}

	//FragColor = vec4(result, 1.0f);
	//FragColor = texture(texture_diffuse1, TexCoords);

	//vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
	vec3 reflCol = texture(skybox, R).rgb;
	vec3 reflTex = texture(texture_reflection1, TexCoords).rgb;
	vec3 diffTex = texture(texture_diffuse1, TexCoords).rgb;
	vec3 result = reflCol;
    FragColor = vec4(result, 1.0);
	//float depth = LinearizeDepth(gl_FragCoord.z) / zFar; 
	//FragColor = vec4(vec3(depth), 1.0f);
}

vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - fragPos);
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(texture_specular1, TexCoords));
	ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	return ambient + diffuse + specular;
}

float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;

	return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}