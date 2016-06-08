#include <sys/time.h>

long elapsed(struct timeval start, struct timeval end) {
    long secs = end.tv_sec - start.tv_sec;
    long usecs = end.tv_usec - start.tv_usec;
    return secs * 1000000 + usecs;
}
