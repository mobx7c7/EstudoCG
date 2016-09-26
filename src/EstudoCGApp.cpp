#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Demos.h"

// produto escalar
// produto cruzado
// reflexão simples
// reflexão multipla
// reflexão com radiação
// intersecção

// Vetor de reflexão:
// Fórmula: R = 2 * (I . N) * N - I
// R = Raio resultante
// I = Raio incidente
// N = Normal do plano
// Todos os vetores de entrada devem estar normalizados .

using namespace ci;
using namespace ci::app;
using namespace std;

class EstudoCGApp : public App {
private:
	std::vector<CG::Demo::SceneRef> mDemos;
	std::vector<CG::Demo::SceneRef>::iterator mCurrentDemo;
  public:
	void setup() override;
	void mouseMove(MouseEvent event) override;
	void mouseDrag(MouseEvent event) override;
	void update() override;
	void draw() override;
};

void EstudoCGApp::setup()
{
	CG::Demo::Scene::Settings& sets = CG::Demo::Scene::GetEnviromentSettings();
	sets.SetFont(Font("Courier New", 20.0f));

#define CreateDemo(demo) mDemos.push_back(CG::Demo::Scene::Create<CG::Demo::##demo>())

	CreateDemo(LineIntersec);
	CreateDemo(DotProduct);
	
#undef CreateDemo

	mCurrentDemo = mDemos.begin();
}

void EstudoCGApp::mouseMove( MouseEvent event )
{
	CG::Demo::Scene::GetEnviromentSettings().SetCursorPosition(event.getPos());
}

void EstudoCGApp::mouseDrag(MouseEvent event)
{
	CG::Demo::Scene::GetEnviromentSettings().SetCursorPosition(event.getPos());
}

void EstudoCGApp::update()
{
}

void EstudoCGApp::draw()
{
	gl::clear(Color());

	if (mCurrentDemo == mDemos.end()){
		//std::advance(mCurrentDemo, 1);
		mCurrentDemo = mDemos.begin();
	}

	(*mCurrentDemo)->draw();
}

CINDER_APP(EstudoCGApp, RendererGl(RendererGl::Options().msaa(8)), [&](App::Settings *settings)
{
	settings->setWindowSize(512, 512);
	settings->setConsoleWindowEnabled();
	settings->disableFrameRate();
});
