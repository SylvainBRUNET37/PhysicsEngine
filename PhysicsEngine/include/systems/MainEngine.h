#ifndef MAIN_ENGINE_H
#define MAIN_ENGINE_H

#include <vector>
#include <functional>
#include <initializer_list>

class MainEngine
{
public:
	using MainLoopCallback = std::function<bool()>;

	explicit MainEngine(const std::initializer_list<MainLoopCallback> callbacks)
		: mainLoopCallbacks{callbacks}
	{
	}

	void Run() const
	{
		auto allCallbacksSucceeded = [&]
		{
			return std::ranges::all_of(mainLoopCallbacks, [](auto& callback)
			{
				return callback();
			});
		};

		while (allCallbacksSucceeded())
		{
			// TODO
		}
	}

private:
	std::vector<MainLoopCallback> mainLoopCallbacks;
};

#endif
