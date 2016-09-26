#include "..\include\Demos.h"

namespace CG
{
	namespace Demo
	{
		DotProduct::DotProduct()
		{
			std::vector<std::string> &textLines = GetTextLines();
			textLines.push_back("REFLEXO VETORIAL:");
			textLines.push_back("");
			textLines.push_back("Modulo   = distancia, comprimento, hipotenusa, sqrt(v.x * v.x + v.y * v.y) ou |v|");
			textLines.push_back("Escalar  = |a||b|");
			textLines.push_back("Reflexao = 2 * |I||N| * N - I (N=Normal, I=Incidente)");
		}

		void DotProduct::onDraw()
		{
			static float theta1 = 0.0f;
			theta1 += 0.01f;
			//float s = sin(theta1);
			//float c = cos(theta1);
			//glm::mat2 rot{ s, c, -c, s };

			CG::Face plane;
			plane.surface.v1 = glm::rotate(CG::Coord2(-1.0f, 0.0f), theta1);
			plane.surface.v2 = glm::rotate(CG::Coord2(1.0f, 0.0f), theta1);
			plane.normal = glm::rotate(CG::Coord2(0.0f, 1.0f), theta1); // Pré-calculado

			glm::vec2 incidental(gl::windowToWorldCoord(GetEnviromentSettings().GetCursorPosition()));
			float dotProduct = glm::dot(incidental, plane.normal);
			glm::vec2 reflected(2.0f * dotProduct * plane.normal - incidental);

			// caso: dot > 0
			// caso: |I||N| N
			// caso: 2.0 |I||N| N - I

			gl::ScopedBlendAdditive scpBlendAdd;
			gl::color(Color(0.0f, 0.0f, 1.0f));
			gl::drawLine(plane.surface.v1, plane.surface.v2);
			gl::color(Color(1.0f, 0.0f, 0.0f));
			gl::drawLine(plane.position, plane.normal);

			if (dotProduct > 0){
				gl::color(Color(1.0f, 1.0f, 0.0f));
				gl::drawLine(plane.position, reflected);
			}

			gl::color(Color(0.0f, 1.0f, 0.0f));
			gl::drawLine(plane.position, incidental);

			// Clipping
			// Reflection
			// Refraction
			// Intersection

			// 2D Rasterizer
			// - Line
			// - Triangle
			// Transformations & Clipping
			// - Perspective
			// - Frustrum
			// 3D Rasterizer
			// - Light shader (Flat, Gouraud)
			// - Texture mapping
			// 
		}

		LineIntersec::LineIntersec()
		{
			std::vector<std::string> &textLines = GetTextLines();
			textLines.push_back("INTERSECCAO LINEAR + PRODUTO ESCALAR (DOT):");
		}

		void LineIntersec::drawLine(const Line& line, const ColorA &color)
		{
			//Line
			gl::color(color);
			gl::drawLine(line.v1, line.v2);
			//Bounds
			gl::color(ColorA(color, 0.25f));
			gl::drawStrokedRect(Rectf(line.v1,line.v2));
			gl::drawStrokedCircle(line.v1, 0.1f, 32);
			gl::drawStrokedCircle(line.v2, 0.1f, 32);
		}

		Coord2 LineIntersec::findIntersecPoint(const Line &intersec, const Line &plane)
		{
			
			// se estiver fora da área ou angulo menor que 0
			// desenhar linha anterior
			//
			// - delta de cada linha
			// - determinante com os dois 'delta' resultantes
			// - produto cruzado de cada linha
			//
			// det == 0: linhas são paralelas.

			Coord2 delta1, delta2; // d1, d2
			float cross1, cross2; // c
			float determinant; //det
			delta1 = intersec.v1 - intersec.v2; // delta
			delta2 = plane.v1 - plane.v2; // delta
			determinant = (delta1.x * delta2.y) - (delta1.y * delta2.x); // determinanterminante?
			cross1 = intersec.v1.x * intersec.v2.y - intersec.v1.y * intersec.v2.x; // produto cruzado?
			cross2 = plane.v1.x * plane.v2.y - plane.v1.y * plane.v2.x; // produto cruzado?
			return Coord2(cross1 * delta2.x - cross2 * delta1.x, cross1 * delta2.y - cross2 * delta1.y) / determinant; // ponto de intersecção
		}


		void LineIntersec::onDraw()
		{
			static const Color colorR(1, 0, 0);
			static const Color colorG(0, 1, 0);
			static const Color colorB(0, 0, 1);
			static const Color colorC(0, 1, 1);
			static const Color colorM(1, 0, 1);
			static const Color colorY(1, 1, 0);

			Line intersec{
				Coord2(gl::windowToWorldCoord(GetEnviromentSettings().GetCursorPosition())),
				Coord2(0.0f, -2.0f),
			};
			Line plane{
				Coord2(-1.0f, 0.0f),
				Coord2(1.0f, 0.0f),
			};

			Coord2 planeNormal(0.0f, 1.0f);

			static float ang = 0.0f;
			ang += 0.1f;
			float theta1 = toRadians(ang);

			intersec.v2 = glm::rotate(intersec.v2, toRadians(45.0f));
			plane.v1	= glm::rotate(plane.v1, theta1);
			plane.v2	= glm::rotate(plane.v2, theta1);
			planeNormal = glm::rotate(planeNormal, theta1);
			planeNormal = glm::normalize(planeNormal);

			Coord2 intersecPoint = findIntersecPoint(intersec, plane);

			////////////////////////////////////////////////////////////////////////
		
			gl::ScopedBlendAdditive scpBlendAdd;

			Rectf intersecBounds(intersec.v1, intersec.v2);
			Rectf planeBounds(plane.v1, plane.v2);

			//Normal
			gl::color(colorR);
			gl::drawLine(planeBounds.getCenter(), planeNormal);
			//Plane
			drawLine(plane, colorG);

			//gl::color(ColorA(Color::white(), 0.25f));
			//gl::drawSolidRect(intersecBounds);
			//gl::drawStrokedCircle(intersecPoint, 0.1f, 32);
			
			float dotPlaneNormal		= glm::dot(intersec.v1, planeNormal);

			float distPlane1			= glm::distance2(intersecPoint, plane.v1);
			float distPlane2			= glm::distance2(intersecPoint, plane.v2);
			float distPlanePoints		= glm::distance2(plane.v1, plane.v2);
			bool insidePlaneLine		= distPlane1 > distPlanePoints || distPlane2 > distPlanePoints;

			float distIntersec1			= glm::distance2(intersecPoint, intersec.v1);
			float distIntersec2			= glm::distance2(intersecPoint, intersec.v2);
			float distIntersecPoints	= glm::distance2(intersec.v1, intersec.v2);
			bool insideIntersecLine		= distIntersec1 > distIntersecPoints || distIntersec2 > distIntersecPoints;

			if (insidePlaneLine || insideIntersecLine){
				drawLine(intersec, colorB);
			}
			else if (dotPlaneNormal < 0){
				drawLine(Line{ intersec.v1, intersecPoint }, colorC);
				drawLine(Line{ intersecPoint, intersec.v2 }, colorY);
			}else{
				drawLine(Line{ intersec.v1, intersecPoint }, colorY);
				drawLine(Line{ intersecPoint, intersec.v2 }, colorC);
			}
		}
	};
}