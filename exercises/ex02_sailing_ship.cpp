#include <iostream>
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

using namespace std::chrono_literals;

void clean()
{
	std::cout << "Cleaning group " << std::this_thread::get_id() << " started" << std::endl;
	std::this_thread::sleep_for(1s);
	std::cout << "Cleaning group " << std::this_thread::get_id() << " finished" << std::endl;
}

void full_speed()
{
	std::cout << "Full speed ahead!" << std::endl;
	std::this_thread::sleep_for(1s);
	std::cout << "Engines at maximum speed." << std::endl;
}

void stop_engines()
{
	std::cout << "Stop the engines!" << std::endl;
	std::this_thread::sleep_for(1s);
	std::cout << "Engines stopped." << std::endl;
}

int main()
{
	std::cout << "I am the captain. What is your command?" << std::endl;

	int input;
	while (true)
	{
		std::cin >> input;

		switch (Action(input))
		{
			case Action::Clean:
				std::thread(clean).detach();
				break;
			case Action::FullSpeed:
				std::thread(full_speed).join();
				break;
			case Action::StopEngines:
				std::thread(stop_engines).join();
				break;
			case Action::Exit:
				return 0;
			default:
				std::cout << "Invalid order" << std::endl;
		}
	}
	return 0;
}
