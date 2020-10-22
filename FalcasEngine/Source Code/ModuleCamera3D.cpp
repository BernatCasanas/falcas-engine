#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "External Libraries/SDL/include/SDL.h"
#include "External Libraries/MathGeoLib/include/Math/MathFunc.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "Shape.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{}

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
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!

	vec3 newPos(0,0,0);
	float speed = 3.0f * dt;
	
	// Mouse motion ----------------
	int wheel = App->input->GetMouseZ();
	if (wheel != 0) {
		vec3 looking = Reference - Position;
		vec3 looking_normalized = looking / sqrt(pow(looking.x, 2) + pow(looking.y, 2) + pow(looking.z, 2));
		looking_normalized *= wheel;
		looking = looking_normalized;
		looking = Position;
		Position += looking_normalized;
		if (wheel > 0 && is_inside(Position, looking, Reference))
			Reference += looking_normalized;

		
		LookAt(Reference);
	}

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int double_speed = 1;
		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
			double_speed = 2;
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed * double_speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed * double_speed;


		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed * double_speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed * double_speed;

		Position += newPos;
		Reference += newPos;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			//Position

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}


	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {


		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			//Position

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
	}
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_IDLE) {
		Reference={0,0,0};
		LookAt(Reference);
	}
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (App->scene_intro->game_object_selected == nullptr) {
			LOG("You must select and object in order to center to it!");
		}
		else{
			float x, y, z;
			App->scene_intro->game_object_selected->GetPosition(x, y, z);
			Reference = { x,y,z };
			Position = Reference+5;
			LookAt(Reference);
		}
	}
	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	if (Z.x != 0 || Z.y != 1 || Z.z != 0)
		X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
