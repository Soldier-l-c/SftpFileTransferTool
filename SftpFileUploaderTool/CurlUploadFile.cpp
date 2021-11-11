#include "stdafx.h"
#include "CurlUploadFile.h"
#include "Util.h"
using MYRESULT = enum
{
	SECCESS = 0,
	OPEN_FILE_ERROR,
	GET_LOCALIP_ERROR,
	WSASTARTUP_ERROR,
	CREATE_PROCESS_ERROR,
	ZIP_FILE_ERROR,
	GET_SFTP_INFO_ERROR,
	UPLOAD_FILE_ERROR,
	INIT_CURL_ERROR,
	CREATE_ZIP_HAND_ERROR
};


void CurlUploadFile::run()
{
	if (!__CheckIsReday())return;
	auto res = __FileUpload();
	COUT_INFO << "Upload File res:[" << res << "]" << std::endl;
}

int64_t hasUpedSize;
int64_t totalSize;
int read_callback (void* ptr, size_t size, size_t nmemb, void* stream)
{
	curl_off_t nread;
	size_t retcode = fread(ptr, size, nmemb, (FILE*)(stream));
	nread = (curl_off_t)retcode;
	hasUpedSize += nread;
	printf("\33[2K\r");
	std::cout << hasUpedSize << "/" << totalSize;
	return retcode;
};

int32_t CurlUploadFile::__FileUpload()
{
	int32_t myres = 0;

	CURL* curl = pcurl;
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

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, (m_strAddress + "/" + m_strToFile).c_str());		//����sftp��·��
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());	//�����û���������	
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);//���ûص�����
		curl_easy_setopt(curl, CURLOPT_READDATA, pSendFile);		//����Ҫ�ϴ��ļ���ָ��
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1); //ѡ���Ƿ���Զ���ļ��в�����ʱ������1Ϊ����
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);                 //�ϴ�
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, iFileSize);//�����ļ���С
		curl_easy_setopt(curl, CURLOPT_FTP_RESPONSE_TIMEOUT, 120);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		//��ȡcurl��ִ�н��
		res = curl_easy_perform(curl);
		if (CURLE_OK != res)
		{
			myres = MYRESULT::UPLOAD_FILE_ERROR;
			long dResponseCode = 0;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &dResponseCode);
			COUT_ERROR << "Upload file failed, curl res:[" << res << "]" << std::endl;
		}
	}
	else
	{
		myres = MYRESULT::UPLOAD_FILE_ERROR;
	}

	//�ر��ļ���curl����
	fclose(pSendFile);
	return myres;
}
