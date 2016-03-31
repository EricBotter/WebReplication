#ifndef THESIS_LOCKABLE_H
#define THESIS_LOCKABLE_H

#include <mutex>
#include <condition_variable>

using namespace std;

template <typename T>
class Lockable {
private:
	T& object;
	mutex mtx;
	condition_variable cv;

public:
	Lockable(Lockable<T>&) = delete;
	Lockable(Lockable<T>&&) = delete;
	Lockable<T>& operator=(const Lockable<T>&) = delete;
	Lockable<T>& operator=(const Lockable<T>&&) = delete;
	Lockable(T& object) : object(object) {}

	T& getObject() {
		return object;
	}

	mutex& getMutex() {
		return mtx;
	}

	condition_variable& getCV() {
		return cv;
	}
};


#endif //THESIS_LOCKABLE_H
