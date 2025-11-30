#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring> // 添加此头文件以使用 bzero 或 memset
#include <netinet/in.h> // 添加此头文件以定义 htons 和 sockaddr_in
#include <string>
#include <unistd.h> // 添加此头文件以使用 write
#include <errno.h>
int main()
{
    int sfp,bd,at,ln,we,rd,i;
	struct sockaddr_in server_addr,server_addr1;
	socklen_t addrlen;
	char sread[1024];
	char s0[] = "现在你可以发送消息了";
	std::cout << "程序已开始运行";
	sfp = socket(AF_INET, SOCK_STREAM, 0);
	if (sfp == -1) {
		std::cerr << "Socket creation failed!" << std::endl;
		return -1;
	}
	int on = 1;
	setsockopt(sfp, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8082);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bd=bind(sfp, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(bd==-1)
    {
        std::cerr << "Bind failed!" << std::endl;
		std::cout << errno;
        return -1;
    }
	ln = listen(sfp, 5);
	if(ln==-1)
	{
		std::cerr << "Listen failed!" << std::endl;
		return -1;
	}
	addrlen = sizeof(server_addr1);
	at = accept(sfp, (struct sockaddr*)&server_addr1, &addrlen);
	if(at==-1)
	{
		std::cerr << "Accept failed!" << std::endl;
		return -1;
	}
	we = write(at, s0, 1024);
	if(we == -1) {
		std::cerr << "Write failed!" << std::endl;
		return -1;
	}
	while(1) {
		rd = read(at, sread, 1024);
		if (rd == -1) {
			std::cerr << "Read failed!" << std::endl;
			return -1;
		}
		if (rd == 0) {
			std::cout << "Client disconnected." << std::endl;
			break;
		}
		we = write(at, sread, rd);
		if(we==-1) {
			std::cerr << "Write failed!" << std::endl;
			return -1;
		}
	}
	close(at);
	close(sfp);
    return 0;
}