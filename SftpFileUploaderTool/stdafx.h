#pragma once
#include <iostream>
#include <vector>
#include <string>
#include<mutex>
#include <map>


#define HISTORY_FILE "sftp_history.his"

#define LOG std::cout

#define INFO_ 20
#define ERROR_ 35
#define COUT_INFO LOG<<UtilBase::UtilTime::GetFormatTime()<<"  "<<INFO_ << "  "
#define COUT_ERROR LOG<<UtilBase::UtilTime::GetFormatTime()<<"  "<<ERROR_ << "  "
#define COUT_EMPTY_LINE LOG << std::endl;
#define END_OF_LINE	std::endl;

#include "Util.h"
#include "ICurlBase.h"
#include "ICurlHandleFile.h"
#ifdef DEBUG
#pragma comment(lib,"/3rd/curl/lib/libcurld.lib")
#else 
#pragma comment(lib,"./3rd/curl/lib/libcurl.lib")
#pragma comment(lib,"./3rd/lib/libssl.lib")
#pragma comment(lib,"./3rd/lib/libcrypto.lib")
#pragma comment(lib, "./3rd/lib/zlib.lib")
#pragma comment(lib, "./3rd/lib/libssh2.lib")
#endif // DEBUG