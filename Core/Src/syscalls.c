//
// Created by hristo on 1/20/21.
//
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/times.h>
#include "myTime.h"

int _gettimeofday_r(struct _reent *r, struct timeval *ptimeval, void *ptimezone) {
    (void) r;
    (void) ptimezone;

    return getTime(ptimeval);
}

int settimeofday(const struct timeval *tv_p, const struct timezone *tz)
{
    (void) tz;

    return setTime((struct timeval *)tv_p);
}
