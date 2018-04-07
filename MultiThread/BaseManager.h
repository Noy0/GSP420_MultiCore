#ifndef _BASEMANAGER_H
#define _BASEMANAGER_H

#include <vector>
#include <utility>
using std::vector;
using std::pair;

template <class T>
class BaseManager
{
public:
	BaseManager(){}
	~BaseManager(){}
	virtual void Begin(){}
	virtual void Update(float dt){}
	virtual void Clear() = 0;
	virtual int AddItem(int type, int *id) = 0;
	virtual bool RemoveItem(int id) = 0;
	virtual bool GetItem(int *id, T& dest)
	{
		T* temp = mGetItem(*id);
		if(temp)
		{
			dest = *temp;
			return true;
		}
		return false;
	}
	virtual void GetCopy(vector<pair<int, T>> &dest){dest = m_Items;}
protected:
	T* mGetItem(int id)
	{
		for(int i = 0; i < m_Items.size(); ++i)
		{
			if(m_Items[i].first == id)
			{
				return &(m_Items[i].second);
			}
		}
		return 0;
	}

	vector<pair<int, T>> m_Items;
	int m_NextID;
};

#endif