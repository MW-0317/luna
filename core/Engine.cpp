#include "Engine.h"
#include "Globals.h"

namespace luna
{
	Engine::Engine()
	{
		space = new Space();
		//stbi_set_flip_vertically_on_load(true);

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, "luna", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "MAIN::WINDOW_FAILED" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		glfwSwapInterval(1); // Enables vsync

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "MAIN::GLAD_INIT_FAILED" << std::endl;
			return;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460 core");

		glViewport(0, 0, this->windowWidth, this->windowHeight);
		glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	}

	Engine::~Engine()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
		delete space;
		delete window;
	}

	int Engine::run()
	{
		while (!glfwWindowShouldClose(window))
		{
			frameUpdate();
		}
		return 0;
	}

	void Engine::frameUpdate()
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (testWindow) {
			ImGui::Begin("Test Window", &testWindow);
			ImGui::Text("Hello World!");
			if (ImGui::Button("Close"))
				testWindow = false;
			ImGui::End();
		}
		ImGui::Render();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		space->updateSpace();
		space->getCamera()->processInput(window, space->getDelta());

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
}
