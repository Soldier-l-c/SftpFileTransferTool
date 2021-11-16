#include "stdafx.h"
#include "CurlDownloadFile.h"
#include "Util.h"

int64_t hasDownedSize;
double totalSized;
size_t CurlDownloadFile::CurlWiteDataToFileCallback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	_WriteDataToFileCbAtom* pParam = (_WriteDataToFileCbAtom*)stream;

	auto getres = curl_easy_getinfo(pParam->pUrl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &totalSized); //获取要下载的文件大小

	hasDownedSize += size * nmemb;
	if (getres == CURLE_OK)ICurlHandleFile::PrintProgress(hasDownedSize, totalSized);

	DWORD dwWrittenBytes = 0;
	WriteFile(pParam->hFile, ptr, size * nmemb, &dwWrittenBytes, NULL);

	return dwWrittenBytes;
}

CurlDownloadFile::CurlDownloadFile()
{
	m_strTransType = "Downloader";
}

void CurlDownloadFile::__FileTrans()
{
	COUT_INFO << "Start Download file:[" << m_strFromFile << "] to: [" << m_strToFile << "]" << END_OF_LINE;

	UtilBase::TimeConsum timec;
	auto res = __FileDownload();

	COUT_EMPTY_LINE;
	COUT_INFO << "Download File res:[" << res << "]" << " Used time:[" << timec << "s]" << END_OF_LINE;
}

int32_t CurlDownloadFile::__FileDownload()
{
	CURLcode res;
	auto urlkey = m_strUser + ":" + m_strPassword; //服务器用户名及密码
	auto ServerIpPath = m_strAddress + "/" + m_strFromFile;

	totalSized = 0;
	hasDownedSize = 0;
	int32_t myres = 0;

	_WriteDataToFileCbAtom atom;
	atom.pThis = this;
	atom.hFile = CreateFileA(m_strToFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, 0, NULL);
	if (atom.hFile == INVALID_HANDLE_VALUE)
	{
		COUT_ERROR << "DownloadToFile, create file failed. hfile is INVALID_HANDLE_VALUE, dest file: [" << m_strToFile << "]" << END_OF_LINE;
		return -1;
	}

	CURL* curl = curl_easy_init();
	atom.pUrl = curl;
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, ServerIpPath.c_str());
		curl_easy_setopt(curl, CURLOPT_USERPWD, urlkey.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWiteDataToFileCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &atom);

#ifndef DISABLE_SSH_AGENT
		curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PASSWORD);
#endif

		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		res = curl_easy_perform(curl);

		if (CURLE_OK != res)
		{
			myres = MYRESULT::UPLOAD_FILE_ERROR;
			long dResponseCode = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &dResponseCode);
			COUT_ERROR << "Download file failed, curl res:[" << res << "]" << END_OF_LINE;
		}

	}

	if (atom.hFile)
		CloseHandle(atom.hFile);

	curl_easy_cleanup(curl);

	return myres;
}
