#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include <cmath>
#include "cinder/Perlin.h"
#include "cinder/ImageIO.h"
#include "cinder/Filesystem.h"
#include "cinder/Utilities.h"
#include <string>

using namespace ci;
using namespace ci::app;
using namespace std;

class CinderForumTestApp : public App {
  public:
    const int FBO_W = 600;
    const int FBO_H = 600;
    int width = cinder::app::getWindowWidth();
    int height = cinder::app::getWindowHeight();
    vector<vec2> offsetArray;
    int offsetSize = 5;

    gl::FboRef mScene;
    gl::FboRef mHDRComp;
    gl::TextureRef mTexture;
    gl::GlslProgRef mCompShader;
    CameraPersp mCam;
    
    void setup() override;
    void update() override;
    void draw() override;
    void prepareSettings(Settings *settings);
};

void CinderForumTestApp::setup()
{
    // this is to initialize positions to sample from the texture
    // to create a layered smoky effect in the background
    for (int i = 0; i < offsetSize; i++){
        offsetArray.push_back(vec2(randFloat(-0.05, 0.05), randFloat(-0.05, 0.05)));
    }
    
    //Frame Buffer Stuff - creating composite frame buffer
    mHDRComp = gl::Fbo::create(FBO_W, FBO_H);
    //creating base frame buffer
    mScene = gl::Fbo::create(toPixels(getWindowWidth()),toPixels(getWindowHeight()));
    //background texture
    mTexture = gl::Texture::create(loadImage(loadAsset("smoke2.jpg")));
    //setting up camera
    vec3 camPos = glm::vec3(0,0,800);
    mTexture -> setWrap(GL_REPEAT, GL_REPEAT);
    mCam.lookAt(camPos, glm::vec3(0.0f), glm::vec3(0,1,0));
    mCam.setPerspective(75, getWindowAspectRatio(), 01.01f, 2500.0f);
    mCompShader = gl::GlslProg::create(loadAsset("comp.vert"), loadAsset("comp.frag"));
}

void CinderForumTestApp::update()
{
}

void CinderForumTestApp::draw()
{
    gl::clear( Color( 0, 0, 0 ) );

    //Base Frame Buffer
    {
        //scoping it to frame buffer. It draws fine without the scoping
        // but once you uncomment it, the scene seems to get stretched out
        // and translate. This is the main problem area
        /*
        gl::ScopedFramebuffer scpScene(mScene);
        gl::ScopedMatrices scpCam;
         */
         
        gl::setMatrices(mCam);
                
        //clear with white to see whether it draws
        gl::clear( Color( 1, 1, 1 ) );
        
        gl::ScopedGlslProg scpGL(gl::getStockShader(gl::ShaderDef().texture().lambert().color()));
        
        //drawing sphere at 0,0 (which should be at the center of the screen)
        gl::drawSphere(vec3(0,0,0),200);
    }
    
    // composite buffer, layering the images on top
    // commenting this out because there seems to be something wrong with the
    // mScene buffer itself, but not this one
    /*
    {
        gl::ScopedFramebuffer scpScene(mHDRComp);
        gl::ScopedGlslProg scpCom(mCompShader);
        gl::ScopedTextureBind scpSceneTex(mScene->getColorTexture(),0);
        gl::ScopedTextureBind scpComposite(mTexture,1);
        mCompShader->uniform("tex0",0);
        mCompShader->uniform("tex1",1);
        vec2 arr [offsetSize];
        for(int i = 0; i < offsetSize; i++){
            arr[i] = offsetArray[i];
        }
        //passing array of initial starts to shader
        mCompShader->uniform("offsetArray", arr, offsetSize);
        gl::drawSolidRect(getWindowBounds());
    }
     */
    //drawing frame buffer to screen
    {
        gl::ScopedMatrices scpa;
        gl::ScopedDepth scpD(false);
        
        // scoped Viewport is also acting very strange when the mScene
        // scoping is uncommented. I'm not sure how it's translating stuff
//        gl::ScopedViewport scpVP(ivec2(0), ivec2(toPixels(getWindowWidth()), toPixels(getWindowHeight())));
        
        gl::ScopedTextureBind scpScene(mScene-> getColorTexture(),0);
        gl::ScopedGlslProg scpGL(gl::getStockShader(gl::ShaderDef().texture()));
        gl::pushMatrices();
        gl::translate(vec3(-300,300,0));
        gl::drawSolidRect(getWindowBounds());
        gl::popMatrices();
    }
    //trying to print out mScene to see if there is something wrong
    // with the mScene buffer or something else
    // seems to be something wrong with the Frame Buffer itself
    if(getElapsedFrames() < 5){
        string filepath = "<add your filepath>";
        writeImage( filepath+ "testMScene.tif", mScene->readPixels8u(mScene->getBounds()));
    }
}

CINDER_APP( CinderForumTestApp, RendererGl, [&](App::Settings *settings){
    settings->setWindowSize(600, 600);
    settings->setFrameRate(60.0f);
   settings->setHighDensityDisplayEnabled(false);
})

