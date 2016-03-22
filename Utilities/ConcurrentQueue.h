#ifndef THESIS_CONCURRENTQUEUE_H
#define THESIS_CONCURRENTQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class ConcurrentQueue {
public:
	void push(const T&);
	T& pop();
	T& front();

private:
	queue<T> q;
	mutex queueMutex;
	condition_variable queueCV;
};


#endif //THESIS_CONCURRENTQUEUE_H
