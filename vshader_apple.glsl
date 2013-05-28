attribute  vec3 vNormal;
attribute  vec4 vPosition;
attribute  vec2 vTexCoords;

attribute  vec4 vMaterialAmbient;
attribute  vec4 vMaterialDiffuse;
attribute  vec4 vMaterialSpecular;
attribute  float vMaterialShininess;

//variables to communicate with the fragment shader
varying vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
varying float Shininess;

varying vec3 fN; //normal at current position
varying vec3 fV; //vector from point to viewer
varying vec3 fL; //vector from point to light

uniform mat4 cMw;
uniform mat4 wMo;

uniform mat4 Projection;
uniform vec4 LightPosition;
uniform vec4 LightAmbient, LightDiffuse, LightSpecular;

void main()
{    
    AmbientProduct = LightAmbient * vMaterialAmbient;
    DiffuseProduct = LightDiffuse * vMaterialDiffuse;
    SpecularProduct = LightSpecular * vMaterialSpecular;
    Shininess = vMaterialShininess;
   
    mat4 ModelView = cMw*wMo;
    
    vec4 N = vec4(vNormal, 0);
    fN = (ModelView*N).xyz;
    fV = -(ModelView*vPosition).xyz;
    //LightPosition refers to LightPosition in world coordinates!
    fL = (cMw*(LightPosition-wMo*vPosition)).xyz;
    
    
    gl_Position = Projection*ModelView*vPosition;
	gl_TexCoord[0].xy = vTexCoords;

}

