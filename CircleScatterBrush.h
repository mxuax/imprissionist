//
// CircleScatterBrush.h
//
// The header file for Scatter Circle Brush. 
//

#ifndef CIRCLESCATTERBRUSH_H
#define CIRCLESCATTERBRUSH_H

#include "ImpBrush.h"

class CircleScatterBrush : public ImpBrush
{
public:
	CircleScatterBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif
