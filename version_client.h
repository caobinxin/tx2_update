#ifndef __VERSION_CLIENT__
#define __VERSION_CLIENT__

#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "util.h"

#define ENV_VAL "SLAM_VERSION"
#define DEFAULT_TX2_VERSION "201812010000"

extern void version_client(int client_fd);

#endif
