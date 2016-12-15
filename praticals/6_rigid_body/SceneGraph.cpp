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
		parent;
		localMatrix = modelToWorld;
		worldMatrix = modelToWorld;
		rotation;
		pare = false;

	}


	 ~SceneGraph()
	{
	}

	// release this object from memory
	void Release() { delete this; }

	const dvec3 GetPosition() const 
	{

		dvec3 position;

		position.x = worldMatrix[3][0];
		position.y = worldMatrix[3][1];
		position.z = worldMatrix[3][2];

		return position;

	}

	// update our scene node
	void Update()
	{
		if (pare == NULL) 
		{
			worldMatrix = parent ->worldMatrix * localMatrix;
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
	const dquat Rotate(SceneGraph* pNode, dquat rotate)
	{
		rotation = mat4_cast(rotate);

		if (pare == true)
		{
			worldMatrix = parent->worldMatrix * (localMatrix * rotation);

		}
		else
		{
			worldMatrix = (localMatrix * rotation);
		}

		return worldMatrix;
	}
	void setParent(SceneGraph* node)
	{
		parent = node;
		pare = true;
	}

protected:
	// list of children
	bool pare;
	std::list<SceneGraph*> nodes;
	dmat4 localMatrix;
	dmat4 worldMatrix;
	SceneGraph* parent;
	dmat4 rotation;

};