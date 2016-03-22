#include "ConcurrentQueue.h"

template <typename T>
void ConcurrentQueue::push(const T& t) {
	unique_lock<mutex> lock(queueMutex);
	q.push(t);
	queueCV.notify_one();
}

template <typename T>
T& ConcurrentQueue::pop() {
	unique_lock<mutex> lock(queueMutex);
	while (q.empty())
		queueCV.wait(lock);
	T& temp = q.front();
	q.pop();
	return temp;
}

template <typename T>
T& ConcurrentQueue::front() {
	unique_lock<mutex> lock(queueMutex);
	while (q.empty())
		queueCV.wait(lock);
	return q.front();
}

