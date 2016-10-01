/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#ifndef __TIMER_H__
#define __TIMER_H__

#ifdef _WIN32
#include <chrono>

using namespace std::chrono;
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
