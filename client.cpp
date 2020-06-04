#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>

using namespace std;

int get_content_length(char* recv_buf){
    char* pTmp = strstr(recv_buf, "Content-Length:");
    pTmp += strlen("Content-Length: ");
    char* pEnd = strstr(pTmp, "\r\n");
    char buf[128]={0};

    int len = pEnd - pTmp;
    memcpy(buf, pTmp, len);
    int contentLength = atoi(buf);
}
void head_req(string host, char* head);
void get_range_req(string host, string range, char* get);

int main(){
    //创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //向服务器（特定的IP和端口）发起请求
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    serv_addr.sin_port = htons(1234);  //端口
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    char head[1024]= {0};
    char head_buf[1920] = {0};
    std::string s("Host: localhost:1234");
    string rg = "bytes=0-39";
    //get_range_req(s, rg, head);
    head_req(s, head);
    cout << "Send head req: " << endl << head << endl;

		int send_len = send(sock, head, strlen(head), 0);
		if (send_len < 0) {
			cout << "发送失败！" << endl;
      return 0;
		}

		int recv_len = recv(sock, head_buf, 1920, 0);
		if (recv_len < 0) {
			cout << "接受失败！" << endl;
      return 0;
		}
		else {
      cout << "recv_len: " << recv_len << endl;
			cout << "服务端信息:" << endl << head_buf << endl;
		}

    int content_length = get_content_length(head_buf);
    cout << "content_length: " << content_length << endl;

    int size_of_package = 40;
    string range;
    int offset = 0;
    auto data = new char[content_length];
    int remain_data = content_length;
    int request_chunk_size;
    //std::vector<string> out;

    while (remain_data > 0){
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      //向服务器（特定的IP和端口）发起请求
      struct sockaddr_in serv_addr;
      memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
      serv_addr.sin_family = AF_INET;  //使用IPv4地址
      serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
      serv_addr.sin_port = htons(1234);  //端口
      connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
      char recv_buf[1920] = {0};
      if ( remain_data < size_of_package ){
        request_chunk_size = content_length - offset + 1;
      }
      else{
        request_chunk_size = size_of_package; 
      }
      range = "bytes=" + string(to_string(offset)) + string("-") + string(to_string(offset+request_chunk_size-1));
      char get_head[1024]= {0};
      get_range_req(s, range, get_head);

      cout << "get_range: " << endl << get_head << endl;
      
      int send_len = send(sock, get_head, strlen(get_head), 0);
      cout << "send_len :" << send_len << endl;
      if (send_len <= 0) {
        cout << "发送失败！" << endl;
        cout << "send_len :" << send_len << endl;
        return 0;
      }
      int recv_len = recv(sock, recv_buf, 1024, MSG_WAITALL);
      if (recv_len < 0) {
        cout << "接受失败！" << endl;
        return 0;
      }
      cout << "recv no error" << endl;
      cout << "recv length: " << recv_len << endl;
      cout << "recv_buf:" <<  endl << recv_buf << endl;
      content_length = get_content_length(recv_buf);
      cout << "content_length:" << endl << content_length << endl;

      int res_content_length = content_length;
  
      if (res_content_length == request_chunk_size){
        cout << "finish tansfer this portion of data: " << endl;
        cout << recv_buf << endl;
      }
      else{
        //cout << "res content length: " << res_content_length << endl;

        cout << "restart transfer this portion of data: " << endl;
 
        remain_data += request_chunk_size;
        offset -= request_chunk_size;
      }
      remain_data -= request_chunk_size;
      cout << "remain_data: " << remain_data + 1 << endl << endl;
      offset += request_chunk_size;
      close(sock);
      sleep(5);
    }
    cout << "final data: " << endl << data << endl; 
    
    //关闭套接字
    close(sock);
    return 0;
}


void head_req(string host, char* head){
  strcat(head, "HEAD ");
  strcat(head, "/stream");
  strcat(head, " HTTP/1.1\r\n");
  strcat(head, host.c_str());
  strcat(head, "\r\n");
  strcat(head, "Accept: */*\r\n");
  strcat(head, "\r\n");
}
void get_range_req(string host, string range, char* get){

  strcat(get, "GET ");
  strcat(get, "/stream");
  strcat(get, " HTTP/1.1\r\n");
  strcat(get, host.c_str());
  strcat(get, "\r\n");
  strcat(get, "Connection: keep-alive\r\n");
  //strcat(get, "Upgrade-Insecure-Requests: 1\r\n");
  strcat(get, "Accept: */*\r\n");
  string Range = "Range: "+string(range)+"\r\n";
  strcat(get, Range.c_str());
  //strcat(get, "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36\r\n");
  //strcat(get, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n");
  //strcat(get, "Accept-Encoding: gzip, deflate\r\n");
  //strcat(get, "Accept-Language: zh-CN,zh;q=0.9\r\n\r\n");
  strcat(get, "\r\n");
}