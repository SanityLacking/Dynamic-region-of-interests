#pragma once

#include "ROIFactory.h"
#include "ROIs.h"

class ROIFactory
{
public:
	// Factory Method
	ROI *createROI(String input, Size s, ObsData obs);
};