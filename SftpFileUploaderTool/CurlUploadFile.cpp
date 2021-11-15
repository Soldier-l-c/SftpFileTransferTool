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

	//�������ϴ����ļ�
	FILE* pSendFile = nullptr;
	struct stat file_info;
	curl_off_t iFileSize;

	if (stat(m_strFromFile.c_str(), &file_info))
	{
		return MYRESULT::OPEN_FILE_ERROR;
	}

	iFileSize = (curl_off_t)file_info.st_size;   //��ȡ�ļ���С

	totalSize = iFileSize; //�ļ��ܴ�С
	hasUpedSize = 0;

	fopen_s(&pSendFile, m_strFromFile.c_str(), "rb"); //���ļ�

	if (nullptr == pSendFile)
	{
		return MYRESULT::OPEN_FILE_ERROR;
	}

	CURL* curl = curl_easy_init();
	if (curl) 
	{
		curl_easy_setopt(curl, CURLOPT_URL, (m_strAddress + "/" + m_strToFile).c_str());		//����sftp��·��
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());	//�����û���������	
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);//���ûص�����
		curl_easy_setopt(curl, CURLOPT_READDATA, pSendFile);		//����Ҫ�ϴ��ļ���ָ��
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1); //ѡ���Ƿ���Զ���ļ��в�����ʱ������1Ϊ����
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);                 //�ϴ�
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, iFileSize);//�����ļ���С
		curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, 120);

		//��ȡcurl��ִ�н��
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

	//�ر��ļ�
	fclose(pSendFile);
	curl_easy_cleanup(curl);

	return myres;
}
