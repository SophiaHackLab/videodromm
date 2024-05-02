#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "CinderNDIReceiver.h"

using namespace ci;
using namespace ci::app;

class BasicReceiverApp : public App {
  public:
	BasicReceiverApp();
	void setup() override;
	void update() override;
	void draw() override;
	void keyDown(KeyEvent event) override;

  private:
	CinderNDIReceiver mReceiver;
};



BasicReceiverApp::BasicReceiverApp()
	: mReceiver{}
{

}

void BasicReceiverApp::setup() 
{
	//mReceiver.setup();
	mReceiver.setup("test-cinder-video");
}

void BasicReceiverApp::update()
{
	mReceiver.update();
	if (mReceiver.isReady()) {
		int senderIndex = mReceiver.getCurrentSenderIndex();
		int senderCount = mReceiver.getNumberOfSendersFound();
		std::string senderName = mReceiver.getCurrentSenderName();
		getWindow()->setTitle("#" + std::to_string(senderIndex) + "/" + std::to_string(senderCount) + ": " + senderName + " @ " + std::to_string((int)getAverageFps()) + " FPS");
	} else {
		getWindow()->setTitle("connecting...");
	}

}

void BasicReceiverApp::draw()
{
	if (mReceiver.isReady()) {
		gl::clear(ColorA::black());
		auto meta = mReceiver.getMetadata();
		auto tex = mReceiver.getVideoTexture();
		if (tex.first) {
			Rectf centeredRect = Rectf(tex.first->getBounds()).getCenteredFit(getWindowBounds(), true);
			gl::draw(tex.first, centeredRect);
		}
	} else {
		gl::clear(ColorAf(0, 0, 1));
	}
	//CI_LOG_I( " Frame: " << tex.second << ", metadata: " << meta.first << " : " << meta.second );
}

void BasicReceiverApp::keyDown(KeyEvent event) {
	if (event.getCode() == KeyEvent::KEY_UP) {
		int currentIndex = mReceiver.getCurrentSenderIndex();
		if (currentIndex < mReceiver.getNumberOfSendersFound() -1) {
			mReceiver.switchSource(currentIndex + 1);
		}
	}
	if (event.getCode() == KeyEvent::KEY_DOWN) {
		int currentIndex = mReceiver.getCurrentSenderIndex();
		if (currentIndex >= 1) {
			mReceiver.switchSource(currentIndex - 1);
		}
	}
}

void prepareSettings( BasicReceiverApp::Settings* settings )
{
}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicReceiverApp, RendererGl, prepareSettings )
