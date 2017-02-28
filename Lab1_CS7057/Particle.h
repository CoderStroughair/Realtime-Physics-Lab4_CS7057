#pragma once

#include "Utilities.h"
#include <list>

class Particle;
class RigidBody;
class Force;
class Drag;
class Gravity;
class ParticleSystem;

class Particle{
	vec3 initialPosition;
public:
	Mesh mesh;

	vec3 position;
	vec3 velocity; 
	vec3 force;
	float mass;
	int health;
	vec3 colour;
	float scale;

	Particle(Mesh& me, vec3 p, int m) {
		scale = rand() % 10 / 10.0f;
		health = 100;
		force = vec3(0.0, 0.0, 0.0);
		velocity = vec3(rand()%30-15, 50, rand()%30-15);
		mesh = me;
		position = p;
		initialPosition = p;
		mass = m;
		mass /= 100.0;
		colour = RED;
	}
	void addForce(vec3 f)
	{
		force += f;
	}

	void resolveForce(float delta)
	{
		vec3 ac = force/mass;
		velocity += ac*delta;
		position += velocity*delta;
	}

	void reset()
	{
		force = vec3(0.0, 0.0, 0.0);
		position = initialPosition;
		velocity = vec3(rand() % 30-15, 50, rand() % 30-15);
		health = rand()%50+50;
		scale = rand() % 1000 / 1000.0f;
		colour = RED;
	}

	void evolve()
	{
		if (health > 80)
			colour = RED;
		else if (health > 60)
			colour = PURPLE;
		else if (health > 40)
			colour = GREEN;
		else if (health > 20)
			colour = YELLOW;
		else
			colour = GREY;
	}
};

class RigidBody {
private:
	vec3 initialposition;
	int identifier;

public:
	vec3 colour = BLUE;
	//constants
	float mass;					//m									
	mat4 ibody;					//									
	mat4 ibodyInv;				//
	Mesh mesh;
	Mesh boundingSphere;

	vec3 position;				//x(t), ie, the center of mass
	mat4 orientationMat;		//R(t)
	vec3 linMomentum;			//P(t) = M*v(t)
	vec3 angMomentum;			//L(t) = I(t)*w(t)
	float radius = 0;

	mat4 iInv;
	vec3 velocity;				//Linear Velocity v(t) = P(t) / m
	vec3 angVelocity;			//w(t)
	
	vec3 force;
	vec3 torque;				//rho(t) = d/dt L(t) = SUM((pi - x(t))*fi)

	float boundingBox[6];

	RigidBody() {};
	RigidBody(vec3 x, vec3 P, vec3 L, float m, float h, float d, float w, Mesh _mesh) 
	{
		identifier = rand() % 1000000000;
		initialposition = x;

		boundingSphere.init(PARTICLE_MESH);
		float a = (1 / 12.0f) * m;
		ibody = mat4(a*(h*h+d*d),	0.0f,			0.0f,			0.0f,
						0.0f,		a*(w*w+d*d),	0.0f,			0.0f,
						0.0f,		0.0f,			a*(w*w+h*h),	0.0f,
						0.0f,		0.0f,			0.0f,			1.0f);
		ibodyInv = inverse(ibody);
		mass = m;
		mesh = _mesh;

		position = x;
		orientationMat = identity_mat4();
		linMomentum = P;
		angMomentum = L;
		
		velocity = linMomentum / mass;
		iInv = orientationMat * ibodyInv * transpose(orientationMat);
		angVelocity = multiply(iInv, angMomentum);

		force = vec3(0.0, 0.0, 0.0);
		torque = vec3(0.0, 0.0, 0.0);

		for (int i = 0; i < mesh.mesh_vertex_count; i+=3)
		{
			vec3 vertex = vec3(mesh.newpoints[i], mesh.newpoints[i + 1], mesh.newpoints[i + 2]);
			if (length(vertex) > radius)
			{
				radius = length(vertex);
			}
		}
		boundingSphere.scale_mesh(radius);
		mesh.update_mesh(orientationMat, initialposition);
		createBoundingBox();
	}

	void addForce(vec3 f, vec3 location)
	{
		force += f/2;
		torque += cross(location, force)/2;
	}

	void resolveForce(float delta)
	{
		if (force.v[0])
			cout <<"";
		linMomentum += force*delta;
		angMomentum += torque*delta;

		velocity = linMomentum / mass;
		iInv = orientationMat * ibodyInv * transpose(orientationMat);
		angVelocity = multiply(iInv, angMomentum);

		mat4 rDot = star(angVelocity*delta)*orientationMat;
		orientationMat.m[0] += rDot.m[0];
		orientationMat.m[1] += rDot.m[1];
		orientationMat.m[2] += rDot.m[2];
		orientationMat.m[3] = 0;

		orientationMat.m[4] += rDot.m[4];
		orientationMat.m[5] += rDot.m[5];
		orientationMat.m[6] += rDot.m[6];
		orientationMat.m[7] = 0;

		orientationMat.m[8] += rDot.m[8];
		orientationMat.m[9] += rDot.m[9];
		orientationMat.m[10] += rDot.m[10];
		orientationMat.m[11] = 0;

		orientationMat.m[12] += rDot.m[12];
		orientationMat.m[13] += rDot.m[13];
		orientationMat.m[14] += rDot.m[14];
		orientationMat.m[15] = 1;

		//Orthonormalisation
		vec3 Cx = vec3(orientationMat.m[0], orientationMat.m[1], orientationMat.m[2]) / length(vec3(orientationMat.m[0], orientationMat.m[1], orientationMat.m[2]));
		vec3 Cz = vec3(orientationMat.m[8], orientationMat.m[9], orientationMat.m[10]);
		vec3 Cy = cross(Cz, Cx);
		Cy = Cy / length(Cy);
		Cz = cross(Cx, Cy);
		Cz = Cz / length(Cz);
		orientationMat.m[0] = Cx.v[0];
		orientationMat.m[1] = Cx.v[1];
		orientationMat.m[2] = Cx.v[2];

		orientationMat.m[4] = Cy.v[0];
		orientationMat.m[5] = Cy.v[1];
		orientationMat.m[6] = Cy.v[2];

		orientationMat.m[8] = Cz.v[0];
		orientationMat.m[9] = Cz.v[1];
		orientationMat.m[10]= Cz.v[2];

		position += velocity * delta;

		if (force.v[0])
			cout << "";

		mesh.update_mesh(orientationMat, position);
	}

	mat4 star(vec3& a)
	{
		mat4 star = mat4(
			 0,		-a.v[2],	 a.v[1],	0,
			 a.v[2], 0,			-a.v[0],	0,
			-a.v[1], a.v[0],	 0,			0,
			 0,		 0,			 0,			0
		);
		return transpose(star);	//converting matrix into Anton's poxy way of doing things.
	}

	void reset(vec3 l, vec3 a)
	{
		position = initialposition;
		orientationMat = identity_mat4();
		linMomentum = l;
		angMomentum = a;

		velocity = linMomentum / mass;
		iInv = orientationMat * ibodyInv * transpose(orientationMat);
		angVelocity = multiply(iInv, angMomentum);

		force = vec3(0.0, 0.0, 0.0);
		torque = vec3(0.0, 0.0, 0.0);
	}

	void createBoundingBox()
	{
		boundingBox[0] = 999;
		boundingBox[1] = 999;
		boundingBox[2] = 999;
		boundingBox[3] = -999;
		boundingBox[4] = -999;
		boundingBox[5] = -999;

		for (int i = 0; i < mesh.newpoints.size(); i+=3)
		{
			if (mesh.newpoints[i] < boundingBox[0])
				boundingBox[0] = mesh.newpoints[i];
			if (mesh.newpoints[i] > boundingBox[3])
				boundingBox[3] = mesh.newpoints[i];

			if (mesh.newpoints[i+1] < boundingBox[1])
				boundingBox[1] = mesh.newpoints[i+1];
			if (mesh.newpoints[i+1] > boundingBox[4])
				boundingBox[4] = mesh.newpoints[i+1];

			if (mesh.newpoints[i+2] < boundingBox[2])
				boundingBox[2] = mesh.newpoints[i];
			if (mesh.newpoints[i+2] > boundingBox[5])
				boundingBox[5] = mesh.newpoints[i+2];
		}
	}

	bool operator==(const RigidBody b)
	{
		return (identifier == b.identifier);
	}
};

class Force {
public:
	virtual void applyForce(Particle& p) = 0;
	virtual void applyForce(RigidBody& b) = 0;
};

class Drag : public Force {
public:
	void applyForce(Particle& p) 
	{
		GLfloat constants = -0.5 * 1.225 * 0.47 * 3.14 * (0.1*p.scale) * (0.1*p.scale);
		vec3 velocity = vec3(p.velocity.v[0] * p.velocity.v[0], p.velocity.v[1] * p.velocity.v[1], p.velocity.v[2] * p.velocity.v[2]);
		p.addForce(velocity*constants);
	}

	void applyForce(RigidBody& b)
	{
		GLfloat constants = -0.5 * 1.225 * 1.05 * 1;
		vec3 velocity = vec3(b.velocity.v[0] * b.velocity.v[0], b.velocity.v[1] * b.velocity.v[1], b.velocity.v[2] * b.velocity.v[2]);
		b.addForce(velocity*constants, vec3(0.0, 0.0, 0.0));
	}
};

class Gravity : public Force {
public:

	void applyForce(Particle& p) 
	{
		GLfloat force_gravity = -9.81f*p.mass;
		p.addForce(vec3(0.0, force_gravity, 0.0));
	}

	void applyForce(RigidBody& b)
	{
		GLfloat force_gravity = -9.81*b.mass;
		b.addForce(vec3(0.0, force_gravity, 0.0), vec3(0.0, 0.0, 0.0));
	}
};

class ParticleSystem {
public:
	vector<Particle> particles;
	//vector<Force*> forces;
	int numParticles;
	int numForces;
	Drag d;
	Gravity g;

	void init(int p, Mesh& m)
	{
		numParticles = p;
		numForces = 0;
		int length = p / 20;
		for (int i = -length/2; i < length/2; i++)
		{
			for (int j = -10; j < 10; j++)
			{
				//Particle particle(m, vec3(rand() % 60 - 30, 20.0f, rand() % 60 - 30), rand() % 400+100);
				Particle particle(m, vec3(0.0f, 20.0f, 0.0f), rand() % 400 + 100);
				if (particle.mass <= 0)
					particle.mass = 1;
				particles.push_back(particle);
			}
		}
		numForces = 2;
	}

	void applyForces(float delta)
	{
		for (int i = 0; i < numParticles; i++)
		{
			if (particles[i].health <= 0)
				particles[i].reset();
			particles[i].force = vec3(0.0, 0.0, 0.0);
			d.applyForce(particles[i]);
			g.applyForce(particles[i]);

			particles[i].resolveForce(delta);
			particles[i].health--;
		}
	}

	void checkCollisions(vec3 point, vec3 normal, float delta)
	{
		float coRest = 0.6;
		for (int i = 0; i < numParticles; i++)
		{
			if (dot((particles[i].position - point), normal) < 0.00001f && dot(particles[i].velocity, normal) < 0.00001f)
			{
				vec3 deltaX = particles[i].position - normal * 1.4 * dot((particles[i].position - point), normal);
				particles[i].position = deltaX;
				particles[i].velocity = particles[i].velocity - (normal*dot(particles[i].velocity, normal))*(1+coRest);
			}
		}
	}
};

class RigidBodySystem{
public:
	vector<RigidBody> bodies;
	int numBodies;
	Drag d;
	Gravity g;

	RigidBodySystem() {};

	RigidBodySystem(int _numBodies, const char* mesh){
		numBodies = _numBodies;
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				Mesh m;
				m.init(mesh);
				RigidBody body(vec3(i*2, 15.0f, j*2), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), 100.0f, 1.0f, 1.0f, 1.0f, m);
				bodies.push_back(body);
			}
		}
	};

	void applyForces(float delta)
	{
		for (int i = 0; i < numBodies; i++)
		{
			bodies[i].force = vec3(0.0, 0.0, 0.0);
			g.applyForce(bodies[i]);
			//d.applyForce(bodies[i]);
			bodies[i].torque = vec3(-1.0, 5.0, 5.0);
			bodies[i].resolveForce(delta);
		}
	}

	void checkSphericalCollisions()
	{
		for (int i = 0; i < numBodies; i++)
		{
			bodies[i].colour = BLUE;
		}
		float coRest = 0.6;
		for (int i = 0; i < numBodies; i++)
		{
			for (int j = i+1; j < numBodies; j++)
			{
				if (i == j)
					continue;
				GLfloat distance = length(bodies[i].position - bodies[j].position);
				GLfloat radii = bodies[i].radius + bodies[j].radius;
				if (distance < radii)	//first, are their spheres overlapping?
				{
					bodies[i].colour = RED;
					bodies[j].colour = RED;
					//Next, checking if the boundary boxes overlap. If they do, turn the colour purple instead of red.
					checkBodyCollisions(bodies[i], bodies[j]);
				}
			}
		}
	}

	void checkBodyCollisions(RigidBody& a, RigidBody& b)
	{
		a.createBoundingBox();
		b.createBoundingBox();
		if (checkInterval(a.boundingBox[0], a.boundingBox[3], b.boundingBox[0], b.boundingBox[3]))
		{
			if (checkInterval(a.boundingBox[1], a.boundingBox[4], b.boundingBox[1], b.boundingBox[4]))
			{
				if (checkInterval(a.boundingBox[2], a.boundingBox[5], b.boundingBox[2], b.boundingBox[5]))
				{
 					a.colour = PURPLE;
					b.colour = PURPLE;
				}
			}
		}
	}
	bool checkInterval(float a1, float a2, float b1, float b2)
	{
		pair<float, float> a(a1, a2);
		pair<float, float> b(b1, b2);
		list<pair<float, float>> blist;
		blist.push_back(a);
		blist.push_back(b);
		blist.sort([](const pair<float, float> &a, const pair<float, float> &b) {return a.first < b.first; });
		return(blist.front().second > blist.back().first);
	}
	void checkPlaneCollisions(vec3 point, vec3 normal, float delta)
	{
		float coRest = 0.0;
		for (int i = 0; i < numBodies; i++)
		{
			if (dot((bodies[i].position - point), normal) <= bodies[i].radius+0.00001f && dot(bodies[i].linMomentum, normal) < 0.00001f)
			{
				vec3 deltaX = bodies[i].position - normal * 1.4 * dot((bodies[i].position - point), normal);
				bodies[i].position = deltaX;
				bodies[i].linMomentum -= (normal*dot(bodies[i].linMomentum, normal))*(1 + coRest);
			}
			/*if (dot((bodies[i].position - point), normal) < 0.00001f && dot(bodies[i].linMomentum, normal) < 0.00001f)
			{
				vec3 deltaX = bodies[i].position - normal * 1.4 * dot((bodies[i].position - point), normal);
				bodies[i].position = deltaX;
				bodies[i].linMomentum = bodies[i].linMomentum - (normal*dot(bodies[i].linMomentum, normal))*(1 + coRest);
			}*/
		}
	}
};
