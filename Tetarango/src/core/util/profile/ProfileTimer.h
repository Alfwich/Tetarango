#pragma once

#if PROFILE == 1
#define AW_PROFILE_METHOD() ProfileTimer __timer__(__FUNCSIG__);
#else
#define AW_PROFILE_SECTION(label, x)
#define AW_PROFILE_METHOD()
#endif // _DEBUG


namespace AW
{
	class ProfileTimer
	{
		const char* label;
		long long start, end;
		bool started = true;

		long long getCurrentMicroseconds()
		{
			auto now = std::chrono::system_clock::now();
			auto time = now.time_since_epoch().count();
			return std::chrono::microseconds(time).count();
		}

		void report()
		{
			std::cout << label << " took " << (end - start) / 10000.0 << "(ms)" << std::endl;
		}

	public:
		ProfileTimer() : label(0), start(getCurrentMicroseconds()) {}
		ProfileTimer(const char* label) : label(label), start(getCurrentMicroseconds()) {}
		~ProfileTimer() { stop(); }

		void stop()
		{
			if (started)
			{
				end = getCurrentMicroseconds();
				started = false;

				report();
			}
		}
	};
}
