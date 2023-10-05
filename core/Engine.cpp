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
		//cs.movement = false;
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
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		if (!IMGUI_INIT)
		{
			ImGui_ImplGlfw_InitForOpenGL(window, true);
			ImGui_ImplOpenGL3_Init("#version 460 core");
			IMGUI_INIT = true;
		}
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_BLEND);
		glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glViewport(0, 0, width, height);
		glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		float delta = 0.0f;
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Frame frame;
			frame.engine = this;
			frame.width = this->width;
			frame.height = this->height;
			frame.deltatime = delta;

			Tick tick;
			tick.engine = this;
			tick.width = this->width;
			tick.height = this->height;
			tick.deltatime = delta;

			mainFrameUpdate(frame);
			mainTickUpdate(tick);
			space->frameUpdate(renderProps);
			if (space->tickUpdate())
			{
				TickProps tp;
				tp.deltatime = space->getDeltaTick();
				tickUpdate(tp);
			}	
			delta = space->getDelta();

			ImGui::Render();
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_h, &display_w);
			glViewport(0, 0, display_h, display_w);

			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				GLFWwindow* backup_current_context = glfwGetCurrentContext();
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				glfwMakeContextCurrent(backup_current_context);
			}
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
			oLog.update("deltaframe", fp.deltatime);
			log.draw("Log:", &debug);
			oLog.draw("Overlay:");
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
