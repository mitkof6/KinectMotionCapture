#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef LARGE_INTEGER timeStamp;

void getCurrentTimeStamp(timeStamp& _time);
timeStamp getCurrentTimeStamp();
double getTimeMili(const timeStamp& start, const timeStamp& end);
double getTimeSecs(const timeStamp& start, const timeStamp& end);

#endif

