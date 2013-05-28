//version 130

attribute vec4 vPosition;
attribute vec3 vNormal;
attribute vec2 vTexCoords;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;

uniform vec4 Offset; // offset of the cube; set .w = 0
uniform mat4 IndividualRotation; // individual rotation of the cube


void main()
{
	vec4 position = IndividualRotation * vPosition + Offset;
	vec3 normal = (IndividualRotation * vec4(vNormal, 1)).xyz;
	
	vec4 N = vec4(normal, 0.0);
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

