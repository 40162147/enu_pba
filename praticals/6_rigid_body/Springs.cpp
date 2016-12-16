#include <vector>
#include <glm/glm.hpp>



using namespace glm;
using namespace std;


struct SpringNode
{
	dvec3 position;
	dvec3 velocity;
	dvec3 force;
	double mass;

	std::vector<SpringNode> neighbours;
	std::vector<double> neighbourDistances;
};

struct AnchorNode
{
	SpringNode *node;
	dvec3 position;
};

class Springs
{
public:

	void ComputeSingleForce(SpringNode const *a, SpringNode const *b);
	void AddAnchorNode(int node, dvec3 const &anchorPosition);
	void Animate(int delta_time);
	virtual void ConnectSprings() = 0;

protected:

	double _springK;
	double _inertialDistance;
	dvec3 _force;
	dvec3 _vel;
	std::vector<SpringNode> _springNodes;
	std::vector<AnchorNode> _anchorNodes;
	std::vector<SpringNode*> _movingNodes;
	double _mass;
	double _energyLoss;

};

float springTolerance;

void Springs::ComputeSingleForce(SpringNode const *a, SpringNode const *b)
{
	_force = b->position - a->position;
	_vel = b->velocity - a->velocity;

	if (glm::length(_force) - _inertialDistance < springTolerance)
	{
		_force = dvec3(0, 0 ,0);
		return;
	}

	double intensity = -_springK * (glm::length(_force) - _inertialDistance);
	glm::normalize(_force);
	_vel.x = _vel.x * 400000;
	_vel.y = _vel.y * 400000;
	_vel.z = _vel.z * 400000;
	_force += ((_force / glm::length(_force)) * intensity) - (_vel);

}

void Springs::AddAnchorNode(int node, dvec3 const &anchorPosition)
{
	_anchorNodes.resize(_anchorNodes.size() + 1);
	_anchorNodes[_anchorNodes.size() - 1].node = &_springNodes[node];
	_anchorNodes[_anchorNodes.size() - 1].position = anchorPosition;
	_movingNodes.erase(_movingNodes.begin() + node);

}

void Springs::Animate(int delta_time)
{
	double time = delta_time;
	// Gravitational force
	// To compute the gravitational force we use F = ma
	// (m = mass, a = acceleration due to gravity (in units per second which is g=9.81m/s^2)
	double fg = -9.81 * _mass;

	// Compute all external forces on the springs
	for (int i = 0; i<_movingNodes.size(); ++i)
	{
		// Zero out the force vector
		_movingNodes[i]->force = dvec3(0,0,0);

		// Factor in the gravitational force. Since it moves only in the y direction, we
		// only modify the y component of the vector.
		_movingNodes[i]->force[1] = fg;
	}

	// Compute internal forces on the springs
	for (int i = 0; i<_movingNodes.size(); ++i)
	{
		// Compute the force the neighbor nodes are exerting on each node
		for (int j = 0; j<_movingNodes[i]->neighbours.size(); ++j)
		{
			//
			ComputeSingleForce(&_movingNodes[i]->neighbours[j], _movingNodes[i]);
			_movingNodes[i]->force += _force; // Add the force exerted to this nodes force vector
		}
	}

	// Increment velocity and position of nodes
	for (int i = 0; i<_movingNodes.size(); ++i)
	{
		_movingNodes[i]->velocity += (_movingNodes[i]->force / _mass)*time; // Add
		_movingNodes[i]->position += _movingNodes[i]->velocity; // Add the velocity to pos
		_movingNodes[i]->velocity *= _energyLoss;
	}
}


class StringSpringSystem : public Springs
{
public:
	void ConnectSprings();

};

void StringSpringSystem::ConnectSprings()
{
	_springNodes.resize(80);
	_movingNodes.resize(80);


	_springK = 8000; // Rigidity of springs
	_inertialDistance = .05; // Springs are 5 cm apart
	_mass = .05; // Each mass is .05 kg
	_energyLoss = 1 - .00001; // Amount of energy the system will lose as time elapses

	// Set springNode distances along the positive x-axis
	for (int i = 0; i<_springNodes.size(); ++i)
	{
		_springNodes[i].position = dvec3(0.0f + i*_inertialDistance, 1.0f, -5.0f);
		_movingNodes[i] = &_springNodes[i];
		_movingNodes[i]->velocity.x = 0;
		_movingNodes[i]->velocity.y = 0;
		_movingNodes[i]->velocity.z = 0;
	}

	// Set up the first spring node
	_springNodes[0].neighbours.resize(1);
	_springNodes[0].neighbourDistances.resize(1);
	_springNodes[0].neighbours[0] = _springNodes[1];
	_springNodes[0].neighbourDistances[0] = _inertialDistance;

	// Set up the last spring node
	_springNodes[_springNodes.size() - 1].neighbours.resize(1);
	_springNodes[_springNodes.size() - 1].neighbourDistances.resize(1);
	_springNodes[_springNodes.size() - 1].neighbours[0] = _springNodes[_springNodes.size() - 2];
	_springNodes[_springNodes.size() - 1].neighbourDistances[0] = _inertialDistance;

	// Set up all nodes in between
	for (int i = 1; i<_springNodes.size() - 1; ++i)
	{
		_springNodes[i].neighbours.resize(2);
		_springNodes[i].neighbourDistances.resize(2);

		//
		_springNodes[i].neighbours[0] = _springNodes[i - 1];
		_springNodes[i].neighbourDistances[0] = _inertialDistance;

		//
		_springNodes[i].neighbours[1] = _springNodes[i + 1];
		_springNodes[i].neighbourDistances[1] = _inertialDistance;
	}

	// Make the middle node an anchor node (i.e. it does not move)
	AddAnchorNode((_springNodes.size() - 1) / 2, dvec3(0.0f, _springNodes[(_springNodes.size() - 1) / 2].position[1], -5.0f));
}