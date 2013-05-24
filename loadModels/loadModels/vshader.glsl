attribute  vec3 vNormal;
attribute  vec4 vPosition;


varying vec3 fN; //normal at current position
varying vec3 fV; //vector from point to viewer
varying vec3 fL; //vector from point to light
varying vec4 color;

uniform mat4 cMw;
uniform mat4 wMo;

uniform mat4 Projection;
uniform vec4 LightPosition;
uniform int shaderType;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float Shininess;

void main()
{    
    mat4 ModelView = cMw*wMo;
    fN = (ModelView*vec4(vNormal,0)).xyz;
    fV = -(ModelView*vPosition).xyz;
    fL = (cMw*(LightPosition-wMo*vPosition)).xyz;

    //Gourard shading or flat shading 
    if(shaderType == 1 || shaderType == 0) {
        //calculate vertex color and let frag shader interpolate
        vec3 N = normalize(fN);
        vec3 E = normalize(fV);
        vec3 L = normalize(fL);

        vec3 H = normalize( L + E );
    
        vec4 ambient = AmbientProduct;

        float Kd = max(dot(L, N), 0.0);
        vec4 diffuse = Kd*DiffuseProduct;
    
        float Ks = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = Ks*SpecularProduct;

        // discard the specular highlight if the light's behind the vertex
        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        color = ambient + diffuse + specular;
        color.a = 1.0;
    }
        //color = vec4(1,0,0,1);

    gl_Position = Projection*ModelView*vPosition;
}

