#pragma once
#include "ICurlHandleFile.h"

class CurlDownloadFile;

typedef struct _WriteDataToFileCbAtom
{
	_WriteDataToFileCbAtom() : pUrl(0), pThis(0), hFile(0) {}

	CURL* pUrl;
	CurlDownloadFile* pThis;
	HANDLE hFile;
}_WriteDataToFileCbAtom;

typedef struct _WriteDataToBufferCbAtom
{
	_WriteDataToBufferCbAtom() : pUrl(0), pThis(0), pBuffer(0), nTotalSize(0), nCurPos(0) {}

	CURL* pUrl;
	CurlDownloadFile* pThis;
	BYTE* pBuffer;
	int nTotalSize;
	int nCurPos;
}_WriteDataToBufferCbAtom;


class CurlDownloadFile:public ICurlHandleFile
{
public:
	virtual ~CurlDownloadFile() {}
	CurlDownloadFile();
	static size_t CurlWiteDataToFileCallback(void* ptr, size_t size, size_t nmemb, void* stream);
private:
	virtual void __FileTrans() override;
	int32_t __FileDownload();
};