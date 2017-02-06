
#include <windows.h>
#include "Antons_maths_funcs.h"
#include "teapot.h" // teapot mesh
#include "Utilities.h"
#include "Particle.h"
#include "Collision.h"
using namespace std;

const float width = 900, height = 900;
/*----------------------------------------------------------------------------
						MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/
vec3 mouseLocation = vec3(0.0f, 0.5f, 0.0f);
vec3 closestPoint = vec3(0.5f, -0.5f, 0.0f);

vec3 triangle[] =
{
	vec3(0.0f, 0.5f, 0.0f),
	vec3(0.5f, -0.5f, 0.0f),
	vec3(-0.5f, -0.5f, 0.0f)
};


/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint simpleShaderID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
/*----------------------------------------------------------------------------
						FUNCTION DEFINITIONS
----------------------------------------------------------------------------*/

void drawloop(mat4 view, mat4 proj, GLuint framebuffer);
void rotateTriangle(float degrees);

/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	simpleShaderID = shaderFactory.CompileShader(SIMPLE_VERT, SIMPLE_FRAG);
}

void display() 
{
	mat4 proj = identity_mat4();
	mat4 view = identity_mat4();
	glViewport(0, 0, width, height);
	drawloop(view, proj, 0);
	draw_texts();
	glutSwapBuffers();
}

void updateScene() {
	static DWORD  last_frame;	//time when last frame was drawn
	static DWORD last_timer = 0;	//time when timer was updated
	DWORD  curr_time = timeGetTime();//for frame Rate Stuff.
	static bool first = true;
	if (first)
	{
		last_frame = curr_time;
		first = false;
	}
	float  delta = (curr_time - last_frame) * 0.001f;
	if (delta >= 0.03f) 
	{
		last_frame = curr_time;
		glutPostRedisplay();
		rotateTriangle(0.5);

		closestPoint = getClosestPointTriangle(triangle, mouseLocation);

		string text;
		text += "P1 = [" + to_string(triangle[0].v[0]) + "," + to_string(triangle[0].v[1]) + "," + to_string(triangle[0].v[2]) + "]\n";
		text += "P2 = [" + to_string(triangle[1].v[0]) + "," + to_string(triangle[1].v[1]) + "," + to_string(triangle[1].v[2]) + "]\n";
		text += "P3 = [" + to_string(triangle[2].v[0]) + "," + to_string(triangle[2].v[1]) + "," + to_string(triangle[2].v[2]) + "]\n";
		text += "Distance = " + to_string(getDistance(mouseLocation, closestPoint)) + "\n";
		update_text(textID, text.c_str());
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) 
{
	if (key == (char)27)	//Pressing Escape Ends the game
	{
		exit(0);
	}
}

void keypressUp(unsigned char key, int x, int y)
{
}

void specialKeypress(int key, int x, int y) 
{
}

void specialKeypressUp(int key, int x, int y) 
{
}

void (mouse)(int x, int y)
{
	mouseLocation.v[0] = ((2 / width) * x) - 1;
	mouseLocation.v[1] = 1 - (((2 / height) * y));
}

#pragma endregion INPUT FUNCTIONS

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("GameApp");
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);


	// Tell glut where the display function is
	glutWarpPointer(width / 2, height / 2);
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);

	// Input Function Initialisers //
	glutKeyboardFunc(keypress);
	glutPassiveMotionFunc(mouse);
	glutSpecialFunc(specialKeypress);
	glutSpecialUpFunc(specialKeypressUp);
	glutKeyboardUpFunc(keypressUp);

	// A call to glewInit() must be done after glut is initialized!
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	init();
	textID = add_text("hi",
		-0.95, 0.9, fontSize, 1.0f, 1.0f, 1.0f, 1.0f);

	glutMainLoop();
	return 0;
}

void drawloop(mat4 view, mat4 proj, GLuint framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);								// enable depth-testing
	glDepthFunc(GL_LESS);									// depth-testing interprets a smaller value as "closer"
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear the color and buffer bits to make a clean slate
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);					//Create a background

	drawLine(simpleShaderID, identity_mat4(), identity_mat4(), mouseLocation, closestPoint, PURPLE);
	
	//Check whether the mouse is within the triangle face

	if (closestPoint == mouseLocation)
		drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[0], triangle[1], triangle[2], PURPLE);
	else
		drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[0], triangle[1], triangle[2], YELLOW);

	vec3 v12 = normalise(triangle[0] - triangle[1]);
	vec3 v23 = normalise(triangle[1] - triangle[2]);
	vec3 v31 = normalise(triangle[2] - triangle[0]);

	//First, check that the closest point isn't a vertice, then check whether its on an edge

	if (closestPoint != triangle[0] && closestPoint != triangle[1] && closestPoint != triangle[2] && closestPoint != mouseLocation)
	{
		//<p1, p2>
		if (dot(normalise(triangle[0] - closestPoint), v12) >= 0.9999)
			drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[0], triangle[1], mouseLocation, PURPLE);
		//<p2, p3>
		if (dot(normalise(triangle[1] - closestPoint), v23) >= 0.9999)
			drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[1], triangle[2], mouseLocation, PURPLE);
		//<p3, p1>
		if (dot(normalise(triangle[2] - closestPoint), v31) >= 0.9999)
			drawTriangle(simpleShaderID, identity_mat4(), identity_mat4(), triangle[2], triangle[0], mouseLocation, PURPLE);
	}

	string text;
	text += "P1 = [" + to_string(triangle[0].v[0]) + "," + to_string(triangle[0].v[1]) + "," + to_string(triangle[0].v[2]) + "]\n";
	text += "P2 = [" + to_string(triangle[1].v[0]) + "," + to_string(triangle[1].v[1]) + "," + to_string(triangle[1].v[2]) + "]\n";
	text += "P3 = [" + to_string(triangle[2].v[0]) + "," + to_string(triangle[2].v[1]) + "," + to_string(triangle[2].v[2]) + "]\n";
	text += "Distance = " + to_string(getDistance(mouseLocation, closestPoint)) + "\n";
	text += "v12 = [" + to_string(v12.v[0]) + "," + to_string(v12.v[1]) + "," + to_string(v12.v[2]) + "]\n";
	text += "v23 = [" + to_string(v23.v[0]) + "," + to_string(v23.v[1]) + "," + to_string(v23.v[2]) + "]\n";
	text += "v31 = [" + to_string(v31.v[0]) + "," + to_string(v31.v[1]) + "," + to_string(v31.v[2]) + "]\n";
	text += "dot(p0,v12) = " + to_string(dot(normalise(triangle[0] - closestPoint), v12)) + "\n";
	text += "dot(p0,v23) = " + to_string(dot(normalise(triangle[1] - closestPoint), v23)) + "\n";
	text += "dot(p0,v31) = " + to_string(dot(normalise(triangle[2] - closestPoint), v31)) + "\n";
	update_text(textID, text.c_str());


	
	
}

void rotateTriangle(float degrees)
{
	mat4 rotate = rotate_z_deg(identity_mat4(), degrees);
	for (int i = 0; i < 3; i++)
	{
		triangle[i] = multiply(rotate, triangle[i]);
	}
}