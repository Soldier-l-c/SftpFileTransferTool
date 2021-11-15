#include "stdafx.h"
#include "./3rd/linenoise/linenoise.h"
#include "CurlUploadFile.h"
#include "CurlDownloadFile.h"
static const std::vector<std::string>examples{ "clear", "user", "password", "exit" , "hostname"};

void completionHook(char const* prefix, linenoiseCompletions* lc) 
{
	size_t i;
	COUT_EMPTY_LINE;
	LOG << prefix << END_OF_LINE;
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
	LOG << "*************************************************************" << END_OF_LINE;
	LOG << "Sftp File Transfer" << END_OF_LINE;
	LOG << "https://www.hkpyy.top" << END_OF_LINE;
	LOG << "*************************************************************" << END_OF_LINE;
	COUT_EMPTY_LINE;
}

void InitLinenoise()
{
	linenoiseInstallWindowChangeHandler();
	linenoiseHistoryLoad(HISTORY_FILE);
	linenoiseSetCompletionCallback(completionHook);
}

void TransferTest(std::shared_ptr<ICurlHandleFile>&& sptr)
{
	sptr->init();
	sptr->run();
}

void DownloadTest()
{
	TransferTest(std::shared_ptr<ICurlHandleFile>(new CurlDownloadFile()));
}

void UploadTest()
{
	TransferTest(std::shared_ptr<ICurlHandleFile>(new CurlUploadFile()));
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
	else if ("/upload" == line)
	{
		UploadTest();
	}
	else if ("/download" == line)
	{
		DownloadTest();
	}
	return;
}

void Start()
{
	char* l = nullptr;
	while ((l = linenoise("\x1b[1;32msftp_uploader\x1b[0m> ")) != NULL) 
	{
		if (l[0] != '\0' && l[0] != '/') 
		{
			printf("echo: '%s'\n", l);
			LOG << "echo:" << l << END_OF_LINE;
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