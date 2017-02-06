#pragma once
#include "Utilities.h"

vec3 getClosestPointTriangle(vec3 triangle[], vec3 p0);
vec3 getClosestPointLine(vec3 line[], vec3 p0);
vec3 getClosestPointPlane(vec3 normal, vec3 p1, vec3 p0);
float getDistance(vec3 v0, vec3 v1);
void getClosestTriangleTriangle(vec3 first[], vec3 second[], vec3 &point1, vec3 &point2);

vec3 getClosestPointTriangle(vec3 triangle[], vec3 p0)
{
	vec3 p1 = triangle[0];
	vec3 p2 = triangle[1];
	vec3 p3 = triangle[2];

	//Check Each Voronoi Region

	//Vertex Regions:

	//p1
	if (dot(p0 - p1, p3 - p1) <= 0 && dot(p0 - p1, p2 - p1) <= 0)
	{
		return p1;
	}
	//p2
	if (dot(p0 - p2, p3 - p2) <= 0 && dot(p0 - p2, p1 - p2) <= 0)
	{
		return p2;
	}
	//p3
	if (dot(p0 - p3, p2 - p3) <= 0 && dot(p0 - p3, p1 - p3) <= 0)
	{
		return p3;
	}

	//Edge Regions:

	//<p1, p2>
	if (dot(cross(cross(p3 - p2, p1 - p2), p1 - p2), p0 - p2) >= 0 && dot(p0 - p1, p2 - p1) >= 0 && dot(p0 - p2, p1 - p2) >= 0)
	{
		vec3 line[] = { p1, p2 };
		return getClosestPointLine(line, p0);
	}
	// <p2, p3>
	if (dot(cross(cross(p1 - p3, p2 - p3), p2 - p3), p0 - p3) >= 0 && dot(p0 - p2, p3 - p2) >= 0 && dot(p0 - p3, p2 - p3) >= 0)
	{
		vec3 line[] = { p2, p3 };
		return getClosestPointLine(line, p0);
	}
	// <p3, p1>
	if (dot(cross(cross(p2 - p1, p3 - p1), p3 - p1), p0 - p1) >= 0 && dot(p0 - p3, p1 - p3) >= 0 && dot(p0 - p1, p3 - p1) >= 0)
	{
		vec3 line[] = { p3, p1 };
		return getClosestPointLine(line, p0);
	}

	//It must be the Face Region
	return getClosestPointPlane(cross(p2, p1), p1, p0);
}
vec3 getClosestPointLine(vec3 line[], vec3 p0)
{
	vec3 u = line[1] - line[0];
	return line[0] + normalise(u)*(dot(p0 - line[0], normalise(u)));
}
vec3 getClosestPointPlane(vec3 normal, vec3 p1, vec3 p0)
{
	normal = normalise(normal);
	return (p0 - normal*dot((p0 - p1), normal));
}
float getDistance(vec3 v0, vec3 v1)
{
	float x_sq = (v0.v[0] - v1.v[0]) * (v0.v[0] - v1.v[0]);
	float y_sq = (v0.v[1] - v1.v[1]) * (v0.v[1] - v1.v[1]);
	float z_sq = (v0.v[2] - v1.v[2]) * (v0.v[2] - v1.v[2]);
	return sqrt(x_sq + y_sq + z_sq);
}

void getClosestTriangleTriangle(vec3 first[], vec3 second[], vec3 &point1, vec3 &point2)
{

}


