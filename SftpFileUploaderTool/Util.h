#pragma once
namespace UtilBase
{
	namespace UtilTime
	{
		std::string GetFormatTime();
	}

	namespace Single
	{
		bool IsRunning();
	}

	class TimeConsum
	{
	public:
		TimeConsum() { m_nTimeStart = time(nullptr); };
		operator int64_t&& ()
		{
			return time(nullptr) - m_nTimeStart;
		}

	private:
		int64_t m_nTimeStart{ 0 };
	};
}