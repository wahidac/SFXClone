// per-fragment interpolated values from the vertex shader
varying  vec3 fN;
varying  vec3 fL;
varying  vec3 fV;

varying vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
varying float Shininess;

uniform sampler2D Tex;
uniform int EnableTex;
uniform int blackTransparent;


uniform int MoveTex;
uniform float TexOffset;
uniform float alpha;
uniform int blendModel;

uniform int useColorPicking;

void main() 
{ 
    //Phong shading
    // Normalize the input lighting vectors
       vec3 N = normalize(fN);
       vec3 E = normalize(fV);
       vec3 L = normalize(fL);

       vec3 H = normalize( L + E );
    
       vec4 ambient = AmbientProduct;

       float Kd = max(dot(L, N), 0.0);
       vec4 diffuse = Kd*DiffuseProduct;
       
       //if (EnableTex == 1)
		//    diffuse *= texture2D(Tex, vec2(gl_TexCoord[0]));
        
       float Ks = pow(max(dot(N, H), 0.0), Shininess);
       
       vec4 specular = Ks*SpecularProduct;

       // discard the specular highlight if the light's behind the vertex
       if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
       }

	   vec4 combined = ambient + diffuse + specular;
	   if (EnableTex == 1 && MoveTex == 0)
			combined = texture2D(Tex, vec2(gl_TexCoord[0]));
	   else if(EnableTex == 1 && MoveTex == 1)
			combined = texture2D(Tex, vec2(gl_TexCoord[0].x, gl_TexCoord[0].y + TexOffset) * 10.0);

       gl_FragColor = combined; //ambient + diffuse + specular;
       gl_FragColor.a = 1.0;
       
       //Make all black transparent if we're animating an explosion so we only see the 
       //flames from the explosion
       if(blackTransparent == 1 && gl_FragColor.xyz == vec3(0,0,0)) {
            gl_FragColor.a = 0.0;
       }
       
       if(blendModel == 1) {
            gl_FragColor.a = alpha;
       }

	   if(useColorPicking == 1) {
			gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	   }
} 
