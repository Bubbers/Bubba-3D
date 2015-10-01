#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef _WIN32
#include <chrono>
using namespace chrono;
#endif

using namespace std;

#ifdef __linux__
#include <sys/time.h>
#endif

namespace utils{
  class Timer {
  public:
    void start() {
#ifdef __linux__
      gettimeofday(&tStart, NULL);
#endif
#ifdef _WIN32
	  tStart = high_resolution_clock::now();
#endif
    }

    void stop() {
#ifdef __linux__
      gettimeofday(&tEnd, NULL);
#endif

#ifdef _WIN32
	  tEnd = high_resolution_clock::now();
#endif
    }

    double getElapsedTime() {
#ifdef __linux__
      elapsedTime += (tEnd.tv_sec -  tStart.tv_sec) * 1000.0;
      elapsedTime += (tEnd.tv_usec - tStart.tv_usec) /1000.0;
#endif
#ifdef _WIN32
	  duration<double> time_span = duration_cast<duration<double>>(tEnd - tStart);
	  elapsedTime = time_span.count() * 1000;
#endif
	  return elapsedTime;
    }

  private:

#ifdef __linux__
    struct timeval tStart;
    struct timeval tEnd;
#endif
#ifdef _WIN32

	high_resolution_clock::time_point tEnd;
	high_resolution_clock::time_point tStart;
#endif
    double elapsedTime;
  };
  
}
#endif //__TIMER_H_
