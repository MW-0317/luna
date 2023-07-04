#include <main/luna.h>

int main()
{
	// luna::Render r = new Render();
	// r.run();
	// luna::Game g = new Game();
	// g.run();
	
	luna::Render* r = new luna::Render("test.mp4", 30.0f, 2.0f, 
						luna::WINDOW_WIDTH, luna::WINDOW_HEIGHT);
	r->run();
	r->save();

	//luna::Engine* e = new luna::Engine(luna::WINDOW_WIDTH, luna::WINDOW_HEIGHT);
	//e->run();

	//return luna::run();
	return 0;
}