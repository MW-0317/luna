#include "Engine.h"
#include "Globals.h"

namespace luna
{
	void Engine::init(int width, int height)
	{
		this->createWindow(width, height);

		CameraSettings cs = CameraSettings();
		cs.width = width;
		cs.height = height;
		cs.cameraType = CameraType::Perspective;
		cs.window = window;
		Camera* cam = new Camera(cs);

		space = new Space(cam);
		this->width = width;
		this->height = height;
		//stbi_set_flip_vertically_on_load(true);

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
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	}

	LUNA_API Engine::Engine(int width, int height)
	{
		init(width, height);
	}

	Engine::Engine()
	{}
	
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

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, width, height);
		glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		float delta = 0.0f;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			FrameProps fp;
			fp.deltatime = delta;
			mainFrameUpdate(fp);

			ImGui::Render();

			RenderProps renderProps;
			renderProps.width = this->width;
			renderProps.height = this->height;
			space->frameUpdate(renderProps);
			if (space->tickUpdate())
			{
				TickProps tp;
				tp.deltatime = space->getDeltaTick();
				tickUpdate(tp);
			}	
			delta = space->getDelta();

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
		}
		glfwTerminate();
		return 0;
	}

	void Engine::mainFrameUpdate(FrameProps fp)
	{
		frameUpdate(fp);
	}

	void Engine::frameUpdate(FrameProps fp)
	{
		// TODO: Figure out windows/widgets
		if (debug) {
			ImGui::Begin("Example Window", &exampleWindow);
			
			

			ImGui::End();
		}
	}

	void Engine::tickUpdate(TickProps fp)
	{
		// Basically complete virtual function, however will be used as a template.
	}

	void Engine::createWindow(int width, int height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		this->window = glfwCreateWindow(width, height, "lunaE", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "MAIN::WINDOW_FAILED" << std::endl;
			std::cout << "MAIN::WINDOW_WIDTH::" << width << " "
				<< "MAIN::WINDOW_HEIGHT::" << height << std::endl;
			glfwTerminate();
			abort();
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

	Space* Engine::getSpace()
	{
		return this->space;
	}

	void Engine::clearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}
}
