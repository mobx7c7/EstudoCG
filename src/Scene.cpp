#include "Scene.h"

namespace CG
{
	namespace Demo
	{
		const CameraOrtho& Scene::CSettings::GetCamera(){
			return mCamera;
		}

		const glm::ivec2& Scene::CSettings::GetCursorPosition(){
			return mCursorPosition;
		}

		const Font&	Scene::CSettings::GetFont(){
			return mFont;
		}

		Scene::Settings& Scene::CSettings::SetCamera(const CameraOrtho &camera){
			mCamera = camera;
			return *this;
		}

		Scene::Settings& Scene::CSettings::SetCursorPosition(const glm::ivec2 &cursor){
			mCursorPosition = cursor;
			return *this;
		}

		Scene::Settings& Scene::CSettings::SetFont(const Font &font){
			mFont = font;
			return *this;
		}

		std::vector<std::string>& Scene::GetTextLines(){
			return mTextLines;
		}

		Scene::Settings& Scene::GetEnviromentSettings(){
			static CSettings mEnviromentSettings;
			return mEnviromentSettings;
		}

		void Scene::onPreDraw()
		{
			CameraOrtho camera;
			camera.setOrtho(-5, 5, -5, 5, 0.001f, 100.0f);
			CG::Demo::Scene::GetEnviromentSettings().SetCamera(camera);
		}

		void Scene::draw(){

			const Font& textFont = GetEnviromentSettings().GetFont();
			glm::vec2 nextTextPosition;
			for (const std::string &text : mTextLines){
				gl::drawString(text, nextTextPosition, Color::white(), textFont);
				nextTextPosition.y += textFont.getSize() - textFont.getDescent();
			}

			onPreDraw();

			gl::ScopedMatrices scpMatrices;
			gl::setMatrices(CG::Demo::Scene::GetEnviromentSettings().GetCamera());

			onDraw();
		}
	}
}