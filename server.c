#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <netdb.h>

#define BUF_SIZE 1024

/**
 * 捕获子进程退出信号，在退出时给出提示信息
 */

void sig_handler( int signo)
{
	pid_t pid ;
	int stat ;

	pid = waitpid( -1, &stat, WNOHANG) ;
	while( pid > 0)
	{
		printf("child process terminated (PID:%ld)\n", (long) getpid()) ;
		pid = waitpid( -1, &stat, WNOHANG) ;
	}
	return ;
}

int main(int argc, const char *argv[])
{
	socklen_t clt_addr_len ;
	int listen_fd ;
	int com_fd ;
	int ret ;
	int i ;
	static char recv_buf[BUF_SIZE] ;
	int len ;
	int port ;
	pid_t pid ;

	struct sockaddr_in clt_addr ;
	struct sockaddr_in srv_addr ;

	if( 2 != argc)
	{
		printf("Usage: %s port\n", argv[0]) ;
		return 1 ;
	}

	/***
	 *　获得输入端口
	 */
	port = atoi( argv[1]) ;

	/**
	 * 设置信号处理函数，　也可以使用　sigaction 函数
	 */

	if( signal( SIGCHLD, sig_handler) < 0)
	{
		perror("cannot set the signal.") ;
		return 1 ;
	}

	/**
	 * 创建套接字　用于服务器的监听
	 */
	listen_fd = socket(PF_INET, SOCK_STREAM, 0) ;
	if( listen_fd < 0)
	{
		perror("cannot create listening socket.") ;
		return 1 ;
	}

	memset( &srv_addr, 0, sizeof(srv_addr)) ;
	srv_addr.sin_family = AF_INET ;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;
	srv_addr.sin_port = htons(port) ;

	ret = bind( listen_fd, (struct sockaddr*)&srv_addr, sizeof(srv_addr)) ;
	if( -1 == ret)
	{
		perror("cannot bind server socket.") ;
		close(listen_fd) ;
		return 1 ;
	}

	/**
	 * 监听指定端口，连接5个客户端
	 */
	
	ret = listen(listen_fd, 5) ;
	if( -1 == ret)
	{
		perror("cannot listen the client connect request") ;
		close( listen_fd) ;
		return 1 ;
	}

	/**
	 * 对每个连接的客户端创建一个进程，单独与其进行通信
	 * 首先调用　read　函数读取客户端发送的信息
	 * 将其转换成大写后发送回客户端
	 * 当输入　＠　时，程序退出
	 */

	while(1)
	{
		len = sizeof(clt_addr) ;
		com_fd = accept( listen_fd, (struct sockaddr*)&clt_addr, &len) ;
		if( com_fd < 0)
		{
			if( EINTR == errno)
			{
				continue ;	
			}else{
				perror("cannot accept client connect request") ;
				close( listen_fd) ;
				return 1 ;
			}
		}

		pid = fork() ;
		if( pid < 0)
		{
			perror("cannot create the child process") ;
			close(listen_fd) ;
			return 1 ;
		}else if( 0 == pid){
			while( ( len = read(com_fd, recv_buf, BUF_SIZE)) > 0)
			{
				printf("Message from client(%d): %s\n", len, recv_buf) ;
				
				if( '@' == recv_buf[0]) break ;

				for( i = 0; i < len; i++)
				{
					recv_buf[i] = toupper(recv_buf[i]) ;
				}

				write(com_fd, recv_buf, len) ;

			}

			close( com_fd) ;
			return 0 ;
		}else{
			close(com_fd) ;
		}
	}
	return 0;
}
