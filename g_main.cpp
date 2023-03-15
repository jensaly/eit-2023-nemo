#include "g_utility.h"
#include "yard.h"

void PlotQueues(Ferry& ferry) {
    for (int i = 0; i < ferry.queues.size(); i++) {
        auto& q = ferry.queues[i];
        for (int j = 0; j < ferry.queues[i].vehicles.size(); j++) {
            auto& v = q.vehicles[j];
            ImVec2 bottom = ImPlot::PlotToPixels(ImVec2(v.x, v.y));
            ImVec2 top = ImPlot::PlotToPixels(ImVec2(v.x + v.length, v.y + v.width));
            ImPlot::GetPlotDrawList()->AddRectFilled(bottom, top, ImGui::ColorConvertFloat4ToU32(ImVec4(v.col[0], v.col[1], v.col[2], v.col[3])));
        }
        double xvals[2] = {0, q.total_size};
        double yvals[2] = {q.width * (i+1), q.width * (i+1)};
        ImPlot::PlotLine(("Line " + std::to_string(i)).c_str(), xvals, yvals, 2);
        float car_x = ferry.car_com.first, car_y = ferry.car_com.second;
        ImPlot::PlotScatter("Car COM", &car_x, &car_y, 1);
        float x = ferry.com.first, y = ferry.com.second;
        ImPlot::PlotScatter("COM", &x, &y, 1);
    }
    ImPlot::EndPlot();
}

void PlotQueues(Yard& yard) {
    for (int i = 0; i < yard.queues.size(); i++) {
        auto& q = yard.queues[i];
        for (int j = 0; j < yard.queues[i].vehicles.size(); j++) {
            auto& v = q.vehicles[j];
            ImVec2 bottom = ImPlot::PlotToPixels(ImVec2(v.x, v.y));
            ImVec2 top = ImPlot::PlotToPixels(ImVec2(v.x + v.length, v.y + v.width));
            ImPlot::GetPlotDrawList()->AddRectFilled(bottom, top, ImGui::ColorConvertFloat4ToU32(ImVec4(v.col[0], v.col[1], v.col[2], v.col[3])));
        }
        double xvals[2] = {0, q.total_size};
        double yvals[2] = {q.width * (i+1), q.width * (i+1)};
        ImPlot::PlotLine(("Line " + std::to_string(i)).c_str(), xvals, yvals, 2);
    }
    ImPlot::EndPlot();
}

int main(int argc, char* argv[]) {
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    SDL_Window* window = SDL_CreateWindow("Voidal", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(-1); // Enable vsync
    //auto icon = IMG_Load("voidal.png");
    //SDL_SetWindowIcon(window, icon);

    //SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    //auto icon = IMG_Load("voidal.png");
    //SDL_SetWindowIcon(window, icon);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    srand(time(NULL));
    Yard y = Yard(BasicRules(), OptimizeCOM(), 5, 60, 4);
    y.queues[0].SetReservedFlag(VehicleFlags::Ambulance);
    y.queues[0].SetReservedFlag(VehicleFlags::HC);
    y.queues[2].SetPriorityFlag(VehicleFlags::Heavy);
    Ferry ferry{5, 17.8, 1.9, 5 * 1.9, 18.0};
    int state = 0; // 0 is no cars, 1 is cars in yard, 2 is cars in ferry
    while (!done)
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowBgAlpha(1);
        ImGui::Begin("Main");
        if (ImGui::BeginCombo("Coarse Algorithm", y.c_algorithm->AlgorithmName().c_str())) {
            if (ImGui::Selectable("Worst Fit")) { y.SetCoarseAlgorithm<WorstFit>(); }
            if (ImGui::Selectable("Best Fit")) { y.SetCoarseAlgorithm<BestFit>(); }
            if (ImGui::Selectable("Basic Rules")) { y.SetCoarseAlgorithm<BasicRules>(); }
            ImGui::EndCombo();
        }
        if (ImGui::BeginCombo("Fine Algorithm", y.f_algorithm->AlgorithmName().c_str())) {
            if (ImGui::Selectable("Worst Fit")) { y.SetFineAlgorithm<WorstFit>(); }
            if (ImGui::Selectable("Best Fit")) { y.SetFineAlgorithm<BestFit>(); }
            if (ImGui::Selectable("Basic Rules")) { y.SetFineAlgorithm<BasicRules>(); }
            if (ImGui::Selectable("Optimize COM")) { y.SetFineAlgorithm<OptimizeCOM>(); }
            ImGui::EndCombo();
        }
        if (state == 0 && ImGui::Button("Generate cars")) {
            y.clear();
            ferry.clear();
            y.SimulteQueueArrival(std::gamma_distribution<double>(1.4, 1.5), 60);
            state = 1;
        }
        else if (state == 1 && ImGui::Button("Embark")) {
            y.Embark(ferry);
            state = 0;
        }
        if (ImPlot::BeginPlot("Cars", ImVec2(-1, -1), ImPlotFlags_AntiAliased | ImPlotFlags_Equal)) {
            if (state == 1) PlotQueues(y);
            else PlotQueues(ferry);
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}