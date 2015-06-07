#ifndef _IMAGECRAWL_H_
#define _IMAGECRAWL_H_

#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

#include <vector>

#include <curl/curl.h>

class ImageCrawl
{
public:
	ImageCrawl() {}
	~ImageCrawl(){}

	void RunQuery(std::string);


private:

	void GrabSource(std::string query);
	void ProcessSource();
	void ProcessImages();
	CURLcode GrabImage(const std::string& url, std::ostream& os, long timeout = 30);

private:
	std::string readBuffer;
	std::vector<std::string> sOut;

};

size_t WriteData(void* buf, size_t size, size_t nmemb, void* userp);
size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);

#endif

