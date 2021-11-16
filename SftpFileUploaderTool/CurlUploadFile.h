#pragma once
#include "ICurlHandleFile.h"
class CurlUploadFile :public ICurlHandleFile
{
public:
	virtual ~CurlUploadFile() {}
	CurlUploadFile();
protected:
	virtual void __FileTrans() override;
	int32_t __FileUpload();
};