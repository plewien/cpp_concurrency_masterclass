#include <functional>
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>


namespace
{
	enum class Action
	{
		Clean = 1,
		FullSpeed = 2,
		StopEngines = 3,
		Exit = 100,
	};
}


using WorkTask = std::function<void()>;
class WorkQueue
{
public:
	WorkQueue();
	void addTask(WorkTask task);
	void stop();

private:
	void runInThread();

	std::queue<WorkTask> m_tasks;
	std::thread m_thread;
	std::mutex m_mutex;
	bool m_done = false;
};

WorkQueue::WorkQueue()
	: m_thread(&WorkQueue::runInThread, this)
{}

void WorkQueue::addTask(WorkTask task)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_tasks.push(task);
}

void WorkQueue::runInThread()
{
	WorkTask task; // Enforces copy when taking m_tasks.front()
	bool isEmpty = false;
	while (! m_done)
	{
		using namespace std::chrono_literals;

		// Note: Doing everything with m_tasks here avoids locking the mutex twice per loop.
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			isEmpty = m_tasks.empty();

			if (! isEmpty)
			{
				task = m_tasks.front();
				m_tasks.pop();
			}
		}

		if (isEmpty)
		{
			// Note: A new work request from the captain can be delayed up to two seconds here before being acted upon. 
			// This is especially problematic when the captain calls Action::Exit and the user is kept waiting up to 
			// 2*count(WorkQueue) for all the threads to join.
			// This could be fixed by reimplementing the waiting code with a std::condition_variable.
			std::this_thread::sleep_for(2s);
		}
		else
		{
			task();

			std::this_thread::sleep_for(1s);
		}
	}
}

void WorkQueue::stop()
{
	m_done = true;
	if (m_thread.joinable())
		m_thread.join();
}

void clean()
{
	std::cout << "Cleaning" << std::endl;
}

void full_speed()
{
	std::cout << "Full speed ahead!" << std::endl;
}

void stop_engines()
{
	std::cout << "Stop the engines!" << std::endl;
}

int main()
{
	std::cout << "I am the captain. What is your command?" << std::endl;

	WorkQueue cleanWorkQueue;
	WorkQueue engineWorkQueue;

	int input;
	while (true)
	{
		std::cin >> input;

		switch (Action(input))
		{
			case Action::Clean:
				cleanWorkQueue.addTask(clean);
				break;
			case Action::FullSpeed:
				engineWorkQueue.addTask(full_speed);
				break;
			case Action::StopEngines:
				engineWorkQueue.addTask(stop_engines);
				break;
			case Action::Exit:
				std::cout << "Exiting..." << std::endl;
				cleanWorkQueue.stop();
				engineWorkQueue.stop();
				std::cout << "All done!" << std::endl;
				return 0;
			default:
				std::cout << "Invalid order" << std::endl;
				break;
		}
	}
	return 0;
}
