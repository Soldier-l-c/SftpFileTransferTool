#pragma once
#include "ICurlHandleFile.h"
class CurlUploadFile :public ICurlHandleFile
{
public:
	virtual ~CurlUploadFile() {}
	CurlUploadFile();
	static int ReadCallback(void* ptr, size_t size, size_t nmemb, void* stream);
protected:
	virtual void __FileTrans() override;
	int32_t __FileUpload();
};