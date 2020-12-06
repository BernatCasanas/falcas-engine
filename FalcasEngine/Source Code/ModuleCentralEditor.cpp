
#include "Application.h"
#include "ModuleCentralEditor.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/MathGeoLib/include/MathGeoLib.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/ImGui/imgui_impl_opengl3.h"
#include "External Libraries/SDL/include/SDL.h"
#include "External Libraries/SDL/include/SDL_opengl.h"
#include <string>
#include <Windows.h>
#include <stdio.h>
#include "ModuleWindow.h"
#include "Console.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Shape.h"
#include "External Libraries/ImGui/imconfig.h"
#include "External Libraries/ImGui/imgui_impl_sdl.h"
#include "FileSystem.h"
#include <algorithm>
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "Importer.h"

ModuleCentralEditor::ModuleCentralEditor(Application* app, bool start_enabled) : Module(app, start_enabled, "moduleCentralEditor"),progress(50.f),progress2(50.f),progress3(50.f), progress4(50.f)
{
}

// Destructor
ModuleCentralEditor::~ModuleCentralEditor()
{

}

// Called before render is available
bool ModuleCentralEditor::Init()
{
	LOG("Init ImGui stuff related");
	bool ret = true;

  
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }


    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
    ImGui_ImplOpenGL3_Init();



	return ret;
}

// Called before quitting
bool ModuleCentralEditor::CleanUp()
{
	LOG("Deleting all ImGui stuff related");
    
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

update_status ModuleCentralEditor::PreUpdate(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();
    int i = 0;
    
    if (fr_arr.size() >= 50) {
        fr_arr.pop_front();
        fr_arr.push_back(ImGui::GetIO().Framerate);
    }
    else {
        fr_arr.push_back(ImGui::GetIO().Framerate);
    }
    
    if (ms_arr.size() >= 50) {
        ms_arr.pop_front();
        ms_arr.push_back(ImGui::GetIO().Framerate/3.6);
    }
    else {
        ms_arr.push_back(ImGui::GetIO().Framerate/3.6);
    }

    return UPDATE_CONTINUE;
}

update_status ModuleCentralEditor::PostUpdate(float dt)
{
    //SHORTCUTS
    if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
        show_console = !show_console;
    if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
        show_hierarchy = !show_hierarchy;
    if (App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
        show_inspector = !show_inspector;
    if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
        show_configuration = !show_configuration;
    if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
        show_assets_window = !show_assets_window;

    if (wantToExit) update_status::UPDATE_STOP;
   
    

    return UPDATE_CONTINUE;
}

void ModuleCentralEditor::Draw()
{

    { // UPSIDE BAR
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Quit", "Esc")) {
                wantToExit=true;
            }
			if (ImGui::MenuItem("Save Scene")) {
				SaveScene();
			}
            if (ImGui::MenuItem("Load Scene")) {
                loading_file = !loading_file;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Console", "Alt + 1")) {
                show_console = !show_console;
            }
            if (ImGui::MenuItem("Hierarchy", "Alt + 2")) {
                show_hierarchy = !show_hierarchy;
            }
            if (ImGui::MenuItem("Inspector", "Alt + 3")) {
                show_inspector = !show_inspector;
            }
            if (ImGui::MenuItem("Configuration", "Alt + 4")) {
                show_configuration = !show_configuration;
            }
            if (ImGui::MenuItem("Inspector", "Alt + 5")) {
                show_assets_window = !show_assets_window;
            }
            if (ImGui::MenuItem("OpenGL Options")) {
                show_openglOptions = !show_openglOptions;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject")) {
            if (ImGui::MenuItem("Create Empty")) {
                App->scene_intro->CreateGameObject("GameObject", App->scene_intro->root);
            }
            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Cube")) {
                    CreateShape(Shape::Cube, "Cube");
                }
                if (ImGui::MenuItem("Rectangular Prism")) {
                    CreateShape(Shape::Rectangular_Prism, "Rectangular Prism");
                }
                if (ImGui::MenuItem("Triangular Pyramid")) {
                    CreateShape(Shape::Triangular_Pyramid, "Triangular Pyramid");
                }
                if (ImGui::MenuItem("Square Pyramid")) {
                    CreateShape(Shape::Square_Pyramid, "Square Pyramid");
                }
                if (ImGui::MenuItem("Rectangular Pyramid")) {
                    CreateShape(Shape::Rectangular_Pyramid, "Rectangular Pyramid");
                }
                if (ImGui::MenuItem("Cilinder")) {
                    CreateShape(Shape::Cilinder, "Cilinder");
                }
                if (ImGui::MenuItem("Cone")) {
                    CreateShape(Shape::Cone, "Cone");
                }
                if (ImGui::MenuItem("Sphere")) {
                    CreateShape(Shape::Sphere, "Sphere");
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("2D Object")) {
                if (ImGui::MenuItem("Plane")) {
                    CreateShape(Shape::Plane, "Plane");
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Create Camera")) {
                GameObject* game_object= App->scene_intro->CreateGameObject("Camera", App->scene_intro->root);
                game_object->CreateComponent(Component_Type::Camera);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("Gui Demo")) {
                show_example = !show_example;
            }
            if (ImGui::MenuItem("Documentation")) {
                ShellExecute(NULL, NULL, "https://github.com/Falcas-Games/Falcas-Engine", NULL, NULL, SW_SHOWNORMAL);
            }
            if (ImGui::MenuItem("Download Latest")) {
                ShellExecute(NULL, NULL, "https://github.com/Falcas-Games/Falcas-Engine/releases", NULL, NULL, SW_SHOWNORMAL);
            }
            if (ImGui::MenuItem("Report a bug")) {
                ShellExecute(NULL, NULL, "https://github.com/Falcas-Games/Falcas-Engine/issues", NULL, NULL, SW_SHOWNORMAL);
            }
            if (ImGui::MenuItem("About")) {
                show_about = !show_about;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    //Demo Gui
    if (show_example) {
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!", NULL);                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }


        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    }
    //About
    if (show_about) {
        ImGui::Begin("About Falcas Engine", &show_about);
        ImGui::TextWrapped("Falcas Engine v0.1\n\n"
            "This Game Engine is made in the game engines subject in CITM\n\n"
            "Is made by:");
        ImGui::Text("\n");
        if (ImGui::Button("Bernat Casanas")) {
            ShellExecute(NULL, NULL, "https://github.com/BernatCasanas", NULL, NULL, SW_SHOWNORMAL);
        }
        ImGui::Text("\n");
        if (ImGui::Button("Arnau Falgueras")) {
            ShellExecute(NULL, NULL, "https://github.com/Arnau77", NULL, NULL, SW_SHOWNORMAL);
        }
        ImGui::Text("\n");
        ImGui::Text("3rd Party Libraries used:\n");
        ImGui::BulletText("SDL 2.0");
        ImGui::BulletText("Glew 7.0");
        ImGui::BulletText("ImGui 1.51");
        ImGui::BulletText("MathGeoLib 1.5");
        ImGui::BulletText("OpenGL 3.1");
        ImGui::BulletText("Devil 1.8");
        ImGui::BulletText("Assimp 5.0.1");
        ImGui::BulletText("PhysFS 3.1");
        ImGui::Separator();
        ImGui::TextWrapped("\n\n"
            "MIT License\n\n"

            "Copyright(c) 2020 Falcas - Games\n\n"

            "Permission is hereby granted, free of charge, to any person obtaining a copy"
            "of this softwareand associated documentation files(the Software), to deal"
            "in the Software without restriction, including without limitation the rights"
            "to use, copy, modify, merge, publish, distribute, sublicense, and /or sell"
            "copies of the Software, and to permit persons to whom the Software is"
            "furnished to do so, subject to the following conditions :\n\n"

            "The above copyright noticeand this permission notice shall be included in all"
            "copies or substantial portions of the Software.\n\n"

            "THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR"
            "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,"
            "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE"
            "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER"
            "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
            "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE"
            "SOFTWARE.");

        ImGui::End();
    }
    //Configuration
    if (show_configuration) {
        ImGui::Begin("Configuration", &show_configuration);
        ImGui::Text("Options");
        if (ImGui::CollapsingHeader("Window")) {
            //BRIGHT
            ImGui::SliderFloat("Bright", &progress, 0.0f, 100.0f);
            SDL_SetWindowBrightness(App->window->window, (progress * 2) / 100);

            //SIZE WINDOW
            ImGui::SliderFloat("Width", &progress2, 0.0f, 100.0f);
            ImGui::SliderFloat("Height", &progress3, 0.0f, 100.0f);
            SDL_SetWindowSize(App->window->window, (int)(progress2 * 1280 / 50) * SCREEN_SIZE, (int)(progress3 * 720 / 50) * SCREEN_SIZE);
        }
        if (ImGui::CollapsingHeader("Application")) {
            float fr[50];
            float ms[50];
            char fr_char[50];
            char ms_char[50];
            std::copy(fr_arr.begin(), fr_arr.end(), fr);
            std::copy(ms_arr.begin(), ms_arr.end(), ms);
            sprintf_s(fr_char, 50, "%f Framerate", fr[49]);
            sprintf_s(ms_char, 50, "%f Milliseconds", ms[49]);
            ImGui::PlotHistogram("", fr, 50, 0, fr_char, 0.0f, 150.0f, ImVec2(0, 80.0f));
            ImGui::PlotHistogram("", ms, 50, 0, ms_char, 0.0f, 30.f, ImVec2(0, 80.0f));
        }
        if (ImGui::CollapsingHeader("Input")) {
            for(int i=0;i<input_list.size();i++)
                ImGui::TextUnformatted(input_list[i]);
            if (need_scroll)
                ImGui::SetScrollHere(1.0f);
            need_scroll = false;
        }
        if (ImGui::CollapsingHeader("Audio")) {
            ImGui::SliderFloat("", &progress4, 0.0f, 100.0f);
        }
        if (ImGui::CollapsingHeader("Hardware")) {
            ImGui::Text("SDL Version: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "2.0");
            ImGui::Text("DevIL Version: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "1.8");
            ImGui::Text("OpenGL Version: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3.1");
            ImGui::Separator();
            ImGui::Text("CPUs: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", SDL_GetCPUCount());
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "(Cache: %i kb)", SDL_GetCPUCacheLineSize());
            ImGui::Text("System RAM: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i Mb", SDL_GetSystemRAM());
            ImGui::Text("Caps: ");
            ImGui::SameLine();
            if (SDL_Has3DNow) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3D Now "); ImGui::SameLine();
            if (SDL_HasAVX) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX "); ImGui::SameLine();
            if (SDL_HasAVX2) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX2 "); ImGui::SameLine();
            if (SDL_HasAltiVec) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AltiVec "); ImGui::SameLine();
            if (SDL_HasMMX) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MMX "); ImGui::SameLine();
            if (SDL_HasRDTSC) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RDTSC "); ImGui::SameLine();
            if (SDL_HasSSE) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE "); ImGui::SameLine();
            if (SDL_HasSSE2) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE2 "); ImGui::SameLine();
            if (SDL_HasSSE3) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE3 "); ImGui::SameLine();
            if (SDL_HasSSE41) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE41 "); ImGui::SameLine();
            if (SDL_HasSSE42) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE42 "); ImGui::SameLine();
            ImGui::Separator();
            const GLubyte* vendor = glGetString(GL_VENDOR);
            const GLubyte* renderer = glGetString(GL_RENDERER);

            ImGui::Text("GPU: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", renderer);
            ImGui::Text("Brand: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", vendor);

        }
        ImGui::End();
    }
    //Console
    if (show_console) {
        ImGui::Begin("Console", &show_console);
        console_logs = App->console->GetLogs();
        for (int i = 0; i < console_logs.size(); i++) {
            ImGui::Text(console_logs[i]);
        }
        ImGui::End();
    }

    //Hierarchy
    if (show_hierarchy) {
        ImGui::Begin("Hierarchy", &show_hierarchy);
        static int node_clicked = -2;
        if (selected_through_screen) {
            if (App->scene_intro->game_object_selected == nullptr)
                node_clicked = -2;
            else
                node_clicked = App->scene_intro->game_object_selected->id;
            selected_through_screen = false;
        }
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        GameObject* game_object = App->scene_intro->root;
        HierarchyRecursiveTree(game_object, base_flags, node_clicked);
        ids_of_parents_selected.clear();
        ImGui::End();
    }

    if (show_inspector) {
        ImGui::Begin("Inspector", &show_inspector);
        if (App->scene_intro->game_object_selected != nullptr) {
            GameObject* game_object = App->scene_intro->game_object_selected;
            ImGui::Checkbox("##0", &game_object->active);
            ImGui::SameLine();
            std::string game_object_name = game_object->name;
            if (ImGui::InputText(" ", &game_object_name, ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (game_object_name != game_object->name) {
                    game_object->name = game_object_name;
                }
            }
            for (int i = 0; i < game_object->components.size(); i++) {
                Component* game_object_component = game_object->components[i];
                if (ImGui::CollapsingHeader(game_object_component->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                    game_object_component->Inspector();
                }
            }
        }
        if (App->scene_intro->game_object_selected != nullptr && !App->scene_intro->game_object_selected->HasComponentType(Component_Type::Camera)) {
            if(ImGui::Button("Create Component Camera")) {
                App->scene_intro->game_object_selected->CreateComponent(Component_Type::Material);
            }
        }
        ImGui::End();
    }

    if (show_assets_window) {
        ImGui::Begin("Assets", &show_assets_window);
        static std::string file_assets_selected = "";
        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        FilesRecursiveTree("Assets", true, true, base_flags, file_assets_selected);
        ImGui::End();
    }

    if (show_openglOptions)
    {
        ImGui::Begin("OpenGL Options", &show_openglOptions);
        ImGui::Checkbox("Depth Test", &depth);
        ImGui::Checkbox("Cull Face", &cullface);
        ImGui::Checkbox("Light", &lighting);
        ImGui::Checkbox("Color Material", &colorMaterial);
        ImGui::Checkbox("Texture", &texture);
        ImGui::Checkbox("Ambient", &ambient);
        ImGui::Checkbox("Stencil", &stencil);
        ImGui::Checkbox("Wireframe", &wireframe);
        ImGui::Checkbox("AABBs", &aabbs);
        ImGui::Checkbox("Vertex Normals", &normals_v);
        ImGui::Checkbox("Face Normals", &normals_f);
        ImGui::Checkbox("Grid", &App->scene_intro->root->components.back()->active);
        ImGui::Checkbox("Camera Frustums", &frustums);
        ImGui::End();
    }

	if (loading_file) {
		LoadFile();
	}


    if (depth) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
    if (cullface) glEnable(GL_CULL_FACE);
    else glDisable(GL_CULL_FACE);
    if (lighting) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);
    if (colorMaterial) glEnable(GL_COLOR_MATERIAL);
    else glDisable(GL_COLOR_MATERIAL);
    if (texture) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);
    if (ambient) glEnable(GL_STENCIL_TEST);
    else glDisable(GL_STENCIL_TEST);
    if (stencil) glEnable(GL_AMBIENT);
    else glDisable(GL_AMBIENT);


    // Rendering
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
        SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
    }
}

bool ModuleCentralEditor::LoadFile()
{
	bool ret = false;
	ImGui::OpenPopup("Load File");
    if (ImGui::BeginPopupModal("Load File")) {
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
        ImGui::BeginChild("File Browser", ImVec2(0, 300), true);
        std::string s = "";
        FilesRecursiveTree("Assets", false, true, 0, s);
        ImGui::EndChild();
        ImGui::PopStyleVar();
        ImGui::PushItemWidth(250.f);
        if (ImGui::InputText("##file_selector", selected_file, 100, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        ImGui::SameLine();
        if (ImGui::Button("Ok", ImVec2(50, 20))) {
            if (App->filesystem->GetTypeFile(selected_file) == FILE_TYPE::SCENE) {
                loading_file = !loading_file;
                LoadScene((const char*)selected_file);
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(50, 20)))
        {
            loading_file = !loading_file;
            selected_file[0] = '\0';
        }
        ImGui::EndPopup();
    }
	return true;
}

bool ModuleCentralEditor::SaveScene()
{
	char* buffer = nullptr;

	JsonObj scene;
	scene.AddString("name", "scene");
	JsonArray arr = scene.AddArray("GameObjects");
	SaveAllGameObjectsTree(App->scene_intro->root, arr);
	scene.Save(&buffer);
	char name[150] = "Library/Scenes/scene.scenefalcas";
    std::string fileName = "scene";
    std::string baseName = "scene";
	for (int i = 0; App->filesystem->FileExists(name); i++) {
        fileName = App->filesystem->GetFileName(name, true);
        if (i != 0) fileName =  baseName + std::to_string(i);
		sprintf_s(name, 150, "Library/Scenes/%s.scenefalcas", fileName.c_str());
	}
	App->filesystem->SaveInternal(name, buffer, strlen(buffer));
    if (App->filesystem->FileExists("Assets/Scenes")) {
		sprintf_s(name, 150, "Assets/Scenes/%s.scenefalcas", fileName.c_str());
	    App->filesystem->SaveInternal(name, buffer, strlen(buffer));
    }
    else {
         if(App->filesystem->FileExists("Assets")){
		    sprintf_s(name, 150, "Assets/%s.scenefalcas", fileName.c_str());
	        App->filesystem->SaveInternal(name, buffer, strlen(buffer));
        }
    }
	return true;
}

void ModuleCentralEditor::LoadScene(const char* file)
{
    DeleteAllGameObjects(App->scene_intro->root);
    char* buffer;
    App->filesystem->LoadPath((char*)file, &buffer);
    JsonObj scene(buffer);
    JsonArray arr_gameObjects(scene.GetArray("GameObjects"));

    for (int i = 0; i < arr_gameObjects.Size(); ++i) {
        JsonObj obj = arr_gameObjects.GetObjectAt(i);

        GameObject* parent = nullptr;
        if (obj.GetInt("Parent UUID") != 0) {
            SearchParent(App->scene_intro->root, obj.GetInt("Parent UUID"));
            parent = parentFound;
            parentFound = nullptr;
        }
        else {
            parent = App->scene_intro->root;
        }
        float3 position = obj.GetFloat3("Translation");
        Quat q_rotation = { obj.GetFloat3("Rotation"),1 };
        float3 p_size = obj.GetFloat3("Scale");

        GameObject* gameObject = App->scene_intro->CreateGameObject(position, q_rotation, p_size, obj.GetString("name"), parent);
        gameObject->parent = parent;
        gameObject->SetUUID(obj.GetInt("UUID"));
        
        std::string grid = obj.GetString("name");
        if (grid == "Grid") {
            App->scene_intro->root->SetUUID(obj.GetInt("UUID"));
            continue;
        }

        JsonArray components = obj.GetArray("Components");
        for(int i = 0; i<components.Size();++i){
            JsonObj comp = components.GetObjectAt(i);
            std::string component_name = comp.GetString("name");
            if (component_name == "Transform") {
                ComponentTransform* trans = (ComponentTransform*)gameObject->GetComponent(Component_Type::Transform);
                trans->SetUUID(comp.GetInt("UUID"));
                float4x4 matrix = comp.GetFloat4x4("GlobalMatrix");
                trans->SetMatricesWithNewParent(matrix);
            }
            else if (component_name == "Mesh") {
                ComponentMesh* mesh = ImportOnlyMesh(gameObject, comp.GetString("Path"), comp.GetString("AssetPath"), comp.GetInt("MeshNumber"));
                mesh->materialIndex = comp.GetInt("MaterialIndex");
            }
            else if (component_name == "Material") {
                //IDEAS
                ComponentMaterial* mat = (ComponentMaterial*)gameObject->CreateComponent(Component_Type::Material, (char*)comp.GetString("Path"));
            }
            else if (component_name == "Camera") {
                ComponentCamera* cam = (ComponentCamera*)gameObject->CreateComponent(Component_Type::Camera);
                cam->SetUUID(comp.GetInt("UUID"));
                cam->SetFarPlaneDistance(comp.GetFloat("FrustumFar"));
                cam->SetNearPlaneDistance(comp.GetFloat("FrustumNear"));
                cam->SetHorizFov(comp.GetFloat("FrustumHfov"));
                cam->SetVerticalFov(comp.GetFloat("FrustumVfov"));
            }
        }

    }
}

bool ModuleCentralEditor::ProcessEvents(SDL_Event event)
{
    bool done = false;
    ImGui_ImplSDL2_ProcessEvent(&event);
    if (event.type == SDL_QUIT)
        done = true;
    if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(App->window->window))
        done = true;
    return done;
}

void ModuleCentralEditor::FilesRecursiveTree(const char* path, bool is_in_dock, bool is_directory, static ImGuiTreeNodeFlags base_flags, std::string& assets_file_clicked)
{
    ImGuiTreeNodeFlags final_flags = base_flags;
    if (!is_directory) {
        final_flags |= ImGuiTreeNodeFlags_Leaf;
        std::string file_name = App->filesystem->GetFileName(path, true);
        if (assets_file_clicked == file_name)
            final_flags |= ImGuiTreeNodeFlags_Selected;
        if (ImGui::TreeNodeEx(file_name.c_str(), final_flags)) {
            if (ImGui::IsItemClicked()) {
                if (!is_in_dock) {
                    sprintf_s(selected_file, 100, "%s", path);

                    if (ImGui::IsMouseDoubleClicked(0)) {
                        sprintf_s(selected_file, 100, "%s", path);
                        loading_file = !loading_file;
                    }
                }
                else {
                    assets_file_clicked = file_name;
                }
            }

            ImGui::TreePop();
        }
        return;
    }
    std::vector<std::string> files;
    std::vector<std::string> dirs;

    std::string dir((path) ? path : "");
    dir += "/";

    App->filesystem->DiscoverFiles(dir.c_str(), files, dirs, "meta");
    if (dirs.size() == 0 && files.size() == 0) {
        final_flags |= ImGuiTreeNodeFlags_Leaf;

    }
    std::string dir_name = App->filesystem->GetFileName(path, false);
    if (assets_file_clicked == dir_name)
        final_flags |= ImGuiTreeNodeFlags_Selected;

    bool open = false;

    if (is_in_dock||path!="Assets") {
        if (ImGui::TreeNodeEx((dir_name).c_str(), final_flags)) {
            open = true;
            if (ImGui::IsItemClicked()) {
                if (!is_in_dock) {
                    sprintf_s(selected_file, 100, "%s", path);

                    if (ImGui::IsMouseDoubleClicked(0)) {
                        sprintf_s(selected_file, 100, "%s", path);
                        loading_file = !loading_file;
                    }
                }
                else {
                    assets_file_clicked = dir_name;
                }
            }
            ImGui::TreePop();
        }
    }
    else {
        open = true;
    }
    if (open == false)
        return;
    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
    for (int i = 0; i < dirs.size(); ++i) {
        FilesRecursiveTree((dir + dirs[i]).c_str(), is_in_dock, true, base_flags, assets_file_clicked);
    }
    for (int i = 0; i < files.size(); ++i) {
        FilesRecursiveTree((dir + files[i]).c_str(), is_in_dock, true, base_flags, assets_file_clicked);
    }
    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

}

void ModuleCentralEditor::HierarchyRecursiveTree(GameObject* game_object, static ImGuiTreeNodeFlags base_flags, int &id_node_clicked)
{
    if (game_object == nullptr) {
        return;
    }
    ImGuiTreeNodeFlags final_flags = base_flags;
    const bool is_selected = game_object->id == id_node_clicked;
    if (is_selected) {
        final_flags |= ImGuiTreeNodeFlags_Selected;
        if (game_object->id >= 0)
            App->scene_intro->game_object_selected = game_object;
        else
            App->scene_intro->game_object_selected = nullptr;
    }
    bool has_children = true;
    if (game_object->children.size() == 0) {
        has_children = false;
        final_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }
    if (game_object->id < 0 && !has_children) {
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
    }
    if ((ids_of_parents_selected.size() > 0 && ids_of_parents_selected.back() == game_object->id && has_children)) {
        ids_of_parents_selected.pop_back();
        ImGui::SetNextItemOpen(true, ImGuiCond_Always);
    }

    bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)game_object->id, final_flags, (game_object->id < 0) ? "Main" : game_object->GetName().c_str());
    if (ImGui::IsItemClicked())
        id_node_clicked = game_object->id;
   
    if (game_object->id >= 0 && ImGui::BeginPopupContextItem())
    {
        if (ImGui::Selectable("Delete Game Object")) {
            game_object->to_delete = true;
        }
        ImGui::EndPopup();
    }

    if (game_object->id >= 0 && ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GAME_OBJECT", &game_object->id, sizeof(int));
        ImGui::Text("%s", game_object->name.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAME_OBJECT"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            int payload_id = *(const int*)payload->Data;
            GameObject* game_obj = App->scene_intro->SearchGameObject(payload_id, App->scene_intro->root);
            if (!game_object->CheckParentRecursively(game_obj)) {
                game_obj->RemoveFromParent();
                game_object->NewChild(game_obj);
            }
        }
        ImGui::EndDragDropTarget();
    }
    
    if ((node_open && has_children)) {
        for (int i = 0; i < game_object->children.size(); i++) {
            HierarchyRecursiveTree(game_object->children[i], base_flags, id_node_clicked);
        }
        ImGui::TreePop();
    }
    if (game_object->id < 0 && !has_children)
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
}

void ModuleCentralEditor::SearchParent(GameObject* game_object, uint uuid)
{
    if (game_object == nullptr) {
        return;
    }
    bool hasChildren = true;
    if (game_object->children.size() == 0) hasChildren = false;
    if (game_object->GetUUID() == uuid) {
        parentFound = game_object;
        bool_parentFound = true;
    }
    if (hasChildren) {
        for (std::vector<GameObject*>::iterator it = game_object->children.begin(); it != game_object->children.end(); ++it) {
            SearchParent((*it), uuid);
            if (bool_parentFound) continue;
        }
    }
}

void ModuleCentralEditor::SaveAllGameObjectsTree(GameObject* game_object, JsonArray arr)
{
	if (game_object == nullptr) {
		return;
	}
	bool hasChildren = true;
	if (game_object->children.size() == 0) hasChildren = false;
	JsonObj obj;
	game_object->SaveGameObject(obj);
	arr.AddObject(obj);
	if (hasChildren) {
		for (std::vector<GameObject*>::iterator it = game_object->children.begin(); it != game_object->children.end(); ++it) {
			SaveAllGameObjectsTree((*it), arr);
		}
	}
}

void ModuleCentralEditor::DeleteAllGameObjects(GameObject* game_object)
{
    bool hasChildren = true;
    if (game_object->children.size() == 0) hasChildren = false;
    game_object->to_delete = true;
    if (hasChildren) {
        for (std::vector<GameObject*>::iterator it = game_object->children.begin(); it != game_object->children.end(); ++it) {
            DeleteAllGameObjects((*it));
        }
    }
}

void ModuleCentralEditor::SelectObject(GameObject* game_obj)
{
    selected_through_screen = true;
    if (game_obj == nullptr)
        return;
    ids_of_parents_selected.push_back(game_obj->id);
    if (game_obj->parent != nullptr)
        SelectObject(game_obj->parent);
}

void ModuleCentralEditor::CreateShape(Shape shape, std::string name)
{
    GameObject* gm = App->scene_intro->CreateGameObject(name, App->scene_intro->root);
    ComponentMesh* mesh = (ComponentMesh*)gm->CreateComponent(Component_Type::Mesh);
    switch (shape)
    {
    case Shape::Cube:
        CreateCube(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Rectangular_Prism:
        CreatePrism(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Triangular_Pyramid:
        CreateTriPyramid(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Square_Pyramid:
        CreateSqrPyramid(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Rectangular_Pyramid:
        CreateRectPyramid(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Cilinder:
        CreateCilinder(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Cone:
        CreateCone(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Sphere:
        CreateSphere(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    case Shape::Plane:
        CreatePlane(mesh->num_vertices, mesh->num_indices, mesh->indices, mesh->vertices);
        break;
    }
        mesh->Initialization();
}

void ModuleCentralEditor::CreateDock()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", (bool*)true, window_flags);

    ImGui::PopStyleVar();

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}

