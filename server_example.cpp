#include "httplib.h"
#include <iostream>
#include <fstream> 

using namespace std;

void print_header(const httplib::Headers headers);

int main(void)
{
	using namespace httplib;

	Server svr;

	svr.Get("/hi", [](const Request& req, Response& res) {
						print_header(req.headers);
					  cout << req.body << endl;
			      res.set_content("Hello World!", "text/plain");
				    });



	const uint64_t DATA_CHUNK_SIZE = 4;
	svr.Get("/stream", [&](const Request &req, Response &res) {
		print_header(req.headers);
		//cout << req.body << endl;
		//auto data = pChars;
		ifstream tspfile("/home/link/Desktop/http/Makefile", ios::binary|ios::ate);
		if (!tspfile.is_open()) 
		{ 
				cout << "未成功打开文件" << endl; 
		} 
		ifstream::pos_type pos = tspfile.tellg();
		int length = pos;
		auto data = new char[length];
		tspfile.seekg(0, ios::beg);
		tspfile.read(data, length);
		tspfile.close();
    //cout << "data: " << data << endl;
		res.set_content_provider(
			length, // Content length
			[data](uint64_t offset, uint64_t length, DataSink &sink) {
				const auto &d = data;
				sink.write(&d[offset], std::min(length, DATA_CHUNK_SIZE));
			},
			[data] { delete data; });
	});

	svr.Post("/post", [&](const Request &req, Response &res) {
		print_header(req.headers);
		auto data = new std::string("abcdefg");

		res.set_content_provider(
			data->size(), // Content length
			[data](uint64_t offset, uint64_t length, DataSink &sink) {
				const auto &d = *data;
				sink.write(&d[offset], std::min(length, DATA_CHUNK_SIZE));
			},
			[data] { delete data; });
	});

	svr.listen("localhost", 1234);
}

void print_header(const httplib::Headers headers){
    for (auto it = headers.begin();it != headers.end();it++){
            cout << it->first << ": ";
            cout << it->second << endl;  
    }
 
}