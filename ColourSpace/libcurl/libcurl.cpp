

/*
THIS IS JUST A CODE DUMP, WILL NOT WORK WITH THE PROJECT AS IS

WILL BE REWORKING THIS INTO THE MAIN CODE BASE

- JAKE THORNE



*/



/*
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>





// callback function writes data to a std::ostream
static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
	if(userp)
	{
		std::ostream& os = *static_cast<std::ostream*>(userp);
		std::streamsize len = size * nmemb;
		if(os.write(static_cast<char*>(buf), len))
			return len;
	}

	return 0;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30)
{
	CURLcode code(CURLE_FAILED_INIT);
	CURL* curl = curl_easy_init();

	if(curl)
	{
		if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
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

void ProcessStringURLS(std::string buffer, std::vector<std::string>& sOut)
{
	for(int i = 0; i < buffer.length(); i++)
	{
		if(int end = buffer.find(".jpg"))
		{
			int start = 0;
			std::string cut;

			end += 4;

			for(int x = end; x > 0; x--)
			{
				if(buffer.at(x) == 'p' &&
				   buffer.at(x-1) == 't' &&
				   buffer.at(x-2) == 't' &&
				   buffer.at(x-3) == 'h')
				{
					start = x-3;
					x = 0;
				}
			}

			//error here
			
			for(int s = start; start < end; s++)
			{
				cut += buffer.at(s);
			}

			sOut.push_back(cut);
			
		}
	}


}



int main()
{
	// Image URL
	std::string url = "http://cpsobsessed.files.wordpress.com/2011/06/isats.jpg";
	std::string query = "games";

	//curl_global_init(CURL_GLOBAL_ALL);

	//std::ofstream ofs("output.jpg", std::ostream::binary);

	//if(CURLE_OK == curl_read(url, ofs))
	//{
	//	// Image successfully written to file
	//}

	//curl_global_cleanup();

	//return 0;

	CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::vector<std::string> urlList;
 
  curl = curl_easy_init();

  std::string weburl = "https://www.google.com/search?q=" + query + "&source=lnms&tbm=isch";

  if(curl) {
	curl_easy_setopt(curl, CURLOPT_URL, weburl.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    
	curl_easy_setopt (curl, CURLOPT_USERAGENT, "Opera/9.80 (J2ME/MIDP; Opera Mini/4.2.14912/870; U; id) Presto/2.4.15");
	curl_easy_setopt (curl, CURLOPT_TRANSFERTEXT, true);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Perform the request, res will get the return code 
    res = curl_easy_perform(curl);
	

	ProcessStringURLS(readBuffer, urlList);

	//std::string source = res;
   //  Check for errors 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

	std::cout << readBuffer << "\n\n";

	for(int i = 0; i < urlList.size(); i++)
	{
		std::cout << urlList[i] << std::endl;
	}
 
    // always cleanup  
    curl_easy_cleanup(curl);
  }

  std::cin.ignore(1000);
  return 0;
}

//$useragent = "Opera/9.80 (J2ME/MIDP; Opera Mini/4.2.14912/870; U; id) Presto/2.4.15";
//$ch = curl_init ("");
//curl_setopt ($ch, CURLOPT_URL, "http://www.google.com/search?hl=en&tbo=d&site=&source=hp&q=".$query);
//curl_setopt ($ch, CURLOPT_USERAGENT, $useragent); // set user agent
//curl_setopt ($ch, CURLOPT_RETURNTRANSFER, true);
//curl_setopt($ch, CURLOPT_FOLLOWLOCATION, TRUE);
//echo $output = curl_exec ($ch);
//curl_close($ch);

//https://www.google.com/search?q=walking&hl=en&source=lnms&tbm=isch&sa=X&ei=mXJtVI_xB9fzapvEgrgN&ved=0CAkQ_AUoAg&biw=1339&bih=996

*/