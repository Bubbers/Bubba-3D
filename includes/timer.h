#ifndef __TIMER_H__
#define __TIMER_H__

//#include <sys/time.h>

namespace utils{
  class Timer {
  public:
    void start() {
     // gettimeofday(&tStart, NULL);
    }

    void stop() {
    //  gettimeofday(&tEnd, NULL);
    }

    double getElapsedTime() {
      elapsedTime += (tEnd.tv_sec -  tStart.tv_sec) * 1000.0;
      elapsedTime += (tEnd.tv_usec - tStart.tv_usec) /1000.0;
      return elapsedTime;
    }

  private:
    struct timeval tStart;
    struct timeval tEnd;
    double elapsedTime;
  };
  
}
#endif //__TIMER_H_
