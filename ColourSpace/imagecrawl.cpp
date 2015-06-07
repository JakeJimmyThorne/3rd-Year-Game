#include "imagecrawl.h"

// Starts the google image grab process
void ImageCrawl::RunQuery(std::string query)
{
	GrabSource(query);
	ProcessSource();
	ProcessImages();
}

// Runs the initial "search" and grabs the source dump
void ImageCrawl::GrabSource(std::string query)
{
	CURL *curl;
	CURLcode res;
	// <------
	std::vector<std::string> urlList;

	curl = curl_easy_init();

	std::string weburl = "https://www.google.com/search?q=" + query + "&source=lnms&tbm=isch";

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, weburl.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Opera/9.80 (J2ME/MIDP; Opera Mini/4.2.14912/870; U; id) Presto/2.4.15");
		curl_easy_setopt(curl, CURLOPT_TRANSFERTEXT, true);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		// Perform the request, res will get the return code 
		res = curl_easy_perform(curl);

		//ProcessStringURLS(readBuffer, urlList);

		//std::string source = res;
		//  Check for errors 
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

		//std::cout << readBuffer << "\n\n";

		for (int i = 0; i < urlList.size(); i++)
		{
			std::cout << urlList[i] << std::endl;
		}

		// always cleanup  
		curl_easy_cleanup(curl);
	}
}

// Processes the source dump
void ImageCrawl::ProcessSource()
{
	std::string buffer = readBuffer;
	int maxImages = 6;

	for (int i = 0; i < buffer.length(); i++)
	{
		if (buffer.at(i) == '.' &&
			buffer.at(i + 1) == 'j' &&
			buffer.at(i + 2) == 'p' &&
			buffer.at(i + 3) == 'g')
		{
			if (maxImages > 0)
			{
				int start = 0;
				std::string cut;

				int end = i + 4;

				for (int x = end; x > 0; x--)
				{
					if (buffer.at(x) == 'p' &&
						buffer.at(x - 1) == 't' &&
						buffer.at(x - 2) == 't' &&
						buffer.at(x - 3) == 'h')
					{
						start = x - 3;
						x = 0;

						for (int s = start; s < end; s++)
						{
							cut += buffer.at(s);
						}

						sOut.push_back(cut);
					}
				}

				maxImages--;
			}
		}
	}

	for (int i = 0; i < sOut.size(); i++)
	{
		std::cout << sOut[i] << std::endl;
	}
}

// Processes cleaned up source to download all images
void ImageCrawl::ProcessImages()
{
	//std::string url = "http://cpsobsessed.files.wordpress.com/2011/06/isats.jpg";
	std::string query = "games";

	curl_global_init(CURL_GLOBAL_ALL);

	//std::ofstream ofs("output.jpg", std::ostream::binary);

	for (int i = 0; i < sOut.size(); i++)
	{
		if (CURLE_OK == GrabImage(sOut[i], std::ofstream("data/images/" + std::to_string(i) + ".jpg", std::ostream::binary)))
		{
			// Image successfully written to file
		}
	}

	curl_global_cleanup();
}

// Downloads a single image
CURLcode ImageCrawl::GrabImage(const std::string& url, std::ostream& os, long timeout)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if (curl)
	{
		if (CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteData))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
			&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
		{
			code = curl_easy_perform(curl);
		}
		curl_easy_cleanup(curl);
	}

	return code;
}


// callback function writes data to a std::ostream
size_t WriteData(void* buf, size_t size, size_t nmemb, void* userp)
{
	if (userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if (os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
