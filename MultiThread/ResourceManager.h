#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <utility>
#include <vector>
using std::vector;
using std::pair;

template<class T>

class ResourceManager
{
public:
	ResourceManager(){}
	virtual void AddItem(int key, T item)
	{
		pair<int, T> newitem(key, item);
		m_Element.push_back(newitem);
	}

	virtual void AddItemFromFile(int key, char* filename)
	{
	}

	virtual T GetItem(int key)
	{
		for(unsigned i = 0; i < m_Element.size(); ++i)
		{
			if(key == m_Element[i].first)
				return m_Element[i].second;
		}
		return NULL;
	}

	virtual void Clear()
	{
		for(unsigned i = 0; i < m_Element.size(); ++i)
		{
			delete m_Element[i].second;
		}
		m_Element.clear();
	}

	virtual bool IsAvaliable(int key)
	{
		for(unsigned i = 0; i < m_Element.size(); ++i)
		{
			if(m_Element[i].first == key)
				return true;
		}
		return false;
	}

	virtual ~ResourceManager()
	{
		for(unsigned i = 0; i < m_Element.size(); ++i)
		{
			delete m_Element[i].second;
		}
	}
protected:
	vector<pair<int, T>> m_Element;
};

#endif