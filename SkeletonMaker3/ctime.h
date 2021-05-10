#ifndef TIME_H
#define TIME_H

// general includes
#include <vector>

extern double get_cpu_time();

// Timer helper class
class CTimer {
public:
	CTimer() { reset(); }
	void reset() { t0 = get_cpu_time(); }
	double get_elapsed() const { return get_cpu_time() - t0; }
private:
	double t0;	
};

#endif