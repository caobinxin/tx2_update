#ifndef __UPLOAD_CLIENT_H__
#define __UPLOAD_CLIENT_H__

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "util.h"

#define CMD_UPLOAD "cmd_upload"
#define CMD_VERSION "cmd_version"

#define UPLOAD_ROOT_DIR "./"
#define UPLOAD_SAVE_DIR ""
#define UPLOAD_FILE_NAME "upload.zip"
#define UPLOAD_FILE_BUFF 512

#define SEV_INQUIRY_UPLOAD_FILE_SIZE "inquiry_upload_file_size\n"
#define SEV_READYED_RECEIVE_FILE "sev_readyed_receive_file\n"
#define SEV_UPLOAD_SUCCESS "sev_upload_success\n"

#define CLI_DECOMPRESSION_CMD  "cli_decompression_cmd"

#define DECOMPRESSION_DIR_NAME "myntai"
#define DECOMPRESSION_SH_NAME "test.sh"

extern int upload_client(int client_fd);
int upload(int client_fd, long file_size) ;
long inquiry_upload_file_size(int client_fd);
int decompression_exec_sh(int client_fd);
void decompression();
int fork_child_process_decompression();
int fork_child_process_exec_sh();
int fork_child_process_clean();
int fork_child_process_reboot();
void exe_sh();
void exe_rm();
void exe_reboot();

#endif