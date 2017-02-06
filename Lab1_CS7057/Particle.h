#pragma once

#include "Utilities.h"

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
	vec3 initialposition;

public:
	//constants
	float mass;					//m									
	mat4 ibody;					//									
	mat4 ibodyInv;				//
	Mesh mesh;

	vec3 position;				//x(t), ie, the center of mass
	mat4 orientationMat;		//R(t)
	vec3 linMomentum;			//P(t) = M*v(t)
	vec3 angMomentum;			//L(t) = I(t)*w(t)


	mat4 iInv;
	vec3 velocity;				//Linear Velocity v(t) = P(t) / m
	vec3 angVelocity;			//w(t)
	
	vec3 force;
	vec3 torque;				//rho(t) = d/dt L(t) = SUM((pi - x(t))*fi)

	//mat3 inertiaTensor;		//I(t)	- Varies in World Space:	|	Ixx		Ixy		Ixz	|
								//									|	Iyx		Iyy		Iyz	|
								//									|	Izx		Izy		Izz	|
								//Diagonal Terms:	Ixx = M * IntegralOverV (Y^2 + z^2) dV
								//Off-Diagonal:		Ixy = -M * IntegralOverV (x*y) dV
								//Body Space - I(t) = R(t)IbodyR(T)^T

	//Rate of change of Orientation = w(t)*R(t)  = (d/dt)R(t) = |	 0		-wz(t)	 wy(t)	0	|
	//															|	 wz(t)	 0		-wx(t)	0	|	*	R(t)
	//															|	-wy(t)	 wx(t)	 0		0	|
	//															|	 0		 0		 0		0	|
	
	RigidBody() {};
	RigidBody(vec3 x, vec3 P, vec3 L, float m, float h, float d, float w, Mesh _mesh) 
	{
		initialposition = x;

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
	}

	void addForce(vec3 f, vec3 location)
	{
		force += f;
		torque += cross(location, force);
	}

	void resolveForce(float delta)
	{
		if (force.v[0])
			cout <<"";
		linMomentum *= 0.0;
		angMomentum *= 0.0;
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
};

class Force {
public:
	virtual void applyForce(Particle& p) = 0;
};

class Drag : public Force {
public:
	void applyForce(Particle& p) 
	{
		GLfloat constants = 0.5 * 1.225 * 0.47 * 3.14 * (0.1*p.scale) * (0.1*p.scale);
		vec3 velocity = vec3(p.velocity.v[0] * p.velocity.v[0], p.velocity.v[1] * p.velocity.v[1], p.velocity.v[2] * p.velocity.v[2]);
		p.addForce(velocity*constants);
			//force -= velocity*constants;
	};
};

class Gravity : public Force {
public:
	void applyForce(Particle& p) 
	{
		float force_gravity = -9.81f*p.mass;
		p.addForce(vec3(0.0, force_gravity, 0.0));
	};
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
				particles[i].velocity = particles[i].velocity - (normal*dot(particles[i].velocity, normal))*(1+coRest);//vec3(particles[i].velocity.v[0] * normal.v[0], particles[i].velocity.v[1] * normal.v[1], particles[i].velocity.v[2] * normal.v[2]);
			}
		}
	}
};
