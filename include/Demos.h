#pragma once
#include "Scene.h"

namespace CG
{
	namespace Demo
	{
		class DotProduct : public Scene
		{
		public:
			DotProduct();
		private:
			void onDraw() override;
		};

		class LineIntersec : public Scene
		{
		public:
			LineIntersec();
		private:
			void	drawLine(const Line &line, const ColorA &color);
			Coord2	findIntersecPoint(const Line &intersec, const Line &plane);
			void	onDraw() override;
		};
	}
}