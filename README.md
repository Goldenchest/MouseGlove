WHAT IT IS:
This project uses OpenCV to detect and translate hand gestures into mouse events.

HOW IT WORKS:
The webcam tracks four colored markers on a pair of cotton gloves. The left glove has three markers: a blue marker on the back of the hand, a pink marker on the pointer knuckle, and an orange marker on the middle knuckle. The right glove has a green marker on the back tip of the pointer finger.

The blue marker represents the position of the mouse pointer; moving this marker will move the pointer. The pink and orange markers, which are initially hidden, trigger a left and right click event, respectively, when they are exposed by moving the appropriate knuckle. The green marker triggers a scroll event; moving the marker above the middle of the webcam image scrolls up, and moving the marker below the middle scrolls down. The farther away the green marker is from the middle, the faster the scroll speed becomes.

Video demonstration: https://www.youtube.com/watch?v=nalC-hLLJnY
