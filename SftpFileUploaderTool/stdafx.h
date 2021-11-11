#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<mutex>

#include "ICurlBase.h"
#include "ICurlHandleFile.h"
#define HISTORY_FILE "sftp_history.his"

#define INFO_ 20
#define ERROR_ 35
#define COUT_INFO std::cout<<UtilBase::UtilTime::GetFormatTime()<<"  "<<INFO_ << "  "
#define COUT_ERROR std::cout<<UtilBase::UtilTime::GetFormatTime()<<"  "<<ERROR_ << "  "
#define COUT_EMPTY_LINE std::cout << std::endl;

//#ifdef DEBUG
//#pragma comment(lib,"/3rd/curl/lib/libcurld.lib")
//#else 
//#pragma comment(lib,"./3rd/curl/lib/libcurl.lib")
//#pragma comment(lib,"./3rd/lib/libeay32.lib")
//#pragma comment(lib,"./3rd/lib/ssleay32.lib")
//#endif // DEBUG