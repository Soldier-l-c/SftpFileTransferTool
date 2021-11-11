#pragma once
#include <curl/curl.h>


class ICurlBase
{
public:
	virtual ~ICurlBase() 
	{
		static std::once_flag oc;
		std::call_once(oc, [this]
			{
				if (pcurl)
				{
					pcurl = nullptr;
					curl_easy_cleanup(pcurl);
				}
			});
	};
	virtual void run() = 0;
	virtual void init() final
	{
		static std::once_flag oc;
		std::call_once(oc, [this]
			{
				//curl init
				curl_global_init(CURL_GLOBAL_ALL);
				pcurl = curl_easy_init();
			});
	};
public:
	static CURL* pcurl;
};
