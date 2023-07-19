#include "Engine.h"
#include "Globals.h"

namespace luna
{
	void Engine::init(int width, int height)
	{
		space = new Space();
		this->width = width;
		this->height = height;
		//stbi_set_flip_vertically_on_load(true);

		this->createWindow(width, height);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui::StyleColorsDark();

		if (!IMGUI_INIT)
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460 core");
			IMGUI_INIT = true;
		}

		glViewport(0, 0, width, height);
		glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	}

	LUNA_API Engine::Engine(int width, int height)
	{
		init(width, height);
	}

	Engine::Engine() {}

	Engine::~Engine()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(window);
		glfwTerminate();
		delete space;
		delete window;

		IMGUI_INIT = false;
	}

	int Engine::run()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		float delta = 0.0f;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			mainFrameUpdate(delta);

			ImGui::Render();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			space->updateSpace();
			space->getCamera()->processInput(window, space->getDelta());
			delta = space->getDelta();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
		return 0;
	}

	void Engine::mainFrameUpdate(float deltatime)
	{
		frameUpdate(deltatime);
	}

	void Engine::frameUpdate(float deltatime)
	{
		// TODO: Figure out windows/widgets
		if (testWindow) {
			ImGui::Begin("Test Window", &testWindow);
			ImGui::Text("Hello World!");
			if (ImGui::Button("Close"))
				testWindow = false;
			ImGui::End();
		}
	}

	void Engine::createWindow(int width, int height)
	{
		std::cout << "h" << std::endl;
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		this->window = glfwCreateWindow(width, height, "luna", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "MAIN::WINDOW_FAILED" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		// glfwSwapInterval(1); // Enables vsync, add to Game::Game()

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "MAIN::GLAD_INIT_FAILED" << std::endl;
			return;
		}
	}

	void Engine::clearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}
