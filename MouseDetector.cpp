#include "MouseDetector.h"

std::string MouseDetector::convertInt(int num) {
	std::stringstream ss;
	ss << num;
	return ss.str();
}

void MouseDetector::setMinArea(int minArea) {
	minMarkerArea = minArea;
}

void MouseDetector::setCenterHSV(int H, int S, int V) {
	centerHSV[0] = H;
	centerHSV[1] = S;
	centerHSV[2] = V;
}
void MouseDetector::setLeftHSV(int H, int S, int V) {
	leftHSV[0] = H;
	leftHSV[1] = S;
	leftHSV[2] = V;
}
void MouseDetector::setRightHSV(int H, int S, int V) {
	rightHSV[0] = H;
	rightHSV[1] = S;
	rightHSV[2] = V;
}
void MouseDetector::setScrollHSV(int H, int S, int V) {
	scrollHSV[0] = H;
	scrollHSV[1] = S;
	scrollHSV[2] = V;
}

void MouseDetector::printCenterCoordinates(cv::Mat image, cv::Point centerCoordinates) {
	if (mouseDetected()) {
		cv::Point textOrg(centerCoordinates.x-110,centerCoordinates.y-30);
		calibrateCoordinates(centerCoordinates);
		std::string coordinate = "Mouse Coordinates: (" + convertInt(centerCoordinates.x) + "," + convertInt(centerCoordinates.y) + ")";
		int fontface = cv::FONT_HERSHEY_SIMPLEX;
		double scale = 0.5;
		int thickness = 1;
		cv::putText(image,coordinate,textOrg,fontface,scale,tealColor,thickness);
	}
}

void MouseDetector::printScrollCoordinates(cv::Mat image, cv::Point scrollCoordinates) {
	if (scrollDetected()) {
		cv::Point textOrg(scrollCoordinates.x-60,scrollCoordinates.y-30);
		std::string coordinate = "Scroll Speed: " + convertInt(getScrollSpeed());
		int fontface = cv::FONT_HERSHEY_SIMPLEX;
		double scale = 0.5;
		int thickness = 1;
		cv::putText(image,coordinate,textOrg,fontface,scale,greenColor,thickness);
	}
}

void MouseDetector::setCenterColorThreshold(int threshold) {
	centerThresh = threshold;
	lowerCenter = cv::Scalar(centerHSV[0]-centerThresh,centerHSV[1]-centerThresh,centerHSV[2]-centerThresh);
	upperCenter = cv::Scalar(centerHSV[0]+centerThresh,centerHSV[1]+centerThresh,centerHSV[2]+centerThresh);
}

void MouseDetector::setLeftColorThreshold(int threshold) {
	leftThresh = threshold;
	lowerLeft = cv::Scalar(leftHSV[0]-leftThresh,leftHSV[1]-leftThresh,leftHSV[2]-leftThresh);
	upperLeft = cv::Scalar(leftHSV[0]+leftThresh,leftHSV[1]+leftThresh,leftHSV[2]+leftThresh);
}

void MouseDetector::setRightColorThreshold(int threshold) {
	rightThresh = threshold;
	lowerRight = cv::Scalar(rightHSV[0]-rightThresh,rightHSV[1]-rightThresh,rightHSV[2]-rightThresh);
	upperRight = cv::Scalar(rightHSV[0]+rightThresh,rightHSV[1]+rightThresh,rightHSV[2]+rightThresh);
}

void MouseDetector::setScrollColorThreshold(int threshold) {
	scrollThresh = threshold;
	lowerScroll = cv::Scalar(scrollHSV[0]-scrollThresh,scrollHSV[1]-scrollThresh,scrollHSV[2]-scrollThresh);
	upperScroll = cv::Scalar(scrollHSV[0]+scrollThresh,scrollHSV[1]+scrollThresh,scrollHSV[2]+scrollThresh);
}

void MouseDetector::setScrollAnchorYCoordinate(int y_coord) {
	scrollAnchorYCoordinate = y_coord;
}

bool MouseDetector::mouseDetected() {
	return bMouseDetected;
}

cv::Point MouseDetector::getCenterMarkerCoordinates() {
	return mouseCoordinates;
}

void MouseDetector::processCenterMarker(cv::Mat &image) {
	cv::cvtColor(image,image,CV_BGR2HSV);
	cv::inRange(image,lowerCenter,upperCenter,result);
	cv::findContours(result,contoursCenter,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cv::cvtColor(image,image,CV_HSV2BGR);
	bMouseDetected = false;
	for (itc = contoursCenter.begin(); itc != contoursCenter.end(); ++itc) {
		if (cv::contourArea(*itc) > minMarkerArea) {
			bMouseDetected = true;
			cv::rectangle(image,cv::boundingRect(cv::Mat(*itc)),tealColor,2);
			cv::Moments mom = cv::moments(cv::Mat(*itc));
			mouseCoordinates = cv::Point(int(mom.m10/mom.m00),int(mom.m01/mom.m00));
			cv::circle(image,mouseCoordinates,2,tealColor,2);
			printCenterCoordinates(image,mouseCoordinates);
		}
	}
}

void MouseDetector::processLeftMarker(cv::Mat &image) {
	cv::cvtColor(image,image,CV_BGR2HSV);
	cv::inRange(image,lowerLeft,upperLeft,result);
	cv::findContours(result,contoursLeft,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cv::cvtColor(image,image,CV_HSV2BGR);
	bLeftClickDetected = false;
	if (bMouseDetected) {
		for (itc = contoursLeft.begin(); itc != contoursLeft.end(); ++itc) {
			if (cv::contourArea(*itc) > minMarkerArea) {
				bLeftClickDetected = true;
				cv::rectangle(image,cv::boundingRect(cv::Mat(*itc)),yellowColor,2);
			}
		}
	}
}

void MouseDetector::processRightMarker(cv::Mat &image) {
	cv::cvtColor(image,image,CV_BGR2HSV);
	cv::inRange(image,lowerRight,upperRight,result);
	cv::findContours(result,contoursRight,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cv::cvtColor(image,image,CV_HSV2BGR);
	bRightClickDetected = false;
	if (bMouseDetected && !bLeftClickDetected) {
		for (itc = contoursRight.begin(); itc != contoursRight.end(); ++itc) {
			if (cv::contourArea(*itc) > minMarkerArea) {
				bRightClickDetected = true;
				cv::rectangle(image,cv::boundingRect(cv::Mat(*itc)),yellowColor,2);
			}
		}
	}
}

void MouseDetector::processScrollMarker(cv::Mat &image) {
	cv::cvtColor(image,image,CV_BGR2HSV);
	cv::inRange(image,lowerScroll,upperScroll,result);
	cv::findContours(result,contoursScroll,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
	cv::cvtColor(image,image,CV_HSV2BGR);
	bScrollDetected = false;
	for (itc = contoursScroll.begin(); itc != contoursScroll.end(); ++itc) {
		if (cv::contourArea(*itc) > minMarkerArea) {
			bScrollDetected = true;
			cv::rectangle(image,cv::boundingRect(cv::Mat(*itc)),greenColor,2);
			cv::Moments mom = cv::moments(cv::Mat(*itc));
			scrollCoordinates = cv::Point(int(mom.m10/mom.m00),int(mom.m01/mom.m00));
			cv::circle(image,scrollCoordinates,2,greenColor,2);
			printScrollCoordinates(image,scrollCoordinates);
		}
	}
}

void MouseDetector::calibrateCoordinates(cv::Point &coordinates) {
	coordinates.x = int(1920*coordinates.x/640.0);
	coordinates.y = int(1080*coordinates.y/480.0);
}

void MouseDetector::moveMouse(cv::Point coordinates) {
	SetCursorPos(coordinates.x,coordinates.y);
}

bool MouseDetector::getLeftClickStatus() {
	return bLeftClickDetected;
}

bool MouseDetector::getRightClickStatus() {
	return bRightClickDetected;
}

int MouseDetector::getScrollYCoordinate() {
	return scrollCoordinates.y;
}

int MouseDetector::getScrollAnchorYCoordinate() {
	return scrollAnchorYCoordinate;
}

bool MouseDetector::scrollDetected() {
	return bScrollDetected;
}

int MouseDetector::getScrollSpeed() {
	return (getScrollAnchorYCoordinate() - getScrollYCoordinate())/2;
}

void MouseDetector::leftClickDown(cv::Point coordinates) {
	mouse_event(MOUSEEVENTF_LEFTDOWN,coordinates.x,coordinates.y,0,0);
}

void MouseDetector::leftClickUp(cv::Point coordinates) {
	mouse_event(MOUSEEVENTF_LEFTUP,coordinates.x,coordinates.y,0,0);
}

void MouseDetector::rightClickDown(cv::Point coordinates) {
	mouse_event(MOUSEEVENTF_RIGHTDOWN,coordinates.x,coordinates.y,0,0);
}

void MouseDetector::rightClickUp(cv::Point coordinates) {
	mouse_event(MOUSEEVENTF_RIGHTUP,coordinates.x,coordinates.y,0,0);
}

void MouseDetector::scroll(cv::Point coordinates,int scrollSpeed) {
	mouse_event(MOUSEEVENTF_WHEEL,coordinates.x,coordinates.y,scrollSpeed,0);
}

void MouseDetector::displayMouseStatus(cv::Mat &image) {
	double scale = 0.65;
	int thickness = 2;
	if (mouseDetected()) {
		cv::putText(image,"Mouse Detected",cv::Point(10,30),cv::FONT_HERSHEY_SIMPLEX,scale,tealColor,thickness);
	} else {
		cv::putText(image,"No Mouse Detected",cv::Point(10,30),cv::FONT_HERSHEY_SIMPLEX,scale,tealColor,thickness);
	}
	if (scrollDetected()) {
		if (getScrollSpeed() > 0)
			cv::putText(image,"Scrolling Up",cv::Point(10,70),cv::FONT_HERSHEY_SIMPLEX,scale,greenColor,thickness);
		else
			cv::putText(image,"Scrolling Down",cv::Point(10,70),cv::FONT_HERSHEY_SIMPLEX,scale,greenColor,thickness);
	}
	if (getLeftClickStatus() == true) {
		cv::putText(image,"Left Click",cv::Point(10,70+40*scrollDetected()),cv::FONT_HERSHEY_SIMPLEX,scale,yellowColor,thickness);
	} else if (getRightClickStatus() == true) {
		cv::putText(image,"Right Click",cv::Point(10,70+40*scrollDetected()),cv::FONT_HERSHEY_SIMPLEX,scale,yellowColor,thickness);
	}

}
