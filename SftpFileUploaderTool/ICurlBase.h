#pragma once
#include <curl/curl.h>


class ICurlBase
{
public:
	virtual ~ICurlBase() 
	{
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
				COUT_INFO << "curl easy init" << std::endl;
			});
	};

	virtual void global_uninit()
	{
		static std::once_flag oc;
		std::call_once(oc, [this]
			{
				if (pcurl)
				{
					curl_easy_cleanup(pcurl);
					pcurl = nullptr;
					COUT_INFO << "curl easy cleanup" << std::endl;
				}
			});
	}
public:
	static CURL* pcurl;
};
