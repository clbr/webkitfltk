#include "webkit.h"

int main() {

	webkitInit();
	Fl_Window *win = new Fl_Window(800, 600);
	webview *v = new webview(0, 0, 800, 600);
	win->end();
	win->show();

	v->load("http://google.com");

//  WebCore::Page* page = new WebCore::Page(pageClients);
//  WebCore::FrameLoaderClient* loaderClient = new WebCore::FrameLoaderClient();
//  RefPtr<WebCore::Frame> frame = WebCore::Frame::create(page, 0, loaderClient);
//  frame->init();
//  WebCore::ResourceRequest req = WebCore::ResourceRequest("http://google.com/");
//  frame->loader()->load(req, false);

	return Fl::run();
}
