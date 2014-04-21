#include "MouseDetector.h"
#include <time.h>

cv::Mat image;
MouseDetector MouseGlove;

cv::Point mouseCoordinates;

cv::VideoCapture camera(0);

bool leftClickStatus;
bool rightClickStatus;

clock_t t;

float getTime() {
	return float(clock() - t)/CLOCKS_PER_SEC;
}

void resetTimer() {
	t = clock();
}

void toggleLeftClick() {
	leftClickStatus = MouseGlove.getLeftClickStatus();
	if (leftClickStatus == true) MouseGlove.leftClickDown(mouseCoordinates);
	else MouseGlove.leftClickUp(mouseCoordinates);
	resetTimer();
}

void toggleRightClick() {
	rightClickStatus = MouseGlove.getRightClickStatus();
	if (rightClickStatus == true) MouseGlove.rightClickDown(mouseCoordinates);
	else MouseGlove.rightClickUp(mouseCoordinates);
	resetTimer();
}

int main() {
    if (!camera.isOpened()) return 1;
	int keyCheck = 0;
	resetTimer();
	MouseGlove.setCenterHSV(104,238,203);
	MouseGlove.setLeftHSV(150,150,232);
	MouseGlove.setRightHSV(15,205,246);
	MouseGlove.setScrollHSV(63,144,204);
	MouseGlove.setCenterColorThreshold(50);
	MouseGlove.setLeftColorThreshold(35);
	MouseGlove.setRightColorThreshold(25);
	MouseGlove.setScrollColorThreshold(30);
	MouseGlove.setScrollAnchorYCoordinate(240);
	MouseGlove.setMinArea(100);
	leftClickStatus = false;
	rightClickStatus = false;
    while(cv::waitKey(10) != 13) {
		if (!camera.read(image)) return 1;
		cv::flip(image,image,1);
		MouseGlove.processCenterMarker(image);
		MouseGlove.processLeftMarker(image);
		MouseGlove.processRightMarker(image);
		MouseGlove.processScrollMarker(image);
		if (getTime() > 0.3) {
			mouseCoordinates = MouseGlove.getCenterMarkerCoordinates();
			MouseGlove.calibrateCoordinates(mouseCoordinates);
		}
		if (MouseGlove.mouseDetected()) {
			MouseGlove.moveMouse(mouseCoordinates);
		}
		if (MouseGlove.getLeftClickStatus() != leftClickStatus)
			toggleLeftClick();
		if (MouseGlove.getRightClickStatus() != rightClickStatus)
			toggleRightClick();
		if (MouseGlove.scrollDetected()) {
			MouseGlove.scroll(mouseCoordinates,MouseGlove.getScrollSpeed());
		}
		MouseGlove.displayMouseStatus(image);
		cv::imshow("Mouse Glove",image);
    }
}
