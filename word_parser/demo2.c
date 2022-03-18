/* 边缘触发的回声服务端 */
# include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void error_handling(char* buf);
void setNonBlockingMode(int fd);

int main(int argc, char* argv[])
{
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t adr_sz;
    int str_len, i;
    char buf[BUF_SIZE];

    struct epoll_event* ep_events;
    struct epoll_event event;
    int epfd, event_cnt;
        if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if(listen(serv_sock, 5) == -1)  // 准备接受连接，最多接受5个请求
        error_handling("listen() error");

    epfd = epoll_create(EPOLL_SIZE);    // 创建epoll实例
    ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    setNonBlockingMode(serv_sock);
    event.events = EPOLLIN; // EPOLLIN: 检测到有数据需要读取
    event.data.fd = serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);  // 向epoll实例中添加服务器socket，监视事件为EPOLLIN

    while(1)
    {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);    // 等待监视事件发生
        if(event_cnt == -1)
        {
            puts("epoll_wait() error");
            break;
        }

        puts("return epoll wait");
        for(i = 0; i < event_cnt; i++)  // 遍历发生监视事件的每一个socket
        {
            if(ep_events[i].data.fd == serv_sock)   // serv_sock发生事件
            {
                adr_sz =  sizeof(clnt_adr);
                clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                setNonBlockingMode(clnt_sock);
                event.events = EPOLLIN | EPOLLET;   // EPOLLET-边缘触发
                event.data.fd = clnt_sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);  // 向epoll实例中添加和客户端通信的socket，监视事件为EPOLLIN
                printf("connected client: %d\n", clnt_sock);
            }
            else    // clnt_sock发生事件
            {
                while(1)
                {
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if(str_len == 0)
                    {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(ep_events[i].data.fd);
                        printf("closed client: %d\n", ep_events[i].data.fd);
                    }
                    else if(str_len == -1)   // 读取完所有数据
                    {
                        if(errno == EAGAIN)
                            break;
                    }
                    else
                    {
                        write(ep_events[i].data.fd, buf, str_len);  // echo
                    }
                }
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}

void setNonBlockingMode(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void error_handling(char* buf)
{
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}