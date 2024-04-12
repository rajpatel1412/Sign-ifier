#ifndef _CAPTURE_H_
#define CAPTURE_H_


// typedef struct argsList
// {
//     int argc;
//     char **argv;
// } argsList;


void* run_capture(void* args);
// void run_capture(int argc, char **argv);
// void captureThread_init(int argc, char **argv);
void captureThread_init();
void captureThread_cleanup(void);

#endif