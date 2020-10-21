#include "Globals.h"
#include "Application.h"
#include "ModuleCentralEditor.h"
#include "External Libraries/Glew/include/glew.h"
#include "External Libraries/MathGeoLib/include/MathGeoLib.h"
#include "External Libraries/ImGui/imgui.h"
#include "External Libraries/ImGui/imgui_impl_sdl.h"
#include "External Libraries/ImGui/imgui_impl_opengl3.h"
#include "External Libraries/SDL/include/SDL.h"
#include "External Libraries/SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <Windows.h>
#include <list>
#include <stdio.h>
#include "ModuleWindow.h"
#include "Console.h"
#include "ModuleInput.h"
#include "ModuleSceneIntro.h"
#include "Shape.h"


ModuleCentralEditor::ModuleCentralEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
    progress = 0.f;
    progress2 = 50.f;
    progress3 = 50.f;
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

    /*
    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = glewInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
    bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
    bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
    bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
#else
    bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }
    */
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->window->gl_context);
    ImGui_ImplOpenGL3_Init();
    show_demo_window = true;
    show_another_window = false;
    show_example = false;
    show_about = false;
    show_configuration = false;
    show_console = false;
    show_openglOptions = false;
    depth = cullface = lighting = colorMaterial = texture = ambient = stencil = wireframe = false;
    progress = 50.f;
    progress2 = 50.f;
    progress3 = 50.f;
    progress4 = 50.f;
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
    if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
        show_configuration = !show_configuration;
    if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
        show_console = !show_console;


    { // UPSIDE BAR
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Quit", "Esc")) {
                return update_status::UPDATE_STOP;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Console", "1")) {
                show_console = !show_console;
            }
            if (ImGui::MenuItem("Configuration", "4")) {
                show_configuration = !show_configuration;
            }
            if (ImGui::MenuItem("OpenGL Options")) {
                show_openglOptions = !show_openglOptions;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("GameObject")) {
            if (ImGui::MenuItem("Create Empty")) {
                App->scene_intro->CreateGameObject(Shape::Empty);
            }
            if (ImGui::BeginMenu("3D Object")) {
                if (ImGui::MenuItem("Cube")) {
                    App->scene_intro->CreateGameObject(Shape::Cube);
                }
                if (ImGui::MenuItem("Rectangular Prism")) {
                    App->scene_intro->CreateGameObject(Shape::RectangularPrism);
                }
                if (ImGui::MenuItem("Triangular Pyramid")) {
                    App->scene_intro->CreateGameObject(Shape::TriangularPyramid);
                }
                if (ImGui::MenuItem("Square Pyramid")) {
                    App->scene_intro->CreateGameObject(Shape::SquarePyramid);
                }
                if (ImGui::MenuItem("Rectangular Pyramid")) {
                    App->scene_intro->CreateGameObject(Shape::RectangularPyramid);
                }
                if (ImGui::MenuItem("Cilinder")) {
                    App->scene_intro->CreateGameObject(Shape::Cilinder);
                }
                if (ImGui::MenuItem("Cone")) {
                    App->scene_intro->CreateGameObject(Shape::SolidCone);
                }
                if (ImGui::MenuItem("Sphere")) {
                    App->scene_intro->CreateGameObject(Shape::SolidSphere);
                }
                ImGui::EndMenu();
            }
            if(ImGui::BeginMenu("2D Object")) {
                if (ImGui::MenuItem("Plane")) {
                    App->scene_intro->CreateGameObject(Shape::SolidPlane);
                }
                ImGui::EndMenu();
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
        ImGui::Begin("About Falcas Engine", NULL);
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
        ImGui::BulletText("SDL Mixer 2.0");
        ImGui::BulletText("Cereal 1.2.2");
        ImGui::BulletText("Glew 7.0");
        ImGui::BulletText("ImGui 1.51");
        ImGui::BulletText("MathGeoLib 1.5");
        ImGui::BulletText("OpenGL 3.1");
        ImGui::BulletText("Assimp 3.1.1");
        ImGui::BulletText("Devil 1.7.8");
        ImGui::BulletText("Assimp 5.0.1");
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
        ImGui::Begin("Configuration");
        ImGui::Text("Options");
        if(ImGui::CollapsingHeader("Window")) {
            //BRIGHT
            ImGui::SliderFloat("Bright", &progress, 0.0f, 100.0f);
            SDL_SetWindowBrightness(App->window->window, (progress*2)/100);

            //SIZE WINDOW
            ImGui::SliderFloat("Width", &progress2, 0.0f, 100.0f);
            ImGui::SliderFloat("Height", &progress3, 0.0f, 100.0f);
            SDL_SetWindowSize(App->window->window, (int)(progress2*1280/50) * SCREEN_SIZE, (int)(progress3 * 720 / 50)* SCREEN_SIZE);
        }
        if (ImGui::CollapsingHeader("Application")) {
            float fr[50];
            float ms[50];
            char fr_char[50];
            char ms_char[50];
            std::copy(fr_arr.begin(), fr_arr.end(), fr);
            std::copy(ms_arr.begin(), ms_arr.end(), ms);
            sprintf_s(fr_char,50, "%f Framerate", fr[49]);
            sprintf_s(ms_char,50, "%f Milliseconds", ms[49]);
            ImGui::PlotHistogram("", fr, 50, 0, fr_char, 0.0f, 150.0f, ImVec2(0, 80.0f));
            ImGui::PlotHistogram("", ms, 50, 0, ms_char, 0.0f, 30.f, ImVec2(0, 80.0f));
        }
        if (ImGui::CollapsingHeader("Input")) {
            ImGui::TextUnformatted(input_list.begin());
            if (need_scroll)
                ImGui::SetScrollHere(1.0f);
            need_scroll = false;
        }
        if (ImGui::CollapsingHeader("Audio")) {
            ImGui::SliderFloat("", &progress4, 0.0f, 100.0f);
            //code goes here
        }
        if (ImGui::CollapsingHeader("Hardware")) {
            ImGui::Text("SDL Version: ");
            ImGui::SameLine();
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "2.0");
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
            if(SDL_Has3DNow) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "3D Now "); ImGui::SameLine();
            if(SDL_HasAVX) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX "); ImGui::SameLine();
            if(SDL_HasAVX2) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AVX2 "); ImGui::SameLine();
            if(SDL_HasAltiVec) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "AltiVec "); ImGui::SameLine();
            if(SDL_HasMMX) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "MMX "); ImGui::SameLine();
            if(SDL_HasRDTSC) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "RDTSC "); ImGui::SameLine();
            if(SDL_HasSSE) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE "); ImGui::SameLine();
            if(SDL_HasSSE2) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE2 "); ImGui::SameLine();
            if(SDL_HasSSE3) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE3 "); ImGui::SameLine();
            if(SDL_HasSSE41) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE41 "); ImGui::SameLine();
            if(SDL_HasSSE42) ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "SSE42 "); ImGui::SameLine();
            ImGui::Separator();
            //no me detecta la funcion glgetstring
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
        //App->console->AddLog("HEY");
        ImGui::Begin("Console");
        console_logs = App->console->GetLogs();
        for (int i = 0; i < console_logs.size(); i++) {
            ImGui::Text(console_logs.at(i));
        }
        ImGui::End();
    }
    
    if (show_openglOptions)
    {     
        ImGui::Begin("OpenGL Options");
        ImGui::Checkbox("Depth Test", &depth);
        ImGui::Checkbox("Cull Face", &cullface);
        ImGui::Checkbox("Light", &lighting);
        ImGui::Checkbox("Color Material", &colorMaterial);
        ImGui::Checkbox("Texture", &texture);
        ImGui::Checkbox("Ambient", &ambient);
        ImGui::Checkbox("Stencil", &stencil);
        ImGui::Checkbox("Wireframe", &wireframe);

        if (!depth) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
        if (!cullface) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);
        if (!lighting) glEnable(GL_LIGHTING);
        else glDisable(GL_LIGHTING);
        if (!colorMaterial) glEnable(GL_COLOR_MATERIAL);
        else glDisable(GL_COLOR_MATERIAL);
        if (!texture) glEnable(GL_TEXTURE_2D);
        else glDisable(GL_TEXTURE_2D);
        if (!ambient) glEnable(GL_STENCIL_TEST);
        else glDisable(GL_STENCIL_TEST);
        if (!stencil) glEnable(GL_AMBIENT);
        else glDisable(GL_AMBIENT);
        ImGui::End();
    }
    // Rendering
    ImGui::Render();
    //glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    //glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    //glClear(GL_COLOR_BUFFER_BIT);
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

    return UPDATE_CONTINUE;
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

