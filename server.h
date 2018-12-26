#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
# include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

#include "upload_client.h"
#include "version_client.h"
#include "debug.h"

#define POST 3333

#define WS_ROOT_DIR "/home/colby"

#define FILE_BUF_SIZE 1024
#define CMD_BUF_SIZE 16

#define CLIENT_TYPE_DEFAULT 0
#define CLIENT_TYPE_UPLOAD 1
#define CLIENT_TYPE_VERSION 2

#define END_OF_COMMUNICATION "end_of_communication\n"

extern void handle_child_process(int client_fd);
void sig_handler( int signo);

#endif