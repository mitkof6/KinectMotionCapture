#include "TimeStamp.h"


void getCurrentTimeStamp(timeStamp& _time)
{
    QueryPerformanceCounter(&_time);
}

timeStamp getCurrentTimeStamp()
{
    timeStamp tmp;
    QueryPerformanceCounter(&tmp);
    return tmp;
}

double getTimeMili(const timeStamp& start, const timeStamp& end)
{
    timeStamp dwFreq;
    QueryPerformanceFrequency(&dwFreq);
    return double(end.QuadPart - start.QuadPart) /
        double(dwFreq.QuadPart) * 1000;
}

double getTimeSecs(const timeStamp& start, const timeStamp& end)
{
    timeStamp dwFreq;
    QueryPerformanceFrequency(&dwFreq);
    return double(end.QuadPart - start.QuadPart) / double(dwFreq.QuadPart);
}
