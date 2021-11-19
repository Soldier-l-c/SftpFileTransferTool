#pragma once
#include "ICurlBase.h"
#include "3rd/linenoise/linenoise.h"

#include <sstream>
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

class ICurlHandleFile :public ICurlBase
{
public:
	virtual ~ICurlHandleFile(){};
	virtual void SetToFile(const std::string& toFile) { m_strToFile = toFile; };
	virtual void SetFromFile(const std::string& toFile) { m_strFromFile = toFile; };
	virtual void SetUser(const std::string& user) { m_strUser = user; };
	virtual void SetPassword(const std::string& pwd) { m_strPassword = pwd; };
	virtual void SetAddress(const std::string& add) { m_strAddress = add; };

	virtual void run() override
	{
		__EnterFileTransfer();
		__CinNeedInfo();
		if (!__CheckIsReday())return;
		__FileTrans();
	}

public:
	static void PrintProgress(const int64_t& curentSize, const int64_t& totalSize)
	{
		if (totalSize <= 0 || curentSize < 0)return;

		static auto lastPercent{ 0 };
		double prograss = (double)(curentSize) / (double)(totalSize);
		auto precent = (int32_t)(100*prograss);
		int32_t i = precent;

		std::stringstream strstr;

		strstr << "Progress:[";
		while (i-- > 0)strstr << ">";
		i = 100 - precent;
		while (i-- > 0)strstr << " ";
		strstr << "] " << precent << "%100 \r";

		LOG << strstr.str() << std::flush;

		lastPercent = precent;
	}

protected:

	virtual void __FileTrans() = 0;

	virtual void __CinNeedInfo()
	{
		static const char* pre = "\x1b[1;32m";
		static const char* suf = "\x1b[0m>";
		char* l = nullptr;

		std::string hint;
		auto pstrNeedCin = __GetLastNeedCin(hint);
		if (!pstrNeedCin)return;

		while ((l = linenoise((pre + hint + suf).c_str())) != NULL)
		{
			if (strlen(l) == 0)
			{
				free(l);
				continue;
			}
			
			*pstrNeedCin = l;
			free(l);

			pstrNeedCin = __GetLastNeedCin(hint);
			if (!pstrNeedCin)return;
		}
	}

	virtual bool __CheckIsReday()
	{
		if (m_strAddress.empty())
		{
			COUT_ERROR << "m_strAddress is nullptr" << std::endl;
			return false;
		}
		if (m_strUser.empty())
		{
			COUT_ERROR << "m_strUser is nullptr" << std::endl;
			return false;
		}
		if (m_strPassword.empty())
		{
			COUT_ERROR << "m_strPassword is nullptr" << std::endl;
			return false;
		}

		if (m_strToFile.empty())
		{
			COUT_ERROR << "m_strToFile is nullptr" << std::endl;
			return false;
		}
		if (m_strFromFile.empty())
		{
			COUT_ERROR << "m_strFromFile is nullptr" << std::endl;
			return false;
		}
		return true;
	}

	virtual std::string* __GetLastNeedCin(std::string& hint)
	{
		if (m_strAddress.empty())
		{
			hint = "sftp_address";
			return &m_strAddress;
		}
		else if (m_strUser.empty())
		{
			hint = "user_name";
			return &m_strUser;
		}
		else if (m_strPassword.empty())
		{
			hint = "password";
			return &m_strPassword;
		}
		else if (m_strToFile.empty())
		{
			hint = "des_file";
			return &m_strToFile;
		}
		else if (m_strFromFile.empty())
		{
			hint = "src_file";
			return &m_strFromFile;
		}
		return nullptr;
	}

	virtual void __EnterFileTransfer()
	{
		linenoiseClearScreen();
		LOG << "*************************************************************" << END_OF_LINE;
		LOG << "Sftp File "<< m_strTransType << END_OF_LINE;
		LOG << "https://www.hkpyy.top" << END_OF_LINE;
		LOG << "*************************************************************" << END_OF_LINE;
		COUT_EMPTY_LINE;
	}

protected:
	std::string m_strUser;
	std::string m_strPassword;
	std::string m_strAddress;

	std::string m_strToFile;
	std::string m_strFromFile;
	
	std::string m_strTransType{ "TransDefault" };
};