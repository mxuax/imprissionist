//
// BlurAndSharpenBrush.cpp
//
// The implementation of blur and sharpen brush. It is a kind of ImpBrush. All your brush implementations
// will look like the file with the different GL primitive calls.
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BlurAndSharpenBrush.h"
#include "math.h"
#include <vector>

extern float frand();

BlurAndSharpenBrush::BlurAndSharpenBrush(ImpressionistDoc* pDoc, char* name) :
	ImpBrush(pDoc, name)
{
	printf("PointBrush initialized!\n");
}

void BlurAndSharpenBrush::BrushBegin(const Point source, const Point target)
{
	glPointSize(1);
	BrushMove(source, target);
}

void BlurAndSharpenBrush::BrushMove(const Point source, const Point target)
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

	double resultR = 0;
	double resultG = 0;
	double resultB = 0;

	if (pDoc->m_pBlurOrSharpen) {
		// initialising standard deviation to 1.0
		int radius = pDoc->m_pUI->getBSRadius() * 10;
		int filterWidth = 2 * radius + 1;
		int filterHeight = 2 * radius + 1;
		double sigma = 1.0;
		double r, s = 2.0 * sigma * sigma;

		// sum is for normalization
		double sum = 0.0;

		// Initialize the gKernel
		std::vector<std::vector<double>> GKernel(filterHeight, std::vector<double>(filterWidth));

		// generating raidus x raidus kernel
		for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				r = sqrt(x * x + y * y);
				GKernel[x + radius][y + radius] = (exp(-(r * r) / s)) / (M_PI * s);
				sum += GKernel[x + radius][y + radius];
			}
		}

		// normalising the Kernel
		for (int i = 0; i < 2 * radius + 1; ++i)
			for (int j = 0; j < 2 * radius + 1; ++j)
				GKernel[i][j] /= sum;

		for (int i = -radius; i <= radius; i++) {
			for (int j = -radius; j <= radius; j++) {
				GLubyte* color = pDoc->GetOriginalPixel(Point(source.x + i, source.y + j));
				resultR += ((double)color[0]) * GKernel[i + radius][j + radius];
				resultG += ((double)color[1]) * GKernel[i + radius][j + radius];
				resultB += ((double)color[2]) * GKernel[i + radius][j + radius];
			}
		}

		double m = max(max(resultR, resultG), resultB);
		if (m > 255) {
			resultR = (resultR / m) * 255;
			resultG = (resultG / m) * 255;
			resultB = (resultB / m) * 255;
		}
	}
	else {
		// initialising standard deviation to 1.0
		int radius = pDoc->m_pUI->getBSRadius();
		int filterWidth = 2 * radius + 1;
		int filterHeight = 2 * radius + 1;

		// sum is for normalization
		double sum = 0.0;

		// Initialize the gKernel
		std::vector<std::vector<double>> SKernel(filterHeight, std::vector<double>(filterWidth));

		// generating raidus x raidus kernel
		for (int x = -radius; x <= radius; x++) {
			for (int y = -radius; y <= radius; y++) {
				SKernel[x + radius][y + radius] = -1.0;
			}
		}
		SKernel[radius][radius] = filterWidth * filterHeight;

		for (int i = -radius; i <= radius; i++) {
			for (int j = -radius; j <= radius; j++) {
				GLubyte* color = pDoc->GetOriginalPixel(Point(source.x + i, source.y + j));
				resultR += ((double)color[0]) * SKernel[i + radius][j + radius];
				resultG += ((double)color[1]) * SKernel[i + radius][j + radius];
				resultB += ((double)color[2]) * SKernel[i + radius][j + radius];
			}
		}
	}
	GLubyte color[4] = { resultR, resultG, resultB, (int)(alpha * 255) };

	glBegin(GL_POINTS);

	glColor4ubv(color);
	glVertex2d(target.x, target.y);

	glEnd();
}

void BlurAndSharpenBrush::BrushEnd(const Point source, const Point target)
{
	// do 
}