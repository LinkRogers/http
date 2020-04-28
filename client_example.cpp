#include "httplib.h"
#include <iostream>
#include <fstream> 
#include <unistd.h>

using namespace std;

//print all headers of a req/response
void print_header(const httplib::Headers headers);

int main(void)
{
  // IMPORTANT: 1st parameter must be a hostname or an IP adress string.
  httplib::Client cli("localhost", 1234);

int show=0;
#ifdef show
  httplib::Headers headers = {
  {"Connection", "keep-alive"}, 
  {"Upgrade-Insecure-Requests", "1"},
  {"User-Agent", "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36"},
  {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8"},
  {"Accept-Encoding", "gzip, deflate"},
  {"Accept-Language", "zh-CN,zh;q=0.9"},
  {"Range", "bytes=0-5"}
  };
#endif


  //auto res = cli.Options("/stream", headers);
  auto head_res = cli.Head("/stream");
  print_header(head_res->headers);

  int content_length = atoi((head_res->get_header_value("Content-Length")).c_str());;

  cout << "content_length: " << content_length <<endl <<endl;
  int size_of_package = 40;
  string range;
  int offset = 0;
  auto data = new char[content_length];
  int remain_data = content_length;
  int request_chunk_size;
  //std::vector<string> out;

  while (remain_data > 0){
    if ( remain_data < size_of_package ){
      request_chunk_size = content_length - offset + 1;
    }
    else{
      request_chunk_size = size_of_package; 
    }
    range = "bytes=" + string(to_string(offset)) + string("-") + string(to_string(offset+request_chunk_size-1));

    httplib::Headers headers = {
    {"Connection", "keep-alive"},
    {"Range", range}
    };
    auto res = cli.Get("/stream", headers);
    int res_content_length = atoi((res->get_header_value("Content-Length")).c_str());;
    if (res_content_length == request_chunk_size){
      cout << "finish tansfer this portion of data: " << endl;
      cout << res->body << endl; 
      strcat(data, res->body.c_str());
      cout << "downloaded data: " << endl << data << endl;
    }
    else{
      cout << "recv length: " << res_content_length << endl;
      cout << "recv: " << res->body << endl;
      cout << "restart transfer this portion of data: " << endl;
      
      print_header(res->headers);
      remain_data += request_chunk_size;
      offset -= request_chunk_size;
    }
    remain_data -= request_chunk_size;
    cout << "remain_data: " << remain_data + 1 << endl << endl;
    offset += request_chunk_size;
    sleep(5);
  }
  //auto res = cli.Get("/stream", headers);
  //auto res = cli.Post("/content_receiver", headers,"Makefile", "multipart/form-data");
  /*
  if (res) {
    cout << "res exist" << endl;
    print_header(res->headers);
    cout << res->body << endl; 
  }*/
  cout << "final data: " << endl << data << endl; 
}

void print_header(const httplib::Headers headers){
    for (auto it = headers.begin();it != headers.end();it++){
            cout << it->first << ": ";
            cout << it->second << endl;  
    }
 
}