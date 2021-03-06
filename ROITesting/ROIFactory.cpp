#include "ROIFactory.h"
#include "ROIs.h"


ROI *ROIFactory::createROI(String input, Size s, ObsData obs){
	if (input == "control"){
		return new Control(s, obs);
	}
	if (input == "dynamicRecentering"){
		return new DynamicRentering(s, obs);
	}
	else if (input == "static"){
		return new Static(s, obs);
	}
	else if (input == "staticRecentering"){
		return new StaticRentering(s, obs);
	}
	else if (input == "staticExpanding"){
		return new StaticExpanding(s, obs);
	}
	else if (input == "expandingBox"){
		return new ExpandingBox(s, obs);
	}
	else if (input == "blockMatching"){
		return new BlockMatching(s, obs);
	}
	else if (input == "kalmanPredictive"){
		return new KalmanPredictive(s, obs);
	}
	else if (input == "monteCarloPredictive"){
		return new MonteCarloPredictive(s, obs);
	}
	else{//default case
		return new DynamicRentering(s, obs);
	}
};