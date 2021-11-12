#pragma once
#include "ICurlBase.h"
#include "3rd/linenoise/linenoise.h"
class ICurlHandleFile :public ICurlBase
{
public:
	virtual ~ICurlHandleFile(){};
	virtual void SetToFile(const std::string& toFile) { m_strToFile = toFile; };
	virtual void SetFromFile(const std::string& toFile) { m_strFromFile = toFile; };
	virtual void SetUser(const std::string& user) { m_strUser = user; };
	virtual void SetPassword(const std::string& pwd) { m_strPassword = pwd; };
	virtual void SetAddress(const std::string& add) { m_strAddress = add; };

public:
	static void PrintProgress(const int64_t& curentSize, const int64_t& totalSize)
	{
		static auto lastPercent{ 0 };
		double prograss = (double)(curentSize) / (double)(totalSize);
		auto precent = (int32_t)(100*prograss);
		int32_t i = precent;

		if (lastPercent != 0 && (precent == lastPercent))return;

		std::cout<<"\33[2K\r";

		std::cout << "Prograss:[";
		while (i--)std::cout << ">";
		i = 100 - precent;
		while (i--)std::cout << " ";
		std::cout << "] " << precent << "%100";

		lastPercent = precent;
	}

protected:
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
		if (pcurl == nullptr)
		{
			COUT_ERROR << "pcurl is nullptr" << std::endl;
			return false;
		}
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
		std::cout << "*************************************************************" << std::endl;
		std::cout << "Sftp File Uploader" << std::endl;
		std::cout << "https://www.hkpyy.top" << std::endl;
		std::cout << "*************************************************************" << std::endl;
		std::cout << std::endl;
	}

protected:
	std::string m_strUser;
	std::string m_strPassword;
	std::string m_strAddress;

	std::string m_strToFile;
	std::string m_strFromFile;
	
	std::string m_strTransType{ "TransDefault" };
};