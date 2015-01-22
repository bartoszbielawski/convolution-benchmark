#include "timediff.h"
#include <sys/time.h>
#include <stdlib.h>

timediff::timediff():
	_start(get_now()), _last_checkpoint(get_now())
{

}

double timediff::get_now()
{
	struct timeval t;
    gettimeofday(&t,NULL);
    double result = (double)t.tv_sec + ((double)t.tv_usec / 1.0e6);
    return result;
}
			

double timediff::get_diff()
{
	double now = get_now();
	double result = now - _last_checkpoint;
	_last_checkpoint = now;
	return result;

}

double timediff::get_start()
{
	return _start;
}

