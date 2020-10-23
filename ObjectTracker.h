#ifndef ObjectTracker_h
#define ObjectTracker_h

#include <map>
#include <set>

class IdCache
{
public:
    IdCache()
    :mIIdCounter(0)
    {
    }

    int GetId();
    void ReleaseId(int aI);
    void Clear();

private:
    std::set<int> mSReturnedIds;
    int mIIdCounter;
};

int IdCache::GetId()
{
    if (mSReturnedIds.empty())
        return ++mIIdCounter;
    else
    {
        int lI = *(mSReturnedIds.begin());
        mSReturnedIds.erase(mSReturnedIds.begin());
        return lI;
    }
}
void IdCache::ReleaseId(int aI)
{
    mSReturnedIds.insert(aI);
}
void IdCache::Clear()
{
    mSReturnedIds.clear();
    mIIdCounter = 0;
}

template <class T>
class ObjectTracker
{
public:
    typedef std::map<int, T*> TIdMap;
    typename T::Args mArgs;

    ObjectTracker(const typename T::Args &aArgs)
    :mArgs(aArgs)
    {
    }

    int NewObj(T *&aT)
    {
        aT = new T(mArgs);
        int lI = mIdCache.GetId();
        mTIdMap.insert(typename TIdMap::value_type(lI, aT));

        return lI;
    }
    T* GetObj(int aId)
    {
        typename TIdMap::iterator lIter = mTIdMap.find(aId);
        if (lIter != mTIdMap.end())
            return lIter->second;
        else
            return 0;
    }
    void ReleaseObj(int aId)
    {
        typename TIdMap::iterator lIter = mTIdMap.find(aId);
        if (lIter != mTIdMap.end())
        {
            T *lT = lIter->second;
            mIdCache.ReleaseId(lIter->first);
            mTIdMap.erase(lIter);
            delete lT;
        }
    }
private:
    TIdMap mTIdMap;
    IdCache mIdCache;
};

#endif
