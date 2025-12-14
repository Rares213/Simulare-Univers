#include "TaskManager.hpp"

#include <iostream>
#include <chrono>
#include <algorithm>

using namespace std::chrono_literals;

namespace task
{
	void TaskManager::add_task(size_t id, std::function<void(void)> && async_fun, bool keep_task)
	{
		m_tasks.emplace_back(id, async_fun, keep_task);
	}

	bool TaskManager::check_task_finished(size_t id)
	{
		std::list<Task>::iterator result;

		try
		{
			result = search_task(id);
		}
		catch (const std::invalid_argument& error)
		{
			throw;
		}

		if (result->m_task_finished == false)
		{
			if (result->m_future.wait_for(0s) == std::future_status::ready)
			{
				result->m_task_finished = true;
				result->m_future.get();
			}
		}

		return result->m_task_finished;
	}

	void TaskManager::wait_all_finished() noexcept
	{
		try
		{
			for (auto task_iter = m_tasks.begin(); task_iter != m_tasks.end(); task_iter++)
			{
				if (task_iter->m_future.valid() == true)
				{
					task_iter->m_future.get();
					task_iter->m_task_finished = true;
				}
			}
		}
		catch (const std::future_error& error)
		{
			std::cout << "\nTaskManager expection\nfunction: wait_all_finished\nwhat: " << error.what() << "\n";
		}
	}

	void TaskManager::launch_task(size_t id)
	{
		std::list<Task>::iterator result;

		try
		{
			result = search_task(id);
		}
		catch (const std::invalid_argument& error)
		{
			throw;
		}

		result->m_task_finished = false;
		result->m_future = std::async(std::launch::async, result->m_async_fun);
	}

	std::list<Task>::iterator TaskManager::search_task(size_t id)
	{
		auto result = std::find_if(m_tasks.begin(), m_tasks.end(), [&](const Task& task)
			{
				if (task.m_id == id)
					return true;
				else
					return false;
			});

		if (result == m_tasks.end())
			throw std::invalid_argument("Task doesn't exist.");

		return result;
	}

	void TaskManager::clear()
	{
		m_tasks.clear();
	}


}


