#ifndef _DYNAMICDATA_H
#define _DYNAMICDATA_H

#include <windows.h>
#include <process.h>
#include <vector>
#include <utility>

using std::pair;
using std::vector;

//Dynamic Data class
//Dynamic data specialized for multithreading use.
//You can only copy and overwrite data, but not directly access the elements.
//Each inherited data class needs to be a singleton and have unique name for mutex.

template<class T>
class DynamicData
{
public:
	DynamicData(LPCSTR MutexName)
	{
		m_hMutex = CreateMutex(0, 0, MutexName);
	}

	virtual ~DynamicData()
	{
		CloseHandle(m_hMutex);
	}

	virtual void Copy(T &dest)
	{
		WaitForSingleObject(m_hMutex, INFINITE);
		dest = m_Data;
		ReleaseMutex(m_hMutex);
	}

	virtual void Overwrite(const T &src)
	{
		WaitForSingleObject(m_hMutex, INFINITE);
		m_Data = src;
		ReleaseMutex(m_hMutex);
	}

protected:
	HANDLE m_hMutex;
	T m_Data;
private:
};

#endif