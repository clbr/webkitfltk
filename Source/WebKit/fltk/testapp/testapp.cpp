#include "webkit.h"

static webview *v;

static void dl(const char *url, const char *file) {
	printf("Download %s to %s finished.\n",
		url, file);
}

int main(int argc, char **argv) {

	webkitInit();
	Fl_Window *win = new Fl_Window(800, 600);
	v = new webview(0, 0, 800, 600);
	win->end();
	win->resizable(v);
	win->show(argc, argv);

	if (argc > 1)
		v->load(argv[1]);
	else
		v->load("http://google.com");

	wk_set_download_func(dl);

	// The mainloop
	Fl::run();

	// Give everything the chance to cleanup
	delete win;
	wk_drop_caches();

	return 0;
}
