#ifndef MOUSEDETECTOR_H
#define MOUSEDETECTOR_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>

class MouseDetector {
private:
	cv::Scalar lowerCenter, upperCenter;
	cv::Scalar lowerLeft, upperLeft;
	cv::Scalar lowerRight, upperRight;
	cv::Scalar lowerScroll, upperScroll;

	std::vector<std::vector<cv::Point> > contoursCenter;
	std::vector<std::vector<cv::Point> > contoursLeft;
	std::vector<std::vector<cv::Point> > contoursRight;
	std::vector<std::vector<cv::Point> > contoursScroll;

	std::vector<std::vector<cv::Point> >::iterator itc;

	cv::Point mouseCoordinates;
	cv::Point scrollCoordinates;

	cv::Scalar centerHSV;
	cv::Scalar leftHSV;
	cv::Scalar rightHSV;
	cv::Scalar scrollHSV;

	cv::Scalar tealColor;
	cv::Scalar greenColor;
	cv::Scalar yellowColor;

	bool bMouseDetected;
	bool bLeftClickDetected;
	bool bRightClickDetected;
	bool bScrollDetected;

	cv::Mat result;
	int minMarkerArea;

	int scrollAnchorYCoordinate;

	int centerThresh;
	int leftThresh;
	int rightThresh;
	int scrollThresh;
public:
	MouseDetector() {
		setCenterColorThreshold(50);
		setLeftColorThreshold(50);
		setRightColorThreshold(50);
		setScrollColorThreshold(50);
		setCenterHSV(0,0,0);
		setLeftHSV(0,0,0);
		setRightHSV(0,0,0);
		setScrollHSV(0,0,0);
		setScrollAnchorYCoordinate(240);
		setMinArea(100);
		mouseCoordinates = cv::Point(0,0);
		scrollCoordinates = cv::Point(0,0);
		bMouseDetected = false;
		bLeftClickDetected = false;
		bRightClickDetected = false;
		bScrollDetected = false;
		tealColor = cv::Scalar(255,255,0);
		greenColor = cv::Scalar(0,230,0);
		yellowColor = cv::Scalar(0,255,255);
	};
	void setMinArea(int minArea);
	void setCenterHSV(int H, int S, int V);
	void setLeftHSV(int H, int S, int V);
	void setRightHSV(int H, int S, int V);
	void setScrollHSV(int H, int S, int V);
	void setCenterColorThreshold(int threshold);
	void setLeftColorThreshold(int threshold);
	void setRightColorThreshold(int threshold);
	void setScrollColorThreshold(int threshold);
	void setScrollAnchorYCoordinate(int y_coord);
	bool mouseDetected();
	cv::Point getCenterMarkerCoordinates();
	void processCenterMarker(cv::Mat &image);
	void processLeftMarker(cv::Mat &image);
	void processRightMarker(cv::Mat &image);
	void processScrollMarker(cv::Mat &image);
	void calibrateCoordinates(cv::Point &coordinates);
	void moveMouse(cv::Point coordinates);
	bool getLeftClickStatus();
	bool getRightClickStatus();
	bool scrollDetected();
	int getScrollSpeed();
	void leftClickDown(cv::Point coordinates);
	void leftClickUp(cv::Point coordinates);
	void rightClickDown(cv::Point coordinates);
	void rightClickUp(cv::Point coordinates);
	void scroll(cv::Point coordinates, int scrollSpeed);
	void displayMouseStatus(cv::Mat &image);
private:
	std::string convertInt(int num);
	void printCenterCoordinates(cv::Mat image, cv::Point coordinates);
	void printScrollCoordinates(cv::Mat image, cv::Point coordinates);
	int getScrollYCoordinate();
	int getScrollAnchorYCoordinate();
};

#endif MOUSEDETECTOR_H
