#include "physics.h"
#include "cPhysicsComponents.h"
#include <phys_utils.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "SceneGraph.cpp"
#include <vector>
#include <graphics_framework.h>
#include "game.h"


using namespace graphics_framework;
using namespace std;
using namespace glm;

class LSystem
{

	static vector<unique_ptr<Entity>> SceneList;
	int iterations = 4;
	float angle = 22.5f;
	SceneGraph base;
	std::vector<SceneGraph*> nodes;
	mat4 modelToWorld;
	mat4 branches[2048];

	int array_top;
	float branchSize = 5.0f;

	std::string axiom = "F";
	std::string rule1 = "FF-[-F+F+F]+[+F-F-F]";


	unique_ptr<Entity> CreateCylinder(const vec3 &position, float dia, float height)
	{
		unique_ptr<Entity> ent(new Entity());
		ent->SetPosition(position);
		ent->SetRotation(angleAxis(0.0f, vec3(1, 0, 0)));
		ent->SetHeightm(height);
		ent->SetDiam(dia);
		unique_ptr<Component> physComponent(new cRigidCylinder());
		unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::CYLINDER));
		renderComponent->SetColour(phys::RandomColour());
		ent->AddComponent(physComponent);
		ent->SetName("Cyl");
		ent->AddComponent(unique_ptr<Component>(new cCylinderCollider()));
		ent->AddComponent(unique_ptr<Component>(move(renderComponent)));

		return ent;
	}






	LSystem::~LSystem()
	{
	}


	void drawBase(SceneGraph *base)
	{
		SceneGraph *node = new SceneGraph(modelToWorld);
		nodes.push_back(node);

		if (nodes.size() == 1)
		{
			app_scene->AddChild(node);
			base = node;
		}
		else 
		{
			base->AddChild(node); 
		}
	
		node = rotate(node, 90.0f, vec3(1.0f, 0.0f, 0.0f));

		SceneList.push_back(move(CreateCylinder({ 0, 0, 0 }, 2.0f, 10.0f)));

	}

	void push_matrix(mat4 matrix)
	{
		branches[array_top] = matrix; //put the current state of modelToWorld into the top of the matrix stack
		array_top++;
	}

	//retrieves the matrix position that were stored in the array of matrices
	mat4 pop_matrix()
	{
		return branches[--array_top]; //bring back the pushed matrix
	}

	void simulate(){
		//reset everything and set values
		modelToWorld = mat4(1.0f);
		std::vector<int> index;


		//for the amount of iterations specified...
		for (int i = 0; i < iterations; i++)
		{
			//after initial iteration, run through the rule string and look for letters to be replaced and push their indexes into an array.
			if (i > 0)
			{
				for (int j = 0; j < axiom.length(); j++)
				{
					if (axiom[j] == 'F')
					{
						index.push_back(j);
					}
				}
				//then for each index specifed replace the letter with the rule specified. LOOP MUST RUN BACKWARDS.
				for (int k = index.size() - 1; k >= 0; k--)
				{
					if (axiom[index[k]] == 'F')
					{
						axiom.replace(index[k], 1, rule1);
					}
				}

				index.clear();
			}

			//then just go through each letter in the drawing rule string and do action associated with it
			for (int l = 0; l <= axiom.length(); l++)
			{
				//create a cylinder and move up
				if (axiom[l] == 'F')
				{
					if (nodes.size() == 1)
					{
						drawBase(&base);
						modelToWorld = translate(modelToWorld, vec3(0, (branchSize + (branchSize / 2)), 0));
					}
				}

				if (axiom[l] == '+')
				{
					modelToWorld = rotate(modelToWorld, angle * 4, vec3(0.0f, 1.0f, 0.0f)); //positive rotate in the y-axis
					modelToWorld = rotate(modelToWorld, angle, vec3(0.0f, 0.0f, 1.0f));  //positive rotate in the z z-axis
				}

				else if (axiom[l] == '-')
				{
					modelToWorld = rotate(modelToWorld, -angle * 4, vec3(0.0f, 1.0f, 0.0f)); //positive rotate in the y-axis
					modelToWorld = rotate(modelToWorld, -angle, vec3(0.0f, 0.0f, 1.0f));  //positive rotate in the z z-axis
				}

				//push the current state of the matrix
				if (axiom[l] == '[')
				{
					push_matrix(modelToWorld);
				}
				//draw leaves and pop the stack back
				else if (axiom[l] == ']')
				{
					drawBase(&base); //first draw the pistil (sphere at end of branch)
					modelToWorld = rotate(modelToWorld, 45.0f, vec3(1.0f, 0.0f, 0.0f));
					modelToWorld = translate(modelToWorld, vec3(0, (branchSize + (branchSize / 2)), 0));
					drawBase(&base);
					for (int q = 0; q < 4; q++)
					{
						for (int p = 0; p < 3; p++)
						{
							modelToWorld = rotate(modelToWorld, 72.0f, vec3(1.0f, 0.0f, 0.0f));
							modelToWorld = translate(modelToWorld, vec3(2.0f, 0.0f, 0.0f));
							drawBase(&base);
						}
						modelToWorld = translate(modelToWorld, vec3(-8.0f, 0.0f, 0.0f));
						modelToWorld = rotate(modelToWorld, 90.0f, vec3(0.0f, 1.0f, 0.0f));
					}
					modelToWorld = pop_matrix();

				}
			}
		}
	}







};