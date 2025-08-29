

//1、socket
//socket(）函数是网络编程中创建套接字的基础函数，它用于创建一个套接字并返回文件描述符，该描述符可以用于后续的网络通信操作。
//函数原型
//int socket(int domain,int type,int protocol);
/*
1.1 domain（协议域/地址族）：domain参数指定了通信使用的协议族(也叫地址族)
①AF_INET：IPv4网络协议，使用IPv4地址（通常用在大多数网络编程中)
②AF_INET6:IPv6 网络协议，使用 IPv6 地址。
③AF_UNIX／ AF_LOCAL：本地通信（即 UNIX 域套接字），用于同一台主机上的进程间通信。
④AF_PACKET：用于原始套接字，直接访问网络层，可以直接处理数据链路层。

1.2. type（套接字类型）：type 参数指定了套接字的类型
①SOCK_STREAM：流式套接字，提供面向连接、可靠的字节流服务（例如TCP) 。
②SOCK_DGRAM：数据报套接字，提供无连接、不可靠的消息传递服务（例如UDP）。
③SOCK_RAW：原始套接字，允许访问底层协议（通常用于定制的网络协议或数据包处理）。

1.3.protocol (协议）：protocol参数指定具体使用的协议。通常可以将此参数设置为0，让系统自动选择与domain和type参数匹配的默认协议。
①对于TCP协议：protocol通常设置为0，系统会自动选择TCP协议(IPPROTO_TCP)
②对于UDP协议：protocol通常也设置为0，系统会自动选择UDP协(IPPROTO_UDP)。
③如果开发者希望显式指定协议，可以使用IPPROTO_TCP（TCP协议）IPPROTO_UDP（UDP协议）等常量。


*/
//void SendData() {
//    //分块发数据
//    size_t total;
//    size_t send_data;
//    const char* msg = "How are you";
//    size_t len = strlen(msg);
//    while (total < len) {
//        send_data = send(fd, msg + total, len - total, 0);
//        total += send_data;
//    }
//
//}

//#include<iostream>
//#include<sys/socket.h>
//#include<netinet/in.h>
//#include<unistd.h>
//#include<cstring>
//
//int main() {
//    int server_fd, new_socket;
//    struct sockaddr_in address;
//    int opt = 1;
//    int addrlen = sizeof(address);
//    const int PORT = 8080;
//    //创建socket
//    server_fd = socket(AF_INET, SOCK_STREAM, 0);
//    if (server_fd == 0) {
//        std::cerr << "socket create failed" << std::endl;
//        return 1;
//    }
//    //设置socket选项，允许地址重用
//    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//    //设置服务器信息
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(PORT);
//    //将socket绑定到指定地址和端口
//    if (bind(server_fd,(struct sockaddr*)&address,sizeof(address)) < 0) {
//        std::cerr << "bind failed\n";
//        return 1;
//    }
//    //开始监听传入的连接请求
//    if (listen(server_fd, 3) < 0) {
//        std::cerr << "Listen failed\n";
//        return 1;
//    }
//    std::cout << "Server is listening on port " << PORT << "..." << std::endl;
//    //无限循环，接受客户链接
//    while (true) {
//        //接受client链接请求
//        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
//        if (new_socket < 0) {
//            std::cerr << "Accepr failed!" << std::endl;
//            continue;
//        }
//        //接受client发送的数据
//        char buffer[1024] = { 0 };
//        //读数据
//        ssize_t bytes_read = recv(new_socket, buffer, sizeof(buffer), 0);
//        if (bytes_read > 0) {
//            std::cout << "Client: " << buffer << std::endl;
//            //发送响应消息给client
//            const char* message = "Hello from server";
//            send(new_socket, message, strlen(message), 0);
//        }
//        //关闭与客户端的链接
//        close(new_socket);
//    }
//    //关闭与服务器的连接
//    close(server_fd);
//    return 0;
//
//}