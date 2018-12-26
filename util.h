#ifndef __UTIL_H__
#define __UTIL_H__

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "debug.h"

#define LOG_MM_SIZE 64

extern int cmd_respond(int client_fd, char* mm);
extern int cmd_confirmation_request(int client_fd, char* confirm_cmd);

#endif