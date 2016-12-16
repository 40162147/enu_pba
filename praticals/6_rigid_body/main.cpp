#include "game.h"
#include "physics.h"
#include "LSystem.cpp"
#include "cPhysicsComponents.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics_framework.h>
#include <phys_utils.h>
#include <thread>


using namespace std;
using namespace graphics_framework;
using namespace glm;
#define physics_tick 1.0 / 60.0
double cursor_x = 0.0;
double cursor_y = 0.0;
double prev_x = 0.0;
double prev_y = 0.0;

static vector<unique_ptr<Entity>> SceneList;
static unique_ptr<Entity> floorEnt;
LSystem tree;



unique_ptr<Entity> CreateParticle(const vec3 &position) {
  unique_ptr<Entity> ent(new Entity());
  ent->SetPosition(position);
  unique_ptr<Component> physComponent(new cParticle());
  unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::SPHERE));
  renderComponent->SetColour(phys::RandomColour());
  ent->AddComponent(physComponent);
  ent->AddComponent(unique_ptr<Component>(new cSphereCollider()));
  ent->AddComponent(unique_ptr<Component>(move(renderComponent)));
  return ent;
}

unique_ptr<Entity> CreateBox(const vec3 &position) {
  unique_ptr<Entity> ent(new Entity());
  ent->SetPosition(position);
  ent->SetRotation(angleAxis(0.0f, vec3(1, 0, 0)));
  unique_ptr<Component> physComponent(new cRigidCube());
  unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::BOX));
  renderComponent->SetColour(phys::RandomColour());
  ent->AddComponent(physComponent);
  ent->SetName("Cuby");
  ent->AddComponent(unique_ptr<Component>(new cBoxCollider()));
  ent->AddComponent(unique_ptr<Component>(move(renderComponent)));

  return ent;
}

unique_ptr<Entity> CreateSphere(const vec3 &position) {
	unique_ptr<Entity> ent(new Entity());
	ent->SetPosition(position);
	ent->SetRotation(angleAxis(0.0f, vec3(1, 0, 0)));
	unique_ptr<Component> physComponent(new cRigidSphere());
	unique_ptr<cShapeRenderer> renderComponent(new cShapeRenderer(cShapeRenderer::SPHERE));
	renderComponent->SetColour(phys::RandomColour());
	ent->AddComponent(physComponent);
	ent->SetName("circ");
	ent->AddComponent(unique_ptr<Component>(new cSphereCollider()));
	ent->AddComponent(unique_ptr<Component>(move(renderComponent)));

	return ent;
}


unique_ptr<Entity> CreateCylinder(const vec3 &position, float dia, float height) {
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

bool initialise()
{
	// ********************************
	// Set input mode - hide the cursor
	// ********************************
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// ******************************
	// Capture initial mouse position
	// ******************************
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

	return true;
}


bool update(double delta_time) 
{

  double current_x;
  double current_y;
  static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
  static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) / static_cast<float>(renderer::get_screen_height());
  double delta_x;
  double delta_y;
  static double t = 0.0;
  static double accumulator = 0.0;
  accumulator += delta_time;

  //glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

  // ***************************************************
  // Calculate delta of cursor positions from last frame
  // ***************************************************
  //delta_x = current_x - prev_x;
  //delta_y = current_y - prev_y;

  // *************************************************************
  // Multiply deltas by ratios - gets actual change in orientation
  // *************************************************************
  //delta_x = delta_x * ratio_width;
  //delta_y = delta_y * ratio_height;

  // *************************
  // Rotate cameras by delta
  // delta_y - x-axis rotation
  // delta_x - y-axis rotation
  // *************************

  //phys::Rotate(delta_x, -delta_y);

  if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) 
  {  
	  Llist[Llist.size() - 1]->getComponent<cRigidSphere>()->AddLinearImpulse(dvec3(-0.5, 0, 0));
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
  {
	  Llist[Llist.size() - 1]->getComponent<cRigidSphere>()->AddLinearImpulse(dvec3(0, 0, 0.5));
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
  {
	  Llist[Llist.size() - 1]->getComponent<cRigidSphere>()->AddLinearImpulse(dvec3(0.5, 0, 0));
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
  {
	  Llist[Llist.size() - 1]->getComponent<cRigidSphere>()->AddLinearImpulse(dvec3(0, 0, -0.5));
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_Q))
  {
	  Llist[Llist.size() - 1]->getComponent<cRigidSphere>()->AddLinearImpulse(dvec3(0, 0.5, 0));
  }
  if (glfwGetKey(renderer::get_window(), GLFW_KEY_E))
  {
	  Llist[Llist.size() - 1]->getComponent<cRigidSphere>()->AddLinearImpulse(dvec3(0, -0.5, 0));
  }



  while (accumulator > physics_tick) {
    UpdatePhysics(t, physics_tick);
    accumulator -= physics_tick;
    t += physics_tick;
  }
  
  for (auto &e : Llist) {
    e->Update(delta_time);
  }
  
  phys::Update(delta_time);
  //prev_x = current_x;
  // prev_y = current_y;
  return true;
}

bool load_content() {
  phys::Init();



 tree.simulate();
 

 
 Llist.push_back(move(CreateSphere(phys::GetCameraPos())));
 

  floorEnt = unique_ptr<Entity>(new Entity());
  floorEnt->AddComponent(unique_ptr<Component>(new cPlaneCollider()));
  floorEnt->SetName("Floor");
  
  phys::SetCameraPos(vec3(80.0f, 10.0f, 20.0f));
  phys::SetCameraTarget(Llist[0]->GetPosition());
  //auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
 // phys::SetCameraProjection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
  InitPhysics();
  return true;
}

bool render() {
  for (auto &e : Llist) 
  {
    e->Render();
  }

  phys::DrawScene();
  return true;
}

void main() {
  // Create application
  app application;
  // Set load content, update and render methods
  application.set_load_content(load_content);
 // application.set_initialise(initialise);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}