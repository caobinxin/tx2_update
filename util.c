#include "util.h"

int cmd_respond(int client_fd, char* mm)
{
	if( (0 >= client_fd) || (NULL == mm))
	{
		printf_error(" client_fd error,or mm == NULL not cmd_respond...exit,") ;
		return -1 ;
	}

	write(client_fd, mm, strlen(mm)) ;
	
	printf_run(mm) ;

	return 0 ;
}

int cmd_confirmation_request(int client_fd, char* confirm_cmd)
{
	
	int ret ;
	int len ;
	char buf[32] = {0} ;
	char log_mm[LOG_MM_SIZE] = {0} ;

	if( NULL == confirm_cmd)
	{
		printf_error(" 未指定，服务器期望等待的命令. confirm_cmd == NULL \n") ;
		return -2 ;
	}

	memset(log_mm, 0, sizeof(log_mm)) ;
	snprintf(log_mm, sizeof(log_mm), " 服务器等待客户端返回： %s\n", confirm_cmd) ;
	printf_run(log_mm) ;
	
	len = read(client_fd, buf, 32);

	memset(log_mm, 0, sizeof(log_mm)) ;
	snprintf(log_mm, sizeof(log_mm), " 客户端返回: %s\n", buf) ;
	printf_run(log_mm) ;

	ret = strncmp(confirm_cmd, buf, strlen(confirm_cmd)) ;
	if( 0 == ret){
		printf_run(" 等待命令 确认成功.\n") ;
		return 0 ;
	}else{
		printf_warn(" 客户端的返回指令，不是预期指令，请尽快查明原因.\n") ;
		return -1 ;
	}

}
