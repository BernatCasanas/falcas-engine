#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "External Libraries/SDL/include/SDL_scancode.h"
#include "External Libraries/SDL/include/SDL_mouse.h"
#include "External Libraries/MathGeoLib/include/Math/MathFunc.h"
#include "External Libraries/MathGeoLib/include/Geometry/LineSegment.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ModuleWindow.h"



//////TEMPORAL
#include "ModuleRenderer3D.h"
ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	camera = (ComponentCamera*)(new GameObject(-2, "Editor Camera", nullptr))->CreateComponent(Component_Type::Camera);
	SetPosition({ 1,1,5 });
	LookAt({ 0,0,0 });

}

ModuleCamera3D::~ModuleCamera3D()
{
	delete camera;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (!camera->camera_active)
		return UPDATE_CONTINUE;
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT)==KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE) {
		float x = App->input->GetMouseX();
		float y = App->input->GetMouseY();
		float scene_x, scene_y, scene_width, scene_height;
		App->scene_intro->GetSceneDimensions(scene_x, scene_y, scene_width, scene_height);
		if (x >= scene_x && x <= scene_width + scene_x && y >= scene_y && y <= scene_height + scene_y) {
			float x_final = ((float)x - scene_x) / (scene_width*0.5);
			float y_final = ((float)y - scene_y) / (scene_height*0.5);
			x_final--;
			y_final=-y_final + 1;
			LineSegment picking_ray = camera->frustum.UnProjectLineSegment(x_final,y_final);
			App->scene_intro->SelectGameObjectWithRay(picking_ray);
		}
	}

	camera->owner->Update();

	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	float3 newPos = { 0,0,0 };
	float speed = 3.0f * dt;
	
	
	// Mouse motion ----------------
	int wheel = App->input->GetMouseZ();
	if (wheel != 0) {
		Zooming(wheel);
	}
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT) {
		int double_speed = 1;
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();
		newPos -= (GetRotation().ToFloat3x3() *float3::unitY) * speed * dy;
		newPos -= (GetRotation().ToFloat3x3() *float3::unitX) * speed * dx;

		SetPosition(GetPosition() + newPos);
		Reference += newPos;

	}
	
	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT)
	{
		int double_speed = 1;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
			double_speed = 2;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += (GetRotation().ToFloat3x3() * float3::unitY) * speed * double_speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= (GetRotation().ToFloat3x3() * float3::unitY) * speed * double_speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos += (GetRotation().ToFloat3x3() * float3::unitX) * speed * double_speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos -= (GetRotation().ToFloat3x3() * float3::unitX) * speed * double_speed;

		SetPosition(GetPosition() + newPos);
		Reference += newPos;
	
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;
		

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			Reference += (GetRotation().ToFloat3x3() * float3::unitX) * DeltaX;
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			float3 increment_y = (GetRotation().ToFloat3x3() * float3::unitY) * DeltaY;
			if (increment_y.y < 0.01 && increment_y.y > -0.01) {
				///NEEDED TO BE CHECKED
				Reference += increment_y;
			}
			else {
				Reference += increment_y;
			}
		}

		
	}
	
	else if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT) {
		int speed = 1;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (dx + dy > 0) {
			speed = -1;
		}
		else if (dx + dy == 0 && dx > 0) {
			speed = -1;
		}

		Zooming(((abs(dx) + abs(dy)) / 2) * speed);
	}
	
	else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();
		float Sensitivity = 0.25f;
		float3 pos = GetPosition();
		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			pos -= (GetRotation().ToFloat3x3() * float3::unitX) * DeltaX;
		}
		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;
			float3 increment_y= (GetRotation().ToFloat3x3() * float3::unitY) * DeltaY;
			if (increment_y.y < 0.01&& increment_y.y > -0.01) {
				///NEEDED TO BE CHECKED
				pos -= increment_y;
			}
			else {
				pos -= increment_y;
			}
		}
		SetPosition(pos);
		
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (App->scene_intro->game_object_selected == nullptr) {
			LOG("You must select and object in order to center to it!");
		}
		else{
			ComponentTransform* transform = (ComponentTransform*)App->scene_intro->game_object_selected->components.front();
			float3 position = transform->GetPosition();
			float3 size = transform->GetSize();
			Reference = { position.x,position.y,position.z };
			SetPosition(Reference + float3{20, 20, 20});//size.MaxElement();
			LookAt(Reference);
		}
	}
	LookAt(Reference);

	return UPDATE_CONTINUE;
}


// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3&Spot)
{
	Reference = Spot;
	float3 vector = Reference - GetPosition();

	float3x3 matrix = float3x3::LookAt(float3::unitZ, vector.Normalized(), float3::unitY, float3::unitY);
	GetComponentTransform()->SetRotation(matrix.ToQuat());
}


// -----------------------------------------------------------------


void ModuleCamera3D::Zooming(float zooming_value)
{
	float3 newPos = GetPosition();
	float distance = Length(newPos - Reference);
	distance *= 0.1;

	newPos += (GetRotation().ToFloat3x3() * float3::unitZ) * zooming_value * distance;
	if (distance < 0.0001);
		Reference += (GetRotation().ToFloat3x3() * float3::unitZ) * zooming_value * distance;
	SetPosition(newPos);
}


ComponentTransform* ModuleCamera3D::GetComponentTransform()
{
	return (ComponentTransform*)camera->owner->GetComponent(Component_Type::Transform);;
}

void ModuleCamera3D::SetPosition(float3 pos)
{
	ComponentTransform* trans = GetComponentTransform();
	trans->SetPosition(pos);
}

float3 ModuleCamera3D::GetPosition()
{
	ComponentTransform* trans = GetComponentTransform();
	return trans->GetPosition();
}

Quat ModuleCamera3D::GetRotation()
{
	ComponentTransform* trans = GetComponentTransform();
	return trans->GetRotation();
}
