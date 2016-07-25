

#include <sstream>
#include <string>
#include <iostream>
#include <vector>

#include "Color.h"


//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
int mx=0;
int my=0;
//int g1=0,g2=0,b1=0,b2=0,p1=0,p2=0,r1=0,r2=0;
String col="",cred="",cyellow="",cblue="",cgreen=""; 
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS=50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 50*50;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed





}
string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}
void createTrackbars(){
	//create window for trackbars


	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}
void drawObject(vector<Color> theColors,Mat &frame){

	for(int i =0; i<theColors.size(); i++){

	cv::circle(frame,cv::Point(theColors.at(i).getXPos(),theColors.at(i).getYPos()),10,cv::Scalar(0,0,255));
	cv::putText(frame,intToString(theColors.at(i).getXPos())+ " , " + intToString(theColors.at(i).getYPos()),cv::Point(theColors.at(i).getXPos(),theColors.at(i).getYPos()+20),1,1,Scalar(0,255,0));
	cv::putText(frame,theColors.at(i).getType(),cv::Point(theColors.at(i).getXPos(),theColors.at(i).getYPos()-30),1,2,theColors.at(i).getColour());
	mx=theColors.at(i).getXPos();
	my=theColors.at(i).getYPos();
	
	
	}
}
void morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);



}
void trackFilteredObject(Mat threshold,Mat HSV, Mat &cameraFeed){

	
	vector <Color> yellows;

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){

					Color yellow;
					
					
					
					yellow.setXPos(moment.m10/area);
				   yellow.setYPos(moment.m01/area);
					yellows.push_back(yellow);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				drawObject(yellows,cameraFeed);}

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}
void trackFilteredObject(Color theColor,Mat threshold,Mat HSV, Mat &cameraFeed,String obj){

	
	vector <Color> yellows;

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){

					Color yellow,blue,red,purple;
					
					yellow.setXPos(moment.m10/area);
					yellow.setYPos(moment.m01/area);

					blue.setXPos(moment.m10/area);
					blue.setYPos(moment.m01/area);
					

					red.setXPos(moment.m10/area);
					red.setYPos(moment.m01/area);

					purple.setXPos(moment.m10/area);
					purple.setYPos(moment.m01/area);

					yellow.setType(theColor.getType());
					yellow.setColour(theColor.getColour());

					yellows.push_back(yellow);

					objectFound = true;

				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				//draw object location on screen
				col = obj;
				drawObject(yellows,cameraFeed);
				
				//putText(cameraFeed,col,Point(0,50),1,2,Scalar(0,0,255),2);
			}
			else
				col ="";

		}else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}
int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = false;
	
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;

	if(calibrationMode){
		//create slider bars for HSV filtering
		createTrackbars();
	}
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while(1){
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		if(calibrationMode==true){
		//if in calibration mode, we track objects based on the HSV slider values.
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		morphOps(threshold);
		imshow(windowName2,threshold);
		trackFilteredObject(threshold,HSV,cameraFeed);
		}else{
		//create some temp Color objects so that
		//we can use their member functions/information
		Color yellow("yellow"), blue("blue"), red("red"),green("green");
		
		//first find yellows
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,yellow.getHSVmin(),yellow.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(yellow,threshold,HSV,cameraFeed,"yellow");
		cyellow=col;
		int y1=mx;
		int y2=my;
		//mx=0;
		//my=0;
		//then blue
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,blue.getHSVmin(),blue.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(blue,threshold,HSV,cameraFeed,"blue");
		cblue=col;
		int b1=mx;
		int b2=my;
		//mx=0;
		//my=0;
		//then red
		
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,red.getHSVmin(),red.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(red,threshold,HSV,cameraFeed,"red");
		cred=col;
		int r1=mx;
		int r2=my;
		//mx=0;
		//my=0;
		
//then green
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
		inRange(HSV,green.getHSVmin(),green.getHSVmax(),threshold);
		morphOps(threshold);
		trackFilteredObject(green,threshold,HSV,cameraFeed,"green");
		cgreen=col;
		int g1=mx;
		int g2=my;
		//mx=0;
		//my=0;
		line(cameraFeed,Point(r1,r2),Point(b1,b2),Scalar(255,0,0),1);
		line(cameraFeed,Point(g1,g2),Point(y1,y2),Scalar(255,0,0),1);
		line(cameraFeed,Point(b1,b2),Point(y1,y2),Scalar(255,0,0),1);
		line(cameraFeed,Point(r1,r2),Point(g1,g2),Scalar(255,0,0),1);
		int h=y1-g1;
		int w=y2-g2;
		imshow(windowName2,threshold);
		if(h>50 && w>50)
		{
		CvCapture* capture = cvCaptureFromCAM(0); 
		IplImage* framein = cvQueryFrame(capture);
		
		cvSetImageROI(framein, cvRect(g1,g2,h,w));
		 IplImage *frameout = cvCreateImage(cvGetSize(framein),  framein->depth, framein->nChannels);
		  /* copy subimage */
        cvCopy(framein, frameout, NULL);
		
        /* always reset the Region of Interest */
        cvResetImageROI(framein);

        cvShowImage("Camera_Output", frameout);  //Show image frames on created window
		 imwrite( "../Gray_Image.jpg",Mat( frameout) );
		imshow("Image", imread("../Gray_Image.jpg"));
		
		}

		}
	
		//show frames 
		
		
		imshow(windowName,cameraFeed);
		



		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	



		}

	return 0;
}