#pragma once

#define GOLD	vec3(247.0f / 255.0f, 194.0f / 255.0f, 87.0f / 255.0f)
#define BROWN	vec3(51.0f / 255.0f, 25.0f / 255.0f, 0.0f / 255.0f)
#define BLACK	vec3(0.0f, 0.0f, 0.0f)
#define WHITE	vec3(1.0f, 1.0f, 1.0f)
#define CYAN	vec3(0.0f, 1.0f, 1.0f)
#define MAGENTA	vec3(1.0f, 0.0f, 1.0f)
#define YELLOW	vec3(1.0f, 1.0f, 0.0f)
#define BLUE	vec3(0.0f, 0.0f, 1.0f)
#define RED		vec3(1.0f, 0.0f, 0.0f)
#define GREY	vec3(0.7f, 0.7f, 0.7f)
#define GREEN	vec3(0.0f, 1.0f, 0.0f)
#define PURPLE	vec3(102.0f/255.0f, 0.0, 204.0f/255.0f)

/*----------------------------------------------------------------------------
							SHADER VARIABLES
----------------------------------------------------------------------------*/
#define SKY_VERT		"../Shaders/skyboxVertexShader.txt"
#define SKY_FRAG		"../Shaders/skyboxFragmentShader.txt"

#define NOTEXTURE_VERT	"../Shaders/noTextureVertexShader.txt"
#define NOTEXTURE_FRAG	"../Shaders/noTextureFragmentShader.txt"

#define REFLECTIVE_FRAG "../Shaders/reflectiveFragmentShader.txt"

#define FRESNEL_FRAG	"../Shaders/fresnelFragmentShader.txt"

#define TEXTURE_VERT	"../Shaders/textureVertexShader.txt"
#define TEXTURE_FRAG	"../Shaders/textureFragmentShader.txt"

#define NORMAL_VERT		"../Shaders/normalisedVertexShader.txt"
#define NORMAL_FRAG		"../Shaders/normalisedFragmentShader.txt"

#define MIRROR_VERT		"../Shaders/mirrorVertexShader.txt"
#define MIRROR_FRAG		"../Shaders/mirrorFragmentShader.txt"

#define TEST_VERT		"../Shaders/testVertex.txt"
#define TEST_FRAG		"../Shaders/testFragment.txt"

#define DEBUG_VERT		"../Shaders/debugSkyboxVertex.txt"
#define DEBUG_FRAG		"../Shaders/debugSkyboxFragment.txt"

#define SIMPLE_VERT		"../Shaders/simple.vert"
#define SIMPLE_FRAG		"../Shaders/simple.frag"

/*----------------------------------------------------------------------------
				MESH AND TEXTURE VARIABLES
----------------------------------------------------------------------------*/

#define MONKEYHEAD_MESH	"../Meshes/suzanne.obj"
#define SPHERE_MESH		"../Meshes/polysphere.obj"
#define GEM_MESH		"../Meshes/gem.obj"
#define WALL_MESH		"../Meshes/wall.obj"
#define BEAR_MESH		"../Meshes/bear.obj"
#define LAMP_MESH		"../Meshes/lamp.obj"
#define TORCH_MESH		"../Meshes/torch.obj"
#define SIGN_MESH		"../Meshes/sign.obj"
#define BUDDHA_MESH		"../Meshes/buddha.dae"
#define PARTICLE_MESH	"../Meshes/particle.dae"
#define CUBE_MESH		"../Meshes/cube.obj"


#define BEAR_TEXTURE	"../Textures/bear.tga"
#define BEAR_TEXTURE2	"../Textures/bear2.tga"
#define CUBE_TEXTURE	"../Textures/me.png"
#define REFLECT_TEXTURE	"../Textures/reflection.png"
#define REFRACT_TEXTURE	"../Textures/refraction.png"
#define NORMAL_TEXTURE	"../Textures/normalmapped.png"
#define BRICK_TEXTURE	"../Textures/brickwall.jpg"
#define BRICK_NORMAL	"../Textures/brickwall_normal.jpg"

GLfloat vertices[] = {
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  0.0f, -1.0f,
	-10.0f, -10.0f, -10.0f,  0.0f,  0.0f, -1.0f,

	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  0.0f,  1.0f,
	-10.0f, -10.0f,  10.0f,  0.0f,  0.0f,  1.0f,

	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f, -10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f, -10.0f,  10.0f, -1.0f,  0.0f,  0.0f,
	-10.0f,  10.0f,  10.0f, -1.0f,  0.0f,  0.0f,

	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  1.0f,  0.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  1.0f,  0.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  1.0f,  0.0f,  0.0f,

	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f,  10.0f,  0.0f, -1.0f,  0.0f,
	-10.0f, -10.0f, -10.0f,  0.0f, -1.0f,  0.0f,

	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f,  10.0f,  0.0f,  1.0f,  0.0f,
	-10.0f,  10.0f, -10.0f,  0.0f,  1.0f,  0.0f
};