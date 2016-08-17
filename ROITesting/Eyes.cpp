#include "Eyes.h"
#include "stdafx.h"

Eyes::Eyes(Rect a, int b, Scalar c){
	rect = a;
	partnerId = b;
	colour = c;
};

Eyes::Eyes(Rect a, int b){
	rect = a;
	partnerId = b;
};

Eyes::Eyes(Rect a){
	rect = a;
};

Point Eyes::center(){
	return Point((rect.x + rect.x + rect.width) / 2, (rect.y + rect.y + rect.height) / 2);
}