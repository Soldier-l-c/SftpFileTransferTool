#include "stdafx.h"
#include "./3rd/linenoise/linenoise.h"
#include "CurlUploadFile.h"
static const std::vector<std::string>examples{ "clear", "user", "password", "exit" , "hostname"};

void completionHook(char const* prefix, linenoiseCompletions* lc) 
{
	size_t i;
	std::cout << std::endl;
	std::cout << prefix << std::endl;
	for (const auto& iter: examples)
	{
		if (0  == iter.find(prefix))
		{
			linenoiseAddCompletion(lc, iter.c_str());
			break;
		}
	}
}

void CoutTitle()
{
	std::cout << "*************************************************************" << std::endl;
	std::cout << "Sftp File Uploader" << std::endl;
	std::cout << "https://www.hkpyy.top" << std::endl;
	std::cout << "*************************************************************" << std::endl;
	std::cout << std::endl;
}

void InitLinenoise()
{
	linenoiseInstallWindowChangeHandler();
	linenoiseHistoryLoad(HISTORY_FILE);
	linenoiseSetCompletionCallback(completionHook);
}

void ProcessCmdline(const std::string& line)
{
	if ("/clear" == line)
	{
		linenoiseClearScreen();
	}
	else if("/user" == line)
	{

	}
	else if ("/exit" == line)
	{
		exit(0);
	}
	return;
}

void UploadTest()
{
	auto sptr = std::shared_ptr<ICurlHandleFile>(new CurlUploadFile());
	sptr->init();
	sptr->SetAddress("sftp://www.hkpyy.top");
	sptr->SetUser("user");
	sptr->SetPassword("pwd");
	sptr->SetFromFile("main.cpp");
	sptr->SetToFile("upload_file_test/main.cpp");
	sptr->run();
}

void Start()
{
	char* l = nullptr;
	while ((l = linenoise("\x1b[1;32msftp_uploader\x1b[0m> ")) != NULL) 
	{
		if (l[0] != '\0' && l[0] != '/') 
		{
			printf("echo: '%s'\n", l);
			free(l);
			continue;
		}
		else if (strlen(l) == 0) 
		{
			free(l);
			continue;
		}

		std::string line = l;
		free(l);

		ProcessCmdline(line);

		linenoiseHistoryAdd(line.c_str());
		linenoiseHistorySave(HISTORY_FILE);
	}
	return;
}

int main(int argc, char* argv[])
{
	InitLinenoise();
	CoutTitle();

	Start();

	return 0;
}
CURL* ICurlBase::pcurl = nullptr;