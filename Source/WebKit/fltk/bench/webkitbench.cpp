/*
	(C) Lauri Kasanen
	Under the GPLv3.

	Sample app for webkitfltk that exits as soon as a page is fully loaded.
	Use for timed DOM/SVG/rendering benchmarks.
*/

#include "webkit.h"

static bool loaded = false;
static Fl_Window *win;

class myview: public webview {
public:
	myview(int x, int y, int w, int h): webview(x, y, w, h) {}

	void draw() override {
		webview::draw();

		// If we drew after the page was loaded, time to exit
		if (loaded) win->hide();
	}

};

static myview *v;

static void progress(webview *, const float amount) {
	if (amount > 0.999f) {
		loaded = true;
	}
}

int main(int argc, char **argv) {

	webkitInit();
	win = new Fl_Window(800, 600);
	v = new myview(0, 0, 800, 600);
	win->end();
	win->show(argc, argv);

	v->progressChangedCB(progress);

	if (argc > 1)
		v->load(argv[1]);
	else
		v->load("http://google.com");

	Fl::run();

	// Give everything the chance to cleanup
	delete win;
	wk_drop_caches();

	return 0;
}
