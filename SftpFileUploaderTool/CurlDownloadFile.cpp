#include "stdafx.h"
#include "CurlDownloadFile.h"
#include "Util.h"
void CurlDownloadFile::run()
{
	if (!__CheckIsReday())return;
	__FileDownload();
}


int32_t CurlDownloadFile::__FileDownload()
{
	
}
