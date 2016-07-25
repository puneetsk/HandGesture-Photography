#include "Color.h"



Color::Color()
{
	//set values for default constructor
	setType("null");
	setColour(Scalar(0,0,0));

}

Color::Color(string name){

	setType(name);
	
	if(name=="yellow"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(0,90,140));
		setHSVmax(Scalar(87,217,256));

		//BGR value for Green:
		setColour(Scalar(0,255,0));

	}
	if(name=="blue"){
		//for orange color
		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(97,170,57));
		setHSVmax(Scalar(113,256,182));

		//BGR value for Yellow:
		setColour(Scalar(255,0,0));

	}
	if(name=="red"){

		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(71,131,136));
		setHSVmax(Scalar(227,209,243));

		//BGR value for Red:
		setColour(Scalar(0,0,255));

	}
	if(name=="green"){
		//for green color
		//TODO: use "calibration mode" to find HSV min
		//and HSV max values

		setHSVmin(Scalar(69,90,47));
		setHSVmax(Scalar(97,256,160));

		//BGR value for Red:
		setColour(Scalar(0,0,255));

	}


}

Color::~Color(void)
{
}

int Color::getXPos(){

	return Color::xPos;

}

void Color::setXPos(int x){

	Color::xPos = x;

}

int Color::getYPos(){

	return Color::yPos;

}

void Color::setYPos(int y){

	Color::yPos = y;

}

Scalar Color::getHSVmin(){

	return Color::HSVmin;

}
Scalar Color::getHSVmax(){

	return Color::HSVmax;
}

void Color::setHSVmin(Scalar min){

	Color::HSVmin = min;
}


void Color::setHSVmax(Scalar max){

	Color::HSVmax = max;
}