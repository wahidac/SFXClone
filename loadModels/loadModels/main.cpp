#include "GLJoe.h"
#include "OBJobject.h"



//Transformation matrices
GLJoe::Mat4 positionCamera = GLJoe::Translate(0.0, 0.0, -2);


OBJMesh *m;
bool flag = false;
GLuint vao,vao2;
GLuint program;
OBJObject *obj;

GLfloat near= 1.0, far=50.0;
//GLfloat left= -5.0, right=5.0, top=5.0, bottom= -5.0, near=1.0, far=500.0;

//Perspective projection parameters
GLfloat  fovy = 90.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect = 1;       // Viewport aspect ratio

//lighting parameters
GLJoe::Vec4 light_position(0,0,1,1);
GLJoe::Vec4 light_ambient(.8, .8, .8, 1.0 );
GLJoe::Vec4 light_diffuse( .8, .8,.8, 1.0 );
GLJoe::Vec4 light_specular( .8, .8, .8, 1.0 );
GLJoe::Mat4 projection = GLJoe::Perspective(fovy, aspect, near, far);


//----------------------------------------------------------------------------

int Index = 0;


void
init()
{
    program = GLJoe::InitShader("vshader.glsl", "fshader.glsl" );
    glUseProgram( program );
    
    glEnable( GL_DEPTH_TEST );
    
    //Set Projection parameters
  
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    obj->drawSelf();
    glutSwapBuffers();
}


OBJObjectShaderHandles initShaderHandles() {
    OBJObjectShaderHandles handles;
    
    handles.cMw = glGetUniformLocation(program, "cMw");
    handles.wMo = glGetUniformLocation( program, "wMo" );
    handles.Projection = glGetUniformLocation( program, "Projection" );
    handles.LightAmbient = glGetUniformLocation(program, "LightAmbient");
    handles.LightDiffuse = glGetUniformLocation(program, "LightDiffuse");
    handles.LightSpecular = glGetUniformLocation(program, "LightSpecular");
    handles.LightPosition = glGetUniformLocation(program, "LightPosition");
    handles.vMaterialAmbient = glGetAttribLocation( program, "vMaterialAmbient" );
    handles.vMaterialDiffuse = glGetAttribLocation( program, "vMaterialDiffuse" );
    handles.vMaterialSpecular = glGetAttribLocation( program, "vMaterialSpecular" );
    handles.vMaterialShininess = glGetAttribLocation( program, "vMaterialShininess" );
    handles.vNormal = glGetAttribLocation( program, "vNormal" );
    handles.vPosition = glGetAttribLocation( program, "vPosition" );

    
    return handles;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int mousex, int mousey )
{
   switch( key ) {
        case 033: // Escape Key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
       case 'i':
           obj->wMo.translate(0, 0, .1);
           break; //forward
       case 'o':
           obj->wMo.translate(0, 0, -.1);
           break; //backwards
       case 'r':
           obj->wMo.rotateX(5);
           break;
       default:
           break;
    }
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Color Cube" );
    
    init();
    
    OBJObjectParams defaults;
    defaults.material_ambient = GLJoe::Vec4(1,0,0,1);
    defaults.material_diffuse = GLJoe::Vec4(1,0,0,1);
    defaults.material_specular = GLJoe::Vec4(1,1,1,1);
    defaults.material_shininess = 300;
    
    OBJObjectShaderHandles handles = initShaderHandles();
    
    //Set light position + lighting properties as well as projection matrix
    glUniform4fv( handles.LightPosition,1, light_position );
    glUniform4fv( handles.LightAmbient, 1, light_ambient );
    glUniform4fv( handles.LightDiffuse, 1, light_diffuse );
    glUniform4fv( handles.LightSpecular, 1, light_specular );
    glUniformMatrix4fv( handles.Projection, 1, GL_TRUE, projection );
    
    
    GLJoe::Transform cMw,wMo;
    
    cMw.translate(0, 0, -2);
    wMo.rotateX(90);
    
    obj = new OBJObject("Models/f-16.obj",handles,cMw,wMo,NULL);

    
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();

    return 0;
}