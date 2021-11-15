#pragma once
#include "./3rd/curl/curl.h"
//#include <curl/curl.h>
class ICurlBase
{
public:
	virtual ~ICurlBase() 
	{
	};

	static size_t cb(char* d, size_t n, size_t l, void* p)
	{
		(void)d;
		(void)p;
		return n * l;
	}

	virtual void run() = 0;
	virtual void init() final
	{
		static std::once_flag oc;
		std::call_once(oc, [this]
			{
				//curl init
				curl_global_init(CURL_GLOBAL_ALL);
				COUT_INFO << "curl global init" << END_OF_LINE;
			});
	};

	virtual void global_uninit()
	{
		static std::once_flag oc;
		std::call_once(oc, [this]
			{
				COUT_INFO << "curl cleanup" << END_OF_LINE;
			});
	}
public:
};
