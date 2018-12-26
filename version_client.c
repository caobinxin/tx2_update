#include "version_client.h"

void query_env_val(int client_fd ,char* env_val);

void version_client(int client_fd)
{
    char* env_val = ENV_VAL ;
    query_env_val(client_fd, env_val) ;
}

void query_env_val(int client_fd ,char* env_val)
{
    char version_buf[128] = {0} ;
    char* local_version = getenv(env_val) ;
    if( NULL == local_version)
    {
        snprintf(version_buf, 128, "%s\n",DEFAULT_TX2_VERSION) ;
    }else{
        snprintf(version_buf, 128, "%s\n",local_version) ;
    }
    printf_run(version_buf) ;
    cmd_respond(client_fd, version_buf) ;
    printf_run("end query_env_val\n") ;
}