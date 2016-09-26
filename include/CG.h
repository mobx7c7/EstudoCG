#pragma once
#include "cinder\gl\gl.h"

using namespace ci;

namespace CG
{
	typedef glm::vec2 Coord2;
	typedef glm::vec3 Coord3;
	typedef glm::vec4 Coord4;

	struct Vertex
	{
		Coord2 position, normal;
	};

	struct Line
	{
		Coord2 v1, v2;
	};

	struct Face
	{
		Line	surface;
		Coord2	position;
		Coord2	normal;
	};

	class Poly
	{
	public:
		class Settings
		{
		public:
			virtual ~Settings() = default;
			virtual bool IsBoundsEnabled() = 0;
			virtual	Settings& SetBounds(bool enable = true) = 0;
		};
	private:
		class CSettings : public Settings
		{
		private:
			bool mBounds;
		public:
			CSettings();
			bool IsBoundsEnabled();
			Settings& SetBounds(bool enable);
		};
		std::vector<Line>	mLines;
		Color				mColor;
		Rectf				mBounds;
		Poly();
	public:
		void				draw();
		static Settings&	GetEnviromentSettings();
	};
}
