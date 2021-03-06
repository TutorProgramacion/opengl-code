#include <iostream>
#include <string>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <imgui\imgui.h>

#include "imgui_impl_glfw_glad.h"

int main(int, char**)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Modern OpenGL Example", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    glfwSwapInterval(0);

    // Setup ImGui binding
    ImGui_ImplGlfwGlad_Init(window, true);

    //ImFontConfig config;
    //config.OversampleH = 3;
    //config.OversampleV = 1;

    //ImGuiIO& io = ImGui::GetIO(); 
    //io.Fonts->AddFontFromFileTTF(PROJECT_SOURCE_DIR "../resources/font/Cousine-Regular.ttf", 15.0f, &config);

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    unsigned int click_count = 0;
    bool opened = true;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplGlfwGlad_NewFrame();

        float alpha = 0.70f;
        ImVec2 size(300, 200);
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders;
        std::string textBtn = "(" + std::to_string(click_count) + ") Clicks";

        if (opened) {
            ImGui::Begin("Primera ventana", &opened, size, alpha, windowFlags);
            ImGui::Text("Muestra un texto en la ventana.");
            if (ImGui::Button(textBtn.c_str())) click_count++;
            ImGui::End();
        }

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            ImGuiStyle& style = ImGui::GetStyle();
            style.WindowRounding = 3.0f;

            static float f = 0.0f;

            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGlad_Shutdown();
    glfwTerminate();

    return 0;
}
