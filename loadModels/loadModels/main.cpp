// rotating cube with two texture objects
// change textures with 1 and 2 keys

#include "Angel.h"
#include "tga.h"
#include "OBJobject.h"

using namespace std;

const int  NumVertices  = 11566*3;
const int  TextureSizeW  = 2048;
const int  TextureSizeH  = 1024;
const GLfloat aspectRatio = float(TextureSizeW)/float(TextureSizeH);


typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

// Texture objects and storage for texture image
GLuint textures[2];

//GLubyte image[TextureSizeH][TextureSizeW][3];
//GLubyte image2[TextureSizeH][TextureSizeW][3];
GLubyte image[TextureSizeH * TextureSizeW * 3];
//GLubyte image2[TextureSizeH * TextureSizeW * 3];


// Vertex data arrays
point4  *points;
color4  quad_colors[NumVertices];
vec2    tex_coords[NumVertices];
vec2    tex_coords_zoom[NumVertices];


// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLuint   theta;

//Indices to interact with shaders
GLuint vPosition, vColor, vTexCoord, ModelView, Projection;

//Perspective projection parameters
GLfloat  fovy = 95.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;      // Viewport aspect ratio
GLfloat  near= 0.1, far=10.0;

//Transformation matrices
mat4 positionCamera = Translate(0.0, 0.0, -2);


OBJMesh *m;
bool flag = false;
GLuint vao,vao2;
GLuint program;
OBJObject *obj;

//----------------------------------------------------------------------------

int Index = 0;

void
quad( int a, int b, int c, int d )
{
    point4 vertices[8] = {
        point4( -0.5, -0.5,  0.5, 1.0 ),
        point4( -0.5,  0.5,  0.5, 1.0 ),
        point4(  0.5,  0.5,  0.5, 1.0 ),
        point4(  0.5, -0.5,  0.5, 1.0 ),
        point4( -0.5, -0.5, -0.5, 1.0 ),
        point4( -0.5,  0.5, -0.5, 1.0 ),
        point4(  0.5,  0.5, -0.5, 1.0 ),
        point4(  0.5, -0.5, -0.5, 1.0 )
    };
    
    color4 colors[8] = {
        color4( 0.0, 0.0, 0.0, 1.0 ),  // black
        color4( 1.0, 0.0, 0.0, 1.0 ),  // red
        color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
        color4( 0.0, 1.0, 0.0, 1.0 ),  // green
        color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
        color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
        color4( 1.0, 1.0, 1.0, 1.0 ),  // white
        color4( 1.0, 1.0, 1.0, 1.0 )   // cyan
    };
    
    //Adjust t so that aspect ration of image is preserved
    float t = .5/aspectRatio;
    t = 1/t;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[a];
    tex_coords_zoom[Index] = vec2( 0.0, t);
    tex_coords[Index] = vec2( 0.0, 1.0);


    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[b];
    tex_coords_zoom[Index] = vec2( 0.0, 0.0);
    tex_coords[Index] = vec2( 0.0, 0.0 );


    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[c];
    tex_coords_zoom[Index] = vec2( 2.0, 0.0);
    tex_coords[Index] = vec2( 1.0, 0.0 );


    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[a];
    tex_coords_zoom[Index] = vec2( 0.0,t);
    tex_coords[Index] = vec2( 0.0, 1.0 );


    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[c];
    tex_coords_zoom[Index] = vec2( 2.0, 0.0);
    tex_coords[Index] = vec2( 1.0, 0.0 );


    Index++;
    
    quad_colors[Index] = colors[a];
    points[Index] = vertices[d];
    tex_coords_zoom[Index] = vec2( 2.0, t);
    tex_coords[Index] = vec2( 1.0, 1.0 );

    Index++;
        
}

//----------------------------------------------------------------------------

void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------


void
init()
{
    program = InitShader( "vshader71.glsl", "fshader71.glsl" );
    glUseProgram( program );
    
    
    glEnable( GL_DEPTH_TEST );
  
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    obj->drawSelf();
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
    aspect = GLfloat(width)/height;
}

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta[Axis] += 0.01;
    
    if ( Theta[Axis] > 360.0 ) {
        Theta[Axis] -= 360.0;
    }
    
    glutPostRedisplay();
}

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
           positionCamera = Translate(0.0, 0.0, 0.1) * positionCamera;
           break; //forward
       case 'o': positionCamera = Translate(0.0, 0.0, -0.1) * positionCamera;
           break; //backwards
       case 'r': positionCamera = Translate(0.0, 0.0, -2);
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
 /*   glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Color Cube" );

    loadFile("earth.tga");
    init();
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();
    return 0;
  */
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutCreateWindow( "Color Cube" );
    
    init();
    obj = new OBJObject("dragon.obj",program);

    
    
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutReshapeFunc( reshape );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );
    
    glutMainLoop();

    //Renderer renderer;
    //renderer.init(&argc, argv);
    
    //Draw the object to the screen
    //renderer.render(m);
   
    return 0;
}