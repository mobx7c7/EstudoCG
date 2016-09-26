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

			Line lineA{
				Coord2(gl::windowToWorldCoord(GetEnviromentSettings().GetCursorPosition())),
				Coord2(0.0f, -2.0f),
			};
			Line lineB{
				Coord2(-1.0f, 0.0f),
				Coord2(1.0f, 0.0f),
			};

			Coord2 normalLineB(0.0f, 1.0f);

			////////////////////////////////////////////////////////////////////////

			static float ang = 0.0f;
			ang += 0.1f;
			float theta1 = toRadians(ang);

			lineA.v2	= glm::rotate(lineA.v2, toRadians(45.0f));
			lineB.v1	= glm::rotate(lineB.v1, theta1);
			lineB.v2	= glm::rotate(lineB.v2, theta1);
			normalLineB = glm::rotate(glm::normalize(normalLineB), theta1);

			Coord2 intersecPoint = findIntersecPoint(lineA, lineB);

			////////////////////////////////////////////////////////////////////////
		
			gl::ScopedBlendAdditive scpBlendAdd;

			Rectf intersecBounds(lineA.v1, lineA.v2);
			Rectf planeBounds(lineB.v1, lineB.v2);

			//Normal
			gl::color(colorR);
			gl::drawLine(planeBounds.getCenter(), normalLineB);
			//Plane
			drawLine(lineB, colorG);

			//gl::color(ColorA(Color::white(), 0.25f));
			//gl::drawSolidRect(intersecBounds);
			//gl::drawStrokedCircle(intersecPoint, 0.1f, 32);

			Coord2 distIntersecByLineA, distIntersecByLineB;
			float dotPlaneNormal, distPointsLineA, distPointsLineB;
			bool intersecOnLineA, intersecOnLineB;
		
			dotPlaneNormal = glm::dot(lineA.v1, normalLineB);

			distIntersecByLineA = Coord2(glm::distance2(intersecPoint, lineA.v1), glm::distance2(intersecPoint, lineA.v2));
			distPointsLineA = glm::distance2(lineA.v1, lineA.v2);
			intersecOnLineA = distIntersecByLineA[0] > distPointsLineA || distIntersecByLineA[1] > distPointsLineA;

			distIntersecByLineB	= Coord2(glm::distance2(intersecPoint, lineB.v1), glm::distance2(intersecPoint, lineB.v2));
			distPointsLineB	= glm::distance2(lineB.v1, lineB.v2);
			intersecOnLineB	= distIntersecByLineB[0] > distPointsLineB || distIntersecByLineB[1] > distPointsLineB;

			if (intersecOnLineA || intersecOnLineB){
				drawLine(lineA, colorB);
			}
			else if (dotPlaneNormal < 0){
				drawLine(Line{ lineA.v1, intersecPoint }, colorC);
				drawLine(Line{ intersecPoint, lineA.v2 }, colorY);
			}else{
				drawLine(Line{ lineA.v1, intersecPoint }, colorY);
				drawLine(Line{ intersecPoint, lineA.v2 }, colorC);
			}
		}
	};
}