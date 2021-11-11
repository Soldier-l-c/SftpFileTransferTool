#pragma once
#include "ICurlBase.h"
class ICurlHandleFile :public ICurlBase
{
public:
	virtual ~ICurlHandleFile(){};
	virtual void SetToFile(const std::string& toFile) { m_strToFile = toFile; };
	virtual void SetFromFile(const std::string& toFile) { m_strFromFile = toFile; };
	virtual void SetUser(const std::string& user) { m_strUser = user; };
	virtual void SetPassword(const std::string& pwd) { m_strPassword = pwd; };
	virtual void SetAddress(const std::string& add) { m_strAddress = add; };

protected:
	virtual bool __CheckIsReday()
	{
		if (pcurl == nullptr)
		{
			std::cout << "pcurl is nullptr" << std::endl;
			return false;
		}
		if (m_strAddress.empty())
		{
			std::cout << "m_strAddress is nullptr" << std::endl;
			return false;
		}
		if (m_strUser.empty())
		{
			std::cout << "m_strUser is nullptr" << std::endl;
			return false;
		}
		if (m_strPassword.empty())
		{
			std::cout << "m_strPassword is nullptr" << std::endl;
			return false;
		}

		if (m_strToFile.empty())
		{
			std::cout << "m_strToFile is nullptr" << std::endl;
			return false;
		}
		if (m_strFromFile.empty())
		{
			std::cout << "m_strFromFile is nullptr" << std::endl;
			return false;
		}
		return true;
	}
protected:
	std::string m_strUser;
	std::string m_strPassword;
	std::string m_strAddress;

	std::string m_strToFile;
	std::string m_strFromFile;
};