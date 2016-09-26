#pragma once
#include "CG.h"

namespace CG
{
	namespace Demo
	{
		typedef std::shared_ptr<class Scene> SceneRef;

		class Scene
		{
		public:

			class Settings{
			public:
				virtual ~Settings() = default;
				virtual const CameraOrtho&	GetCamera() = 0;
				virtual const glm::ivec2&	GetCursorPosition() = 0;
				virtual const Font&			GetFont() = 0;
				virtual Settings& SetCamera(const CameraOrtho &camera) = 0;
				virtual Settings& SetCursorPosition(const glm::ivec2 &cursor) = 0;
				virtual Settings& SetFont(const Font &font) = 0;
			};

		private:

			class CSettings : public Settings
			{
			private:
				glm::ivec2	mCursorPosition;
				CameraOrtho mCamera;
				Font		mFont;
			public:
				const CameraOrtho&	GetCamera();
				const glm::ivec2&	GetCursorPosition();
				const Font&			GetFont();
				Settings&			SetCamera(const CameraOrtho &camera);
				Settings&			SetCursorPosition(const glm::ivec2 &cursor);
				Settings&			SetFont(const Font &font);
			};

			std::vector<std::string> mTextLines;
			virtual void onPreDraw();
			virtual void onDraw(){}

		protected:

			std::vector<std::string>& GetTextLines();

		public:

			void				draw();
			static Settings&	GetEnviromentSettings();

			template<typename T>
			static SceneRef Create(){
				return SceneRef(new T());
			}
		};
	}
}