#include <string>
#include <list>

class SceneGraph
{
public:

	SceneGraph()
	{
	}


	virtual ~SceneGraph()
	{
	}

	// release this object from memory
	void Release() {delete this;}

	// update our scene node
	virtual void Update()
	{
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

protected:
	// list of children
	std::list<SceneGraph*> nodes;
};