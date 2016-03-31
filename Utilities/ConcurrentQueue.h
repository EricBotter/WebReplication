#ifndef THESIS_CONCURRENTQUEUE_H
#define THESIS_CONCURRENTQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class ConcurrentQueue {
public:
	void push(const T& t) {
		unique_lock<mutex> lock(queueMutex);
		q.push(t);
		queueCV.notify_one();
	}

	T& pop() {
		unique_lock<mutex> lock(queueMutex);
		while (q.empty())
			queueCV.wait(lock);
		T& temp = q.front();
		q.pop();
		return temp;
	}

	T& front() {
		unique_lock<mutex> lock(queueMutex);
		while (q.empty())
			queueCV.wait(lock);
		return q.front();
	}

private:
	queue<T> q;
	mutex queueMutex;
	condition_variable queueCV;
};


#endif //THESIS_CONCURRENTQUEUE_H
