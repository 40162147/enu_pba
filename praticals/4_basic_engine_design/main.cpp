#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics_framework.h>
#include <phys_utils.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

bool load_content() {
	phys::Init();
	return true;
}

bool update(float delta_time) {
	static float rot = 0.0f;
	rot += 0.2f * delta_time;
	phys::SetCameraPos(rotate(vec3(15.0f, 12.0f, 15.0f), rot, vec3(0, 1.0f, 0)));
	phys::Update(delta_time);
	return true;
}

bool render() {

	phys::DrawScene();
	return true;
}

void main() {
	// Create application
	app application;
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}