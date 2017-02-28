
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

Mesh cubeMapID, cubeID;

/*----------------------------------------------------------------------------
							CAMERA VARIABLES
----------------------------------------------------------------------------*/

vec3 startingPos = { 0.0f, 0.0f, 10.0f };
GLfloat pitCam = 0, yawCam = 0, rolCam = 0, frontCam = 0, sideCam = 0, speed = 1;
float rotateY = 50.0f, rotateLight = 0.0f;
EulerCamera cam(startingPos, 270.0f, 0.0f, 0.0f);

/*----------------------------------------------------------------------------
								SHADER VARIABLES
----------------------------------------------------------------------------*/
GLuint simpleShaderID, noTextureShaderID, cubeMapShaderID;
Shader shaderFactory;
/*----------------------------------------------------------------------------
							OTHER VARIABLES
----------------------------------------------------------------------------*/

const char* atlas_image = "../freemono.png";
const char* atlas_meta = "../freemono.meta";

float fontSize = 25.0f;
int textID = -1;
bool pause = false;

RigidBodySystem bodySystem;
/*----------------------------------------------------------------------------
						FUNCTION DEFINITIONS
----------------------------------------------------------------------------*/

void drawloop(mat4 view, mat4 proj, GLuint framebuffer);

/*--------------------------------------------------------------------------*/

void init()
{
	if (!init_text_rendering(atlas_image, atlas_meta, width, height)) 
	{
		fprintf(stderr, "ERROR init text rendering\n");
		exit(1);
	}
	simpleShaderID = shaderFactory.CompileShader(SIMPLE_VERT, SIMPLE_FRAG);
	noTextureShaderID = shaderFactory.CompileShader(NOTEXTURE_VERT, NOTEXTURE_FRAG);
	cubeMapShaderID = shaderFactory.CompileShader(SKY_VERT, SKY_FRAG);

	cubeMapID.initCubeMap(vertices, 36, "desert");
	cubeID.init(CUBE_MESH);

	bodySystem = RigidBodySystem(10, CUBE_MESH);
}

void display() 
{
	mat4 proj = perspective(87.0, (float)width / (float)(height), 1, 1000.0);
	mat4 view = look_at(cam.getPosition(), cam.getPosition() + cam.getFront(), cam.getUp());
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
		delta = 0.03f;
		last_frame = curr_time;
		glutPostRedisplay();

		cam.movForward(frontCam*speed);
		cam.movRight(sideCam*speed);
		cam.changeFront(pitCam, yawCam, rolCam);
		update_text(textID, "Paused");
		if (!pause)
		{
			string output = "EULER_MODE: Front: [" + to_string(cam.getFront().v[0]) + ", " + to_string(cam.getFront().v[1]) + ", " + to_string(cam.getFront().v[2]) + "]\n";
			output += "Position: [" + to_string(cam.getPosition().v[0]) + ", " + to_string(cam.getPosition().v[1]) + ", " + to_string(cam.getPosition().v[2]) + "]\n";
			output += "Up: [" + to_string(cam.getUp().v[0]) + ", " + to_string(cam.getUp().v[1]) + ", " + to_string(cam.getUp().v[2]) + "]\n";
			output += "Pitch: " + to_string(cam.pitch) + "\n";
			output += "Yaw: " + to_string(cam.yaw) + "\n";
			output += "Roll: " + to_string(cam.roll) + "\n";
			update_text(textID, output.c_str());

			bodySystem.applyForces(delta);
			bodySystem.checkSphericalCollisions();
			bodySystem.checkPlaneCollisions(vec3(5.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0), delta);
			bodySystem.checkPlaneCollisions(vec3(-5.0, 0.0, 0.0), vec3(-1.0, 1.0, 0.0), delta);
			bodySystem.checkPlaneCollisions(vec3(5.0, 20.0, 0.0), vec3(1.0, -1.0, 0.0), delta);
			bodySystem.checkPlaneCollisions(vec3(-5.0, 20.0, 0.0), vec3(-1.0, -1.0, 0.0), delta);

		}
	}
	
}

#pragma region INPUT FUNCTIONS

void keypress(unsigned char key, int x, int y) {
	switch (key)
	{
	case ((char)27):
		exit(0);
		break;
	case('w'):
	case('W'):
		frontCam = 1;
		printf("Moving Forward\n");
		break;
	case('s'):
	case('S'):
		frontCam = -1;
		printf("Moving Backward\n");
		break;
	case('a'):
	case('A'):
		sideCam = -1;
		printf("Moving Left\n");
		break;
	case('d'):
	case('D'):
		sideCam = 1;
		printf("Moving Right\n");
		break;
	case('q'):
	case('Q'):
		rolCam = -1;
		printf("Spinning Negative Roll\n");
		break;
	case('e'):
	case('E'):
		rolCam = 1;
		printf("Spinning Positive Roll\n");
		break;
	}
}

void keypressUp(unsigned char key, int x, int y){
	switch (key)
	{
	case('w'):
	case('W'):
	case('s'):
	case('S'):
		frontCam = 0;
		break;
	case('a'):
	case('A'):
	case('d'):
	case('D'):
		sideCam = 0;
		break;
	case('q'):
	case('Q'):
	case('e'):
	case('E'):
		rolCam = 0;
		break;
	case(' '):
		pause = !pause;
		break;
	}
}

void specialKeypress(int key, int x, int y){
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 4;
		break;
	case (GLUT_KEY_LEFT):
		printf("Spinning Negative Yaw\n");
		yawCam = -1;
		break;
	case (GLUT_KEY_RIGHT):
		printf("Spinning Positive Yaw\n");
		yawCam = 1;
		break;
	case (GLUT_KEY_UP):
		printf("Spinning Positive Pit\n");
		pitCam = 1;
		break;
	case (GLUT_KEY_DOWN):
		printf("Spinning Negative Pit\n");
		pitCam = -1;
		break;
	}
}

void specialKeypressUp(int key, int x, int y){
	switch (key)
	{
	case (GLUT_KEY_SHIFT_L):
	case (GLUT_KEY_SHIFT_R):
		speed = 1;
		break;
	case (GLUT_KEY_LEFT):
	case (GLUT_KEY_RIGHT):
		yawCam = 0;
		break;
	case (GLUT_KEY_UP):
	case (GLUT_KEY_DOWN):
		pitCam = 0;
		break;
	}
}

void (mouse)(int x, int y){
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

	// light properties
	vec3 Ls = vec3(0.0001f, 0.0001f, 0.0001f);	//Specular Reflected Light
	vec3 Ld = vec3(0.5f, 0.5f, 0.5f);	//Diffuse Surface Reflectance
	vec3 La = vec3(1.0f, 1.0f, 1.0f);	//Ambient Reflected Light
	vec3 light = vec3(5 * sin(rotateLight), 10, -5.0f*cos(rotateLight));//light source location
	vec3 coneDirection = light + vec3(0.0f, -1.0f, 0.0f);
	float coneAngle = 10.0f;
	// object colour
	vec3 Ks = vec3(0.1f, 0.1f, 0.1f); // specular reflectance
	vec3 Kd = BLUE;
	vec3 Ka = BLUE; // ambient reflectance
	float specular_exponent = 0.000000005f; //specular exponent - size of the specular elements

	mat4 model = identity_mat4();

	drawCubeMap(cubeMapShaderID, cubeMapID.tex, view, proj, model, vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), cam, cubeMapID, GL_TRIANGLES);
	
	for (int i = 0; i < bodySystem.numBodies; i++)
	{
		drawObject(noTextureShaderID, view, proj, identity_mat4(), light, Ls, La, Ld, Ks, bodySystem.bodies[i].colour, bodySystem.bodies[i].colour, specular_exponent, cam, bodySystem.bodies[i].mesh, coneAngle, coneDirection, GL_QUADS);
		model = translate(identity_mat4(), bodySystem.bodies[i].position);
		if (bodySystem.bodies[i].colour == RED)
			drawObject(noTextureShaderID, view, proj, model, light, Ls, La, Ld, Ks, bodySystem.bodies[i].colour, bodySystem.bodies[i].colour, specular_exponent, cam, bodySystem.bodies[i].boundingSphere, coneAngle, coneDirection, GL_TRIANGLES);
	}
}