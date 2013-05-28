#version 130

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

uniform vec4 Offset; // offset of the cube; set .w = 0
uniform mat4 IndividualRotation; // individual rotation of the cube


void main()
{
	vec4 position = IndividualRotation * vPosition + Offset;
	vec3 normal = (IndividualRotation * vec4(vNormal, 1)).xyz;
	
	vec4 N = vec4(normal, 0.0f);
	fN =  (ModelView * N).xyz;
	fE = -(ModelView * position).xyz;
	fL =  (LightPosition).xyz;

	if (LightPosition.w != 0.0)
	{
		fL = LightPosition.xyz - position.xyz;
	}

	gl_Position = Projection * ModelView * position;
	gl_TexCoord[0].xy = vTexCoords;
}

