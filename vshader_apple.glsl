attribute  vec3 vNormal;
attribute  vec4 vPosition;
attribute  vec2 vTexCoords;

//For simplicity, I store shader parameters for each individual
//vertex on the GPU, because for a given object loaded using the GLM library, 
//it is possible for different parts of the object itself to 
//have different shading parameters.
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

uniform int calculateTexCoordInShader;

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
    
    if(calculateTexCoordInShader == 1) {
        //Manually calculate proper texture coordinate
       // dot(
        vec4 sPlane = vec4(1,0,0,0);
        vec4 tPlane = vec4(0,1,0,0);
        float sCoord = dot(vPosition,sPlane);
        float tCoord = dot(vPosition,tPlane);
        gl_TexCoord[0].xy = vec2(sCoord,tCoord);
    } else {
        gl_TexCoord[0].xy = vTexCoords;
    }

}

