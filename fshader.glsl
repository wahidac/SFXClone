#version 130 

in vec3 fN;
in vec3 fL;
in vec3 fE;

uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform float Shininess;
uniform sampler2D Tex;
uniform int EnableTex;

void main() 
{ 
	// Normalize the input lighting vectors
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);

	vec3 H = normalize(L + E);
	
	vec4 ambient = AmbientProduct;

	float Kd = max(dot(L, N), 0.0);
	vec4 diffuse = Kd * DiffuseProduct;
	
	if (EnableTex == 1)
		diffuse *= texture2D(Tex, vec2(gl_TexCoord[0]));
	
	float Ks = pow(max(dot(N, H), 0.0), Shininess);
	
	vec4 specular = Ks * SpecularProduct;

	// discard the specular highlight if the light's behind the vertex
	if (dot(L, N) < 0.0)
	{
		specular = vec4(0.0, 0.0, 0.0, 1.0);
	}

	gl_FragColor = ambient + diffuse + specular;
	gl_FragColor.a = 1.0;
	
} 

