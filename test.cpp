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


  auto head_res = cli.Head("/stream");
  print_header(head_res->headers);

  httplib::Headers headers = {
  {"Range", "bytes=0-5"}
  };
  auto res = cli.Get("/stream");
  //auto res = cli.Post("/content_receiver", headers,"Makefile", "multipart/form-data");
  
  if (res) {
    cout << "res exist" << endl;
    print_header(res->headers);
    cout << res->body << endl; 
  }

}

void print_header(const httplib::Headers headers){
    for (auto it = headers.begin();it != headers.end();it++){
            cout << it->first << ": ";
            cout << it->second << endl;  
    }
 
}