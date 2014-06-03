#include <config.h>
#include <Page.h>
#include <Frame.h>
#include <FrameLoaderClient.h>
#include <InitializeThreading.h>
#include <platform/Widget.h>
#include <EmptyClients.h>

int main(int argc, char* argv[]) {
  WebCore::Page::PageClients pageClients;

// More:
  JSC::initializeThreading();
  WTF::initializeMainThread();
  WebCore::fillWithEmptyClients(pageClients);
  WebCore::Page* page = new WebCore::Page(pageClients);
//  WebCore::FrameLoaderClient* loaderClient = new WebCore::FrameLoaderClient();
//  RefPtr<WebCore::Frame> frame = WebCore::Frame::create(page, 0, loaderClient);
//  frame->init();
//  WebCore::ResourceRequest req = WebCore::ResourceRequest("http://google.com/");
//  frame->loader()->load(req, false);

  return 0;
}
