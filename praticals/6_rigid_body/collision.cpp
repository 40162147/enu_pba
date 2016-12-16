#include "collision.h"
#include "cPhysicsComponents.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
using namespace std;
using namespace glm;
namespace collision {

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cSphereCollider &c1, const cSphereCollider &c2) {
  const dvec3 p1 = c1.GetParent()->GetPosition();
  const dvec3 p2 = c2.GetParent()->GetPosition();
  const dvec3 d = p2 - p1;
  const double distance = glm::length(d);
  const double sumRadius = c1.radius + c2.radius;
  if (distance < sumRadius) {
    auto depth = sumRadius - distance;
    auto norm = -glm::normalize(d);
    auto pos = p1 - norm * (c1.radius - depth * 0.5f);
    civ.push_back({&c1, &c2, pos, norm, depth});
    return true;
  }
  return false;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cCylinderCollider &c1, const cCylinderCollider &c2)
{
	return false;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cCylinderCollider &c1, const cPlaneCollider &c2) 
{
	
	
	const dvec3 sp = c1.GetParent()->GetPosition();
	const dvec3 pp = c2.GetParent()->GetPosition();
	float height = c1.GetParent()->Getheightm();
	float dia = c1.GetParent()->GetDiam();

	const dvec3 endPoint = dvec3(c1.GetParent()->GetPosition().x, sp.y + height / 2, c1.GetParent()->GetPosition().z);
	const dvec3 startPoint = dvec3(c1.GetParent()->GetPosition().x, sp.y - (height / 2), c1.GetParent()->GetPosition().z);


	
	// Calculate a vector from a point on the plane to the center of the sphere


	/*
	const dvec3 ab = endPoint - startPoint;
	const dvec3 ac = c2.normal - startPoint;
	const dvec3 bc = c2.normal - endPoint;

	float e = dot(ac, ab);
	float f = dot(ab, ab);

	if (e <= 0.0f)
	{
		dist = dot(ac, ac);
	}
	else if (e >= f)
	{
		dist = dot(bc, bc);
	}
	else
	{
		dist = dot(ac, ac) - e * e / f;
	}

	*/

	
	// local coords on cylinder rectangle
	dvec3 points[8] = { dvec3(c1.dia / 2, sp.y + (c1.height / 2), c1.dia / 2), dvec3(-(c1.dia / 2), sp.y + (c1.height / 2), c1.dia / 2),
						dvec3(c1.dia / 2, sp.y -(c1.height / 2), c1.dia / 2), dvec3(-(c1.dia / 2), sp.y - (c1.height / 2), c1.dia / 2),
						dvec3(c1.dia / 2, sp.y + (c1.height / 2), -(c1.dia / 2)), dvec3(-(c1.dia / 2),sp.y + (c1.height / 2), -(c1.dia / 2)),
						dvec3(c1.dia / 2, sp.y - (c1.height / 2), -(c1.dia / 2)), dvec3(-(c1.dia / 2), sp.y -(c1.height / 2), -(c1.dia / 2)) };

	// transfrom to global


	double distances[8];
	bool isCollided = false;
	for (int i = 0; i < 8; i++) 
	{
		dvec3 planeNormal = c2.normal;

		distances[i] = dot(pp, planeNormal) - dot(points[i], planeNormal);

		if (distances[i] > 0)
		{

			civ.push_back({ &c2, &c1, points[i] + planeNormal * distances[i], planeNormal, distances[i] });
			isCollided = true;

		}
	}

	return isCollided;
}






bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cSphereCollider &s, const cPlaneCollider &p) {
  const dvec3 sp = s.GetParent()->GetPosition();
  const dvec3 pp = p.GetParent()->GetPosition();

  // Calculate a vector from a point on the plane to the center of the sphere
  const dvec3 vecTemp(sp - pp);

  // Calculate the distance: dot product of the new vector with the plane's normal
  double distance = dot(vecTemp, p.normal);

  if (distance <= s.radius) {
    civ.push_back({&s, &p, sp - p.normal * distance, p.normal, s.radius - distance});
    return true;
  }

  return false;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cSphereCollider &c1, const cBoxCollider &c2) {
  const dvec3 sp = c1.GetParent()->GetPosition();
  const dvec3 bp = c2.GetParent()->GetPosition();
  if (length(sp - bp) < c1.radius + c2.radius) {
    // TODO
    cout << "Sphere Box" << endl;
    // return true;
  }

  return false;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cPlaneCollider &c1, const cPlaneCollider &c2) {
  // TODO
  cout << "PLANE PLANE" << endl;
  return false;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cCylinderCollider &c2, const cBoxCollider &c1) 
{
	return false;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cCylinderCollider &c2, const cSphereCollider &c1)
{

	const dvec3 sp = c1.GetParent()->GetPosition();
	const dvec3 pp = c2.GetParent()->GetPosition();
	
	const dvec3 endPoint = dvec3(c2.GetParent()->GetPosition().x, c2.GetParent()->GetPosition().y + c2.height / 2, c2.GetParent()->GetPosition().z);
	const dvec3 startPoint = dvec3(c2.GetParent()->GetPosition().x, c2.GetParent()->GetPosition().y - (c2.height / 2), c2.GetParent()->GetPosition().z);

	const dvec3 ab = endPoint - startPoint;
	float distanceFactorFromEP1 = dot(sp - startPoint, ab) / dot(ab, ab);
	dvec3 abd;
	abd.x = ab.x * distanceFactorFromEP1;
	abd.y = ab.y * distanceFactorFromEP1;
	abd.z = ab.z * distanceFactorFromEP1;


	
	if (distanceFactorFromEP1 < 0)
	{
		distanceFactorFromEP1 = 0.0f;
	}
	if (distanceFactorFromEP1 > 1)
	{
		distanceFactorFromEP1 = 1.0f;
	}
	const dvec3 closestPoint = startPoint + abd;

	const dvec3 collisionVector = sp - closestPoint;
	double distance = glm::length(collisionVector);
	dvec3 cold;
	cold.x = collisionVector.x / distance;
	cold.y = collisionVector.y /  distance;
	cold.z = collisionVector.z / distance;

	const dvec3 collisionNormal = cold;

	if (distance < c1.radius + (c2.dia/2))
	{
		//collision occurred. use collisionNormal to reflect sphere off cyl

		float factor = dot(c1.GetParent()->getComponent<cRigidSphere>()->angVelocity, collisionNormal);
		dvec3 change;
		change.x = collisionNormal.x * (2*factor);
		change.y = collisionNormal.y * (2 * factor);
		change.z = collisionNormal.z * (2 * factor);

		cout << "some" << endl;
		
		civ.push_back({ &c1, &c2,  sp - collisionVector * distance, collisionVector, c1.radius - distance });

		return true;
	}

	return false;
	
}


bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cPlaneCollider &p, const cBoxCollider &b) {
  const dvec3 pp = p.GetParent()->GetPosition();
  const dvec3 bp = b.GetParent()->GetPosition();

  // local coords on cube
  dvec3 points[8] = {dvec3(b.radius, b.radius, b.radius),   dvec3(-b.radius, b.radius, b.radius),
                     dvec3(b.radius, -b.radius, b.radius),  dvec3(-b.radius, -b.radius, b.radius),
                     dvec3(b.radius, b.radius, -b.radius),  dvec3(-b.radius, b.radius, -b.radius),
                     dvec3(b.radius, -b.radius, -b.radius), dvec3(-b.radius, -b.radius, -b.radius)};

  // transfrom to global
  const mat4 m = glm::translate(bp) * mat4_cast(b.GetParent()->GetRotation());
  for (int i = 0; i < 8; i++) {
    points[i] = dvec3(m * dvec4(points[i], 1.0));
  }

  // For each point on the cube, which side of cube are they on?
  double distances[8];
  bool isCollided = false;
  for (int i = 0; i < 8; i++) {
    dvec3 planeNormal = p.normal;

    distances[i] = dot(pp, planeNormal) - dot(points[i], planeNormal);

    if (distances[i] > 0) {
      //	 cout << "CuboidPlane!\n";
      civ.push_back({&p, &b, points[i] + planeNormal * distances[i], planeNormal, distances[i]});
      isCollided = true;
    }
  }

  return isCollided;
}

bool IsCollidingCheck(std::vector<collisionInfo> &civ, const cBoxCollider &c1, const cBoxCollider &c2) {
  cout << "Box Box" << endl;
  return false;
}

bool IsColliding(std::vector<collisionInfo> &civ, const cCollider &c1, const cCollider &c2) {
  enum shape { UNKOWN = 0, PLANE, SPHERE, BOX, CYLINDER};
  shape s1 = UNKOWN;
  shape s2 = UNKOWN;
  if (dynamic_cast<const cSphereCollider *>(&c1)) 
  {
    s1 = SPHERE;
  } 
  else if (dynamic_cast<const cPlaneCollider *>(&c1)) 
  {
    s1 = PLANE;
  } 
  else if (dynamic_cast<const cBoxCollider *>(&c1)) 
  {
    s1 = BOX;
  }
  else if (dynamic_cast<const cCylinderCollider *>(&c1))
  {
	s1 = CYLINDER;
  }

  if (dynamic_cast<const cSphereCollider *>(&c2)) 
  {
    s2 = SPHERE;
  } 
  else if (dynamic_cast<const cPlaneCollider *>(&c2)) 
  {
    s2 = PLANE;
  } 
  else if (dynamic_cast<const cBoxCollider *>(&c2)) 
  {
    s2 = BOX;
  }
  else if (dynamic_cast<const cCylinderCollider *>(&c2))
  {
	s2 = CYLINDER;
  }

  if (!s1 || !s2) 
  {
    cout << "Routing Error" << endl;
    return false;
  }
  if (s1 == PLANE) 
  {
    if (s2 == PLANE) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
    } 
	else if (s2 == SPHERE)
	{
      return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
    } 
	else if (s2 == BOX) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
    } 
	else if (s2 == CYLINDER)
	{
		return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
	}
	else 
	{
      cout << "Routing Error" << endl;
      return false;
    }
  } 
  else if (s1 == SPHERE) 
  {
    if (s2 == PLANE) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
    } 
	else if (s2 == SPHERE) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cSphereCollider &>(c2));
    } 
	else if (s2 == BOX) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
    } 
	
	else if (s2 == CYLINDER)
	{
		return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c2), dynamic_cast<const cSphereCollider &>(c1));
	}
	
	else 
	{
      cout << "Routing Error" << endl;
      return false;
    }
  } 
  else if (s1 == BOX) 
  {
    if (s2 == PLANE) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cPlaneCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
    } 
	else if (s2 == SPHERE) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cSphereCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
    }
	else if (s2 == BOX) 
	{
      return IsCollidingCheck(civ, dynamic_cast<const cBoxCollider &>(c2), dynamic_cast<const cBoxCollider &>(c1));
    } 
	else if (s2 == CYLINDER)
	{
		return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
	}
	
	else 
	{
      cout << "Routing Error" << endl;
      return false;
    }
  }
  
  else if (s1 == CYLINDER) 
  {
	  if (s2 == PLANE) 
	  {
		  return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c1), dynamic_cast<const cPlaneCollider &>(c2));
	  }
	  else if (s2 == CYLINDER)
	  {
		  return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c2), dynamic_cast<const cCylinderCollider &>(c1));
	  }
	  else if (s2 == SPHERE)
	  {
		  return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c1), dynamic_cast<const cSphereCollider &>(c2));
	  }
	  else if (s2 == BOX)
	  {
		  return IsCollidingCheck(civ, dynamic_cast<const cCylinderCollider &>(c1), dynamic_cast<const cBoxCollider &>(c2));
	  }
	  
	  else 
	  {
		  cout << "Routing Error" << endl;
		  return false;
	  }
  }
  
  return false;
}




}