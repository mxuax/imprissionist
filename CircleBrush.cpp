//
// CircleBrush.cpp
//
// The implementation of Circle Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlebrush.h"
#include "math.h"


extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("PointBrush::BrushMove  document is NULL\n");
		return;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	double alpha = pDoc->getAlpha();
	int r = pDoc->getSize() * 0.5;

	glBegin(GL_TRIANGLE_STRIP);
		SetColor(source, alpha);
		int num_segments = 360;
		for (int ii = 0; ii < num_segments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
			float x = r * cosf(theta);//calculate the x component 
			float y = r * sinf(theta);//calculate the y component 
			glVertex2f(x + target.x, y + target.y);//output vertex 
			glVertex2f(target.x, target.y);
		}
		glVertex2f(target.x + r, target.y);
	glEnd();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
