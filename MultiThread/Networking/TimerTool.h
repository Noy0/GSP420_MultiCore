#ifndef TIMER_TOOL_H
#define TIMER_TOOL_H

#include <time.h>

//Return values for the Check method.
#define TIME_OFF	0
#define TIM_ON		1
#define TIME_UNDER	2
#define TIME_OVER	3
#define TIME_UP		4

class Timer
{
private:
	clock_t m_CurrentTime;
	clock_t m_TimeStamp;
	clock_t m_TimeLimit;
	int m_Seconds;
	bool m_Started;
public:
	Timer(){m_Started = false;}
	~Timer();

	void Reset()
	{
		m_CurrentTime = NULL;
		m_TimeStamp = NULL;
		m_TimeLimit = NULL;
		m_Seconds = NULL;

		m_Started = false;
	}

	void Start(int pSeconds)
	{
		Reset();
		m_Started = true;
		m_Seconds = pSeconds;
		m_CurrentTime = clock();

		m_TimeStamp = m_CurrentTime + m_Seconds * CLOCKS_PER_SEC;
		m_TimeLimit = m_TimeStamp * 2;
	}

	int Check()
	{
		if(m_Started == true)
		{
			m_CurrentTime = clock();

			if(m_CurrentTime > m_TimeStamp)
			{
				if(m_CurrentTime > m_TimeLimit)
				{
					return TIME_UP;
				}
				else
				{
					return TIME_OVER;
				}
			}
			else
			{
				return TIME_UNDER;
			}
		}
		else
		{
			return TIME_OFF;
		}
	}

	void Stop()
	{
		m_Started = false;
	}
};

#endif //TIMER_TOOL_H