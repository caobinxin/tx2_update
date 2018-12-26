#include "upload_client.h"
#include "debug.h"

int upload_client(int client_fd)
{
	int ret = -1;

	printf_run(" 文件上传客户端登录成功! \n") ;

	long file_size = inquiry_upload_file_size(client_fd) ;
	
	cmd_respond( client_fd,SEV_READYED_RECEIVE_FILE);
	int success = upload(client_fd, file_size) ;
	if( 0 != success){
		printf_error(" 上传失败!!!") ;
		return -1 ;
	}
	printf_run(" 上传成功!!!");	
	cmd_respond(client_fd, SEV_UPLOAD_SUCCESS) ;

	ret = decompression_exec_sh(client_fd) ;

	return ret ;
}

int upload(int client_fd, long file_size)
{
    int fd_w ;
    char save_file_name[128] = {0};
    int num ;
    char buffer[UPLOAD_FILE_BUFF] ;
    long current_size = 0 ;

    strcat(save_file_name, UPLOAD_ROOT_DIR) ;
    strcat(save_file_name, UPLOAD_FILE_NAME) ;
    printf_run("服务器段将要保存的目录:") ;
    printf_run(save_file_name) ;    

    if( ( fd_w = open( save_file_name, O_CREAT | O_WRONLY, 0777)) == -1)
	{
		perror("Cannot create the destination file") ;
		return 1 ;
	}

	while( (num = read( client_fd, buffer, UPLOAD_FILE_BUFF)) > 0)
	{
        current_size += num ;
		if( write( fd_w, buffer, num) == -1)
		{
			perror("Cannot write the fiel content to the file") ;
            close(fd_w);
			return 1 ;
		}

        if( current_size == file_size){
            printf_run("接收完毕！！！") ;
            break;
        }
	}

    close(fd_w) ;
    return 0 ;
}

long inquiry_upload_file_size(int client_fd)
{
	long file_size = 0 ;
	char buf[16] = {0} ;
	int len = 0 ;

	cmd_respond( client_fd,SEV_INQUIRY_UPLOAD_FILE_SIZE);
	len = read(client_fd, buf, 16);
	printf_run("上传的文件大小为：") ;

	file_size = atol(buf) ;
	printf("%ld\n", file_size) ;

	return file_size ;
}


int decompression_exec_sh(int client_fd)
{
	int ret = 0 ;

	ret = cmd_confirmation_request(client_fd, CLI_DECOMPRESSION_CMD) ;
	if( 0 != ret)
	{
		printf_error("处理异常，否则不执行\n") ;
		return -1 ;
	}

	//todo 执行
	ret = fork_child_process_decompression();
	if( 0 != ret)
	{
		printf_error("由于解压失败， 不执行后续脚本\n") ;
		return -1 ;
	}

	ret = fork_child_process_exec_sh() ;
	if( 0 != ret)
	{
		printf_error("执行脚本失败! 程序执行失败\n") ;
		printf_error("向客户端发送 脚本执行失败的返回值\n") ;
		//todo 向客户端发送执行结果的状态码
		return -1 ;
	}

	ret = fork_child_process_clean();
	if( 0 != ret)
	{
		printf_error("清理 压缩包和解压目录! 程序执行失败\n") ;
		printf_error("向客户端发送 脚本执行失败的返回值\n") ;
	}

	// ret = fork_child_process_reboot();

	return ret ;
}

int fork_child_process_decompression()
{
	int ret = 0 ;
	pid_t pid, wait_pid ;
	int status ;

	printf_run(" 开始解压...\n") ;

	pid = fork() ;
	if( -1 == pid)
	{
		perror("cannot create new process decompression:") ;
		return -1 ;
	}else if( pid == 0){
		decompression() ;
	}else{
		wait_pid = waitpid( pid, &status, WUNTRACED | WCONTINUED) ;
		if( -1 == status)
		{
			perror("cannot using waitpid function") ;
			return -1 ;
		}else if( 0 == status){
			printf_run(" 解压成功!!!\n") ;
			return 0 ;
		}else{
			printf_warn(" 解压异常退出!!! 解压失败\n") ;
			return -1 ;
		}
	}

	return ret ;
}

int fork_child_process_exec_sh()
{
	int ret = 0 ;
	pid_t pid, wait_pid ;
	int status ;

	printf_run(" 开始执行解压后，目录中的脚本\n") ;

	pid = fork() ;
	if( -1 == pid)
	{
		perror("cannot create new process decompression:") ;
		return -1 ;
	}else if( pid == 0){
		//todo 执行解压目录中的脚本
		exe_sh() ;

	}else{
		wait_pid = waitpid( pid, &status, WUNTRACED | WCONTINUED) ;
		printf_run("脚本执行结束：返回的值\n") ;
		if( -1 == status)
		{
			perror("cannot using waitpid function") ;
			return -1 ;
		}else if( 0 == status){
			printf_run(" 执行成功!!!\n") ;
			return 0 ;
		}else{
			printf_warn(" 脚本执行失败!!!\n") ;
			return -1 ;
		}
	}

	return ret ;
}

int fork_child_process_clean()
{
	int ret = 0 ;
	pid_t pid, wait_pid ;
	int status ;

	printf_run(" 开始执行清理 工作\n") ;

	pid = fork() ;
	if( -1 == pid)
	{
		perror("cannot create new process :") ;
		return -1 ;
	}else if( pid == 0){
		//todo 执行解压目录中的脚本
		exe_rm() ;

	}else{
		wait_pid = waitpid( pid, &status, WUNTRACED | WCONTINUED) ;
		printf_run("清理结束：返回的值\n") ;
		if( -1 == status)
		{
			perror("cannot using waitpid function") ;
			return -1 ;
		}else if( 0 == status){
			printf_run(" 清理成功!!!\n") ;
			return 0 ;
		}else{
			printf_warn(" 清理失败!!!\n") ;
			return -1 ;
		}
	}

	return ret ;
}

int fork_child_process_reboot()
{
	int ret = 0 ;
	pid_t pid, wait_pid ;
	int status ;

	printf_run(" 开始重启 工作\n") ;

	pid = fork() ;
	if( -1 == pid)
	{
		perror("cannot create new process :") ;
		return -1 ;
	}else if( pid == 0){
		//todo 执行解压目录中的脚本
		exe_reboot() ;

	}else{
		wait_pid = waitpid( pid, &status, WUNTRACED | WCONTINUED) ;
		printf_run("重启\n") ;
		if( -1 == status)
		{
			perror("cannot using waitpid function") ;
			return -1 ;
		}else if( 0 == status){
			printf_run(" 清理成功!!!\n") ;
			return 0 ;
		}else{
			printf_warn(" 清理失败!!!\n") ;
			return -1 ;
		}
	}

	return ret ;
}


void decompression()
{
	int ret ;
	char dir_buf[128] = {0} ;

	getcwd(dir_buf, 128) ;

	printf_run(" 当前解压目录：\n") ;
	printf_warn(dir_buf) ;

	execlp("unzip", "unzip", UPLOAD_FILE_NAME, (char *)NULL) ;
}

void exe_sh()
{
	char dir_buf[128] = {0} ;
	getcwd(dir_buf, 128) ;

	char exe_sh_path[128] = {0} ;
	snprintf(exe_sh_path, sizeof(exe_sh_path), "%s/%s/%s", dir_buf,DECOMPRESSION_DIR_NAME, DECOMPRESSION_SH_NAME ) ;
	printf_run(exe_sh_path) ;

	execlp("bash", "bash", exe_sh_path, (char *)NULL) ;
}

void exe_rm()
{
	execlp("rm", "rm","-rf", DECOMPRESSION_DIR_NAME , UPLOAD_FILE_NAME, (char *)NULL) ;
}

void exe_reboot()
{
	execlp("reboot", "reboot",(char *)NULL) ;
}
