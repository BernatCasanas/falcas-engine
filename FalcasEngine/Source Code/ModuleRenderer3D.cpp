#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/SDL/include/SDL.h"
#include "ModuleWindow.h"
#include "ModuleCentralEditor.h"
#include "ModuleSceneIntro.h"
#include "ComponentCamera.h"



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Source Code/External Libraries/Glew/libx86/glew32.lib")


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;
	

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);


		
	}

	//Creating frame buffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0
	);

	glGenRenderbuffers(1, &rboDepthStencil);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthStencil);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboDepthStencil);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		LOG("Famebuffer is not complete");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);



	GLenum err = glewInit();


	LOG("Using Glew %s", glewGetString(GLEW_VERSION));

	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	camera = App->camera->camera;
	camera->camera_active = true;

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);


	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	if (camera->update_projection_matrix || changed_camera) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(camera->GetProjectionMatrix());
		camera->update_projection_matrix = false;
		changed_camera = false;

	}
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->GetPosition().x, App->camera->GetPosition().y, App->camera->GetPosition().z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	glLineWidth(1.0f);
	
	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	for (int i = 0; i < aabbs.size(); i++) {
		DrawAABB(aabbs[i]);
	}
	aabbs.clear();

	DrawFrustum(App->scene_intro->camera->frustum);
	DrawFrustum(App->camera->camera->frustum);

	glBegin(GL_LINES);
	glVertex3f(line_origin.x, line_origin.y, line_origin.z);
	glVertex3f(line_end.x, line_end.y, line_end.z);
	glEnd();
	
	return update_status::UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	App->central_editor->CreateDock();
	App->scene_intro->Draw(texColorBuffer);

	

	App->central_editor->Draw();


	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	glDeleteFramebuffers(1, &frameBuffer);

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::DrawAABB(AABB aabb)
{
	glBegin(GL_LINES);

	glVertex3f(aabb.CornerPoint(0).x, aabb.CornerPoint(0).y, aabb.CornerPoint(0).z);
	glVertex3f(aabb.CornerPoint(2).x, aabb.CornerPoint(2).y, aabb.CornerPoint(2).z);
	glVertex3f(aabb.CornerPoint(2).x, aabb.CornerPoint(2).y, aabb.CornerPoint(2).z);
	glVertex3f(aabb.CornerPoint(3).x, aabb.CornerPoint(3).y, aabb.CornerPoint(3).z);
	glVertex3f(aabb.CornerPoint(3).x, aabb.CornerPoint(3).y, aabb.CornerPoint(3).z);
	glVertex3f(aabb.CornerPoint(1).x, aabb.CornerPoint(1).y, aabb.CornerPoint(1).z);
	glVertex3f(aabb.CornerPoint(1).x, aabb.CornerPoint(1).y, aabb.CornerPoint(1).z);
	glVertex3f(aabb.CornerPoint(0).x, aabb.CornerPoint(0).y, aabb.CornerPoint(0).z);
	glVertex3f(aabb.CornerPoint(4).x, aabb.CornerPoint(4).y, aabb.CornerPoint(4).z);
	glVertex3f(aabb.CornerPoint(5).x, aabb.CornerPoint(5).y, aabb.CornerPoint(5).z);
	glVertex3f(aabb.CornerPoint(5).x, aabb.CornerPoint(5).y, aabb.CornerPoint(5).z);
	glVertex3f(aabb.CornerPoint(7).x, aabb.CornerPoint(7).y, aabb.CornerPoint(7).z);
	glVertex3f(aabb.CornerPoint(7).x, aabb.CornerPoint(7).y, aabb.CornerPoint(7).z);
	glVertex3f(aabb.CornerPoint(6).x, aabb.CornerPoint(6).y, aabb.CornerPoint(6).z);
	glVertex3f(aabb.CornerPoint(6).x, aabb.CornerPoint(6).y, aabb.CornerPoint(6).z);
	glVertex3f(aabb.CornerPoint(4).x, aabb.CornerPoint(4).y, aabb.CornerPoint(4).z);
	glVertex3f(aabb.CornerPoint(0).x, aabb.CornerPoint(0).y, aabb.CornerPoint(0).z);
	glVertex3f(aabb.CornerPoint(4).x, aabb.CornerPoint(4).y, aabb.CornerPoint(4).z);
	glVertex3f(aabb.CornerPoint(6).x, aabb.CornerPoint(6).y, aabb.CornerPoint(6).z);
	glVertex3f(aabb.CornerPoint(2).x, aabb.CornerPoint(2).y, aabb.CornerPoint(2).z);
	glVertex3f(aabb.CornerPoint(3).x, aabb.CornerPoint(3).y, aabb.CornerPoint(3).z);
	glVertex3f(aabb.CornerPoint(7).x, aabb.CornerPoint(7).y, aabb.CornerPoint(7).z);
	glVertex3f(aabb.CornerPoint(5).x, aabb.CornerPoint(5).y, aabb.CornerPoint(5).z);
	glVertex3f(aabb.CornerPoint(1).x, aabb.CornerPoint(1).y, aabb.CornerPoint(1).z);
	glEnd();
}

void ModuleRenderer3D::DrawFrustum(Frustum frustum)
{
	vec* f_vec = new vec[8];
	frustum.GetCornerPoints(f_vec);
	glBegin(GL_LINES);
	glVertex3f(f_vec[0].x, f_vec[0].y, f_vec[0].z);
	glVertex3f(f_vec[1].x, f_vec[1].y, f_vec[1].z);
	glVertex3f(f_vec[1].x, f_vec[1].y, f_vec[1].z);
	glVertex3f(f_vec[3].x, f_vec[3].y, f_vec[3].z);
	glVertex3f(f_vec[3].x, f_vec[3].y, f_vec[3].z);
	glVertex3f(f_vec[2].x, f_vec[2].y, f_vec[2].z);
	glVertex3f(f_vec[2].x, f_vec[2].y, f_vec[2].z);
	glVertex3f(f_vec[0].x, f_vec[0].y, f_vec[0].z);
	glVertex3f(f_vec[4].x, f_vec[4].y, f_vec[4].z);
	glVertex3f(f_vec[5].x, f_vec[5].y, f_vec[5].z);
	glVertex3f(f_vec[5].x, f_vec[5].y, f_vec[5].z);
	glVertex3f(f_vec[7].x, f_vec[7].y, f_vec[7].z);
	glVertex3f(f_vec[7].x, f_vec[7].y, f_vec[7].z);
	glVertex3f(f_vec[6].x, f_vec[6].y, f_vec[6].z);
	glVertex3f(f_vec[6].x, f_vec[6].y, f_vec[6].z);
	glVertex3f(f_vec[4].x, f_vec[4].y, f_vec[4].z);
	glVertex3f(f_vec[0].x, f_vec[0].y, f_vec[0].z);
	glVertex3f(f_vec[4].x, f_vec[4].y, f_vec[4].z);
	glVertex3f(f_vec[6].x, f_vec[6].y, f_vec[6].z);
	glVertex3f(f_vec[2].x, f_vec[2].y, f_vec[2].z);
	glVertex3f(f_vec[3].x, f_vec[3].y, f_vec[3].z);
	glVertex3f(f_vec[7].x, f_vec[7].y, f_vec[7].z);
	glVertex3f(f_vec[5].x, f_vec[5].y, f_vec[5].z);
	glVertex3f(f_vec[1].x, f_vec[1].y, f_vec[1].z);
	glEnd();
	delete[] f_vec;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf(camera->GetProjectionMatrix());
	//ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::ChangeCameraActive(ComponentCamera* camera_to_change)
{
	changed_camera = true;
	camera->camera_active = false;
	if (camera_to_change!=nullptr)
		camera = camera_to_change;
	else {
		camera = App->camera->camera;
	}
	camera->camera_active = true;
}

void ModuleRenderer3D::ChangeCullingCamera(ComponentCamera* camera_culling_to_change)
{
	camera->frustum_culling = false;
	if (camera_culling_to_change !=nullptr)
		camera_culling = camera_culling_to_change;
	else {
		camera_culling = App->camera->camera;
	}
	camera_culling->frustum_culling = true;
}
