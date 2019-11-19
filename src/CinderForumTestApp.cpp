#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderForumTestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void CinderForumTestApp::setup()
{
}

void CinderForumTestApp::mouseDown( MouseEvent event )
{
}

void CinderForumTestApp::update()
{
}

void CinderForumTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( CinderForumTestApp, RendererGl )
