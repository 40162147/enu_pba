#include <string>
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;
using namespace std;


class SceneGraph
{
public:

	SceneGraph()
	{

	}

	SceneGraph(mat4 modelToWorld)
	{
		parent = NULL;
		localMatrix = modelToWorld;
		worldMatrix = modelToWorld;
		rotation;

	}


	 ~SceneGraph()
	{
	}

	// release this object from memory
	void Release() { delete this; }

	const dvec3 GetPosition() const 
	{

		dvec3 position;


		position.x = worldMatrix[0][3];
		position.y = worldMatrix[1][3];
		position.z = worldMatrix[2][3];

		return position;

	}

	// update our scene node
	void Update()
	{
		if (parent) 
		{
			worldMatrix = parent -> worldMatrix * localMatrix;
			
		}
		else 
		{ 
			worldMatrix = localMatrix;
					}

		// loop through the list and update the children
		for (std::list<SceneGraph*>::iterator i = nodes.begin(); i != nodes.end(); i++)
		{
			(*i)->Update();
		}
	}

	// destroy all the children
	void Destroy()
	{
		for (std::list<SceneGraph*>::iterator i = nodes.begin(); i != nodes.end(); i++)
		{
			(*i)->Release();
		}
		nodes.clear();
	}

	// add a child to our custody
	void AddChild(SceneGraph* pNode)
	{
		nodes.push_back(pNode);
	}
	void Rotate(SceneGraph* pNode, dquat rotate)
	{
		rotation = mat4_cast(rotate);

		if (parent)
		{
			worldMatrix = parent->worldMatrix * (localMatrix * rotation);

		}
		else
		{
			worldMatrix = (localMatrix * rotation);
		}
	}

protected:
	// list of children

	std::list<SceneGraph*> nodes;
	dmat4 localMatrix;
	dmat4 worldMatrix;
	SceneGraph* parent;
	dmat4 rotation;

};