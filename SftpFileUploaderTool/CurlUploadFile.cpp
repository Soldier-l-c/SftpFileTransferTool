#include "stdafx.h"
#include "CurlUploadFile.h"
#include "Util.h"

void CurlUploadFile::run()
{
	m_strTransType = "Uploader";
	__EnterFileTransfer();
	__CinNeedInfo();

	if (!__CheckIsReday())return;

	COUT_INFO << "Start Upload file:[" << m_strFromFile << "] to: [" << m_strToFile << "]" << END_OF_LINE;

	auto startTime = time(nullptr);
	auto res = __FileUpload();
	auto endTime = time(nullptr);

	COUT_EMPTY_LINE;
	COUT_INFO << "Upload File res:[" << res << "]"<< " Used time:[" << endTime - startTime << "s]" << END_OF_LINE;
}

int64_t hasUpedSize;
int64_t totalSize;
int read_callback(void* ptr, size_t size, size_t nmemb, void* stream)
{
	curl_off_t nread;
	size_t retcode = fread(ptr, size, nmemb, (FILE*)(stream));
	nread = (curl_off_t)retcode;
	hasUpedSize += nread;

	ICurlHandleFile::PrintProgress(hasUpedSize, totalSize);
	return retcode;
};

int32_t CurlUploadFile::__FileUpload()
{
	int32_t myres = 0;

	CURLcode res{ CURLE_OK };
	std::string userPwd = m_strUser + ":" + m_strPassword;

	//打开所需上传的文件
	FILE* pSendFile = nullptr;
	struct stat file_info;
	curl_off_t iFileSize;

	if (stat(m_strFromFile.c_str(), &file_info))
	{
		return MYRESULT::OPEN_FILE_ERROR;
	}

	iFileSize = (curl_off_t)file_info.st_size;   //获取文件大小

	totalSize = iFileSize; //文件总大小
	hasUpedSize = 0;

	fopen_s(&pSendFile, m_strFromFile.c_str(), "rb"); //打开文件

	if (nullptr == pSendFile)
	{
		return MYRESULT::OPEN_FILE_ERROR;
	}

	CURL* curl = curl_easy_init();
	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, (m_strAddress + "/" + m_strToFile).c_str());		//设置sftp的路径
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());	//设置用户名和密码	
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);//设置回调函数
		curl_easy_setopt(curl, CURLOPT_READDATA, pSendFile);		//设置要上传文件的指针
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1); //选择是否在远程文件夹不存在时创建，1为创建
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);                 //上传
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, iFileSize);//设置文件大小
		curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, 120);

		//获取curl的执行结果
		res = curl_easy_perform(curl);
		if (CURLE_OK != res)
		{
			myres = MYRESULT::UPLOAD_FILE_ERROR;
			long dResponseCode = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &dResponseCode);
			COUT_ERROR << "Upload file failed, curl res:[" << res << "]" << END_OF_LINE;
		}
	}
	else
	{
		myres = MYRESULT::UPLOAD_FILE_ERROR;
	}

	//关闭文件
	fclose(pSendFile);
	curl_easy_cleanup(curl);

	return myres;
}
