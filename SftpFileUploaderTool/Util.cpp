#include "stdafx.h"
#include "util.h"
#define SINGLE_MUTEX L"Global\\{DE45BDFF-B5D1-4B65-BA78-09EC77CA57A9}"

std::string UtilBase::UtilTime::GetFormatTime()
{
	struct tm local_time {0};
	char str_time[100];
	auto begin_time = time(NULL);
	localtime_s(&local_time, &begin_time);
	strftime(str_time, sizeof(str_time), "%Y-%m-%d %H:%M:%S", &local_time);
	return 	str_time;
}

bool UtilBase::Single::IsRunning()
{
	HANDLE hObject = CreateMutex(nullptr, false, SINGLE_MUTEX);

	if (nullptr == hObject || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		if (nullptr != hObject)CloseHandle(hObject);
		return true;
	}

	return false;
}
