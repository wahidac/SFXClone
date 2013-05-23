attribute  vec4 vPosition;
attribute  vec4 vColor;
attribute  vec2 vTexCoord;

varying vec4 color;
varying vec2 texCoord;

uniform vec3 theta;


uniform mat4 Projection;
uniform mat4 ModelView;

void main() 
{    
    color       = vec4(1,0,0,1);
    gl_Position = ModelView*vPosition;
    
    //Projection*ModelView*vPosition;
} 
