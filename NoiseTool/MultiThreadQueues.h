#pragma once
#include <mutex>
#include <queue>

template<typename T>
class GenerateQueue
{
public:
    void Clear()
    {
        std::unique_lock<std::mutex> lock( mMutex );
        mQueue = {};
    }

    T Pop()
    {
        std::unique_lock<std::mutex> lock( mMutex );
        while( mQueue.empty() )
        {
            mCond.wait( lock );
        }
        auto item = mQueue.front();
        mQueue.pop();
        return item;
    }

    size_t Push( const T& item )
    {
        std::unique_lock<std::mutex> lock( mMutex );
        mQueue.push( item );
        size_t size = mQueue.size();
        lock.unlock();
        mCond.notify_one();
        return size;
    }

private:
    std::queue<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mCond;
};

template<typename T>
class CompleteQueue
{
public:
    void SetVersion( uint32_t v )
    {
        std::unique_lock<std::mutex> lock( mMutex );
        mVersion = v;
    }

    uint32_t IncVersion()
    {
        std::unique_lock<std::mutex> lock( mMutex );
        return ++mVersion;
    }

    size_t Count()
    {
        std::unique_lock<std::mutex> lock( mMutex );
        return mQueue.size();
    }

    bool Pop( T& out )
    {
        std::unique_lock<std::mutex> lock( mMutex );
        if( mQueue.empty() )
        {
            return false;
        }
        out = mQueue.front();
        mQueue.pop();
        return true;
    }

    bool Push( const T& item, uint32_t version )
    {
        std::unique_lock<std::mutex> lock( mMutex );
        if( version == mVersion )
        {
            mQueue.push( item );
            return true;
        }
        return false;
    }

private:
    uint32_t mVersion = 0;
    std::queue<T> mQueue;
    std::mutex mMutex;
};
