// starting and stopping the webcam capture and streaming
#ifndef _CAPTURE_H_
#define _CAPTURE_H_

// thread for constantly capturing frames and streaming them
void* capture(void* args);

// initialize the webcam capturing and streaming
void capture_init(void);

// turn the camera off and stop streaming
void capture_cleanup(void);

#endif