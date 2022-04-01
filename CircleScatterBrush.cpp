//
// CircleScatterBrush.cpp
//
// The implementation of Circle Scatter Brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "circlescatterbrush.h"
#include "math.h"


extern float frand();

CircleScatterBrush::CircleScatterBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
}

void CircleScatterBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CircleScatterBrush::BrushMove(const Point source, const Point target)
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
	int num_splotchs = 3;

	int num_segments = 360;
	for (int i = 0; i < num_splotchs; i++) {
		glBegin(GL_TRIANGLE_STRIP);
		double dx = (frand() - 0.5) * r * 2;
		double dy = (frand() - 0.5) * r * 2;
		SetColor(Point(target.x + dx, target.y + dy), alpha);
		for (int ii = 0; ii < num_segments; ii++) {
			float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle 
			float x = r * cosf(theta);//calculate the x component 
			float y = r * sinf(theta);//calculate the y component 
			glVertex2f(x + target.x + dx, y + target.y + dy);//output vertex 
			glVertex2f(target.x + dx, target.y + dy);
		}
		glVertex2f(target.x + r + dx, target.y + dy);
		glEnd();
	}
}

void CircleScatterBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
