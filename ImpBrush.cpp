//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int			ImpBrush::c_nBrushCount	= 0;
ImpBrush**	ImpBrush::c_pBrushes	= NULL;

ImpBrush::ImpBrush(ImpressionistDoc*	pDoc, 
				   char*				name) :
					m_pDoc(pDoc), 
					m_pBrushName(name)
{
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample 
// the color from
//----------------------------------------------------
void ImpBrush::SetColor (const Point source, double alpha = 1.0)
{
	ImpressionistDoc* pDoc = GetDocument();
	double r = pDoc->m_pUI->m_colorChooser->r();
	double g = pDoc->m_pUI->m_colorChooser->g();
	double b = pDoc->m_pUI->m_colorChooser->b();

	GLubyte color[4] = { 0, 0, 0, (int) (alpha * 255)};

	memcpy ( color, pDoc->GetOriginalPixel( source ), 3 );
	color[0] = min(255, color[0] * r);
	color[1] = min(255, color[1] * g);
	color[2] = min(255, color[2] * b);
 
	glColor4ubv( color );
	glFlush();

}

int ImpBrush::GetColorGreyScale(const Point source)
{
	ImpressionistDoc* pDoc = GetDocument();

	auto colorRGB = pDoc->GetOriginalPixel(source);
	int colorGreyScale = colorRGB[0] * 0.299 + colorRGB[1] * 0.587 + colorRGB[2] * 0.144;

	return colorGreyScale;
}

double ImpBrush::ApplyFilter(const Point source, double * filter)
{
	int a = GetColorGreyScale(Point(source.x - 1, source.y - 1));
	int b = GetColorGreyScale(Point(source.x, source.y - 1));
	int c = GetColorGreyScale(Point(source.x + 1, source.y - 1));
	int d = GetColorGreyScale(Point(source.x - 1, source.y));
	int e = GetColorGreyScale(Point(source.x, source.y));
	int f = GetColorGreyScale(Point(source.x + 1, source.y));
	int g = GetColorGreyScale(Point(source.x - 1, source.y + 1));
	int h = GetColorGreyScale(Point(source.x, source.y + 1));
	int i = GetColorGreyScale(Point(source.x + 1, source.y + 1));

	double result = a * filter[0] + b * filter[1] + c * filter[2] +
		d * filter[3] + e * filter[4] + f * filter[5] + g * filter[6] + h * filter[7] + i * filter[8];

	return result;
}

Point ImpBrush::GetGradient(const Point source)
{
	//int dx = GetColorGreyScale(Point(source.x + 1, source.y)) - GetColorGreyScale(source);
	//int dy = GetColorGreyScale(Point(source.x, source.y + 1)) - GetColorGreyScale(source);
	double guassianFilter[9] = { 0.0625, 0.125, 0.0625,
								 0.125, 0.25, 0.125,
								 0.0625, 0.125, 0.0625 };
	double a = ApplyFilter(Point(source.x - 1, source.y - 1), guassianFilter);
	double b = ApplyFilter(Point(source.x, source.y - 1), guassianFilter);
	double c = ApplyFilter(Point(source.x + 1, source.y - 1), guassianFilter);
	double d = ApplyFilter(Point(source.x - 1, source.y), guassianFilter);
	double f = ApplyFilter(Point(source.x + 1, source.y), guassianFilter);
	double g = ApplyFilter(Point(source.x - 1, source.y + 1), guassianFilter);
	double h = ApplyFilter(Point(source.x, source.y - 1), guassianFilter);
	double i = ApplyFilter(Point(source.x + 1, source.y + 1), guassianFilter);

	double gradientX[9] = { 1, 0, -1, 2, 0, -2, 1, 0, -1 };
	double gradientY[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	int dx = a - c + 2 * d - 2 * f + g - i;
	int dy = a + 2 * b + c - g - 2 * h - i;


	return Point(dx, dy);
}



