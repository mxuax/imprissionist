//
// PointBrush.h
//
// The header file for Point Brush. 
//

#ifndef BLURANDSHARPEN_H
#define BLURANDSHARPEN_H

#include "ImpBrush.h"

class BlurAndSharpenBrush : public ImpBrush
{
public:
	BlurAndSharpenBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif