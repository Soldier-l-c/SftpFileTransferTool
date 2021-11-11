#pragma once
#include "ICurlHandleFile.h"
class CurlUploadFile :public ICurlHandleFile
{
public:
	virtual ~CurlUploadFile() {}
	virtual void run() override;
protected:
	int32_t __FileUpload();
};