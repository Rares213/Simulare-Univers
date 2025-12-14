#pragma once

#include <future>
#include <functional>
#include <list>

namespace task
{
	class TaskManager;

	class Task
	{
	public:
		Task(size_t id, std::function<void(void)>& async_fun, bool keep_task)
			:
			m_id(id),
			m_async_fun(async_fun),
			m_keep_task(keep_task)
		{

		}

		friend class TaskManager;

	private:

		size_t m_id;
		std::function<void(void)> m_async_fun;
		std::future<void> m_future;
		bool m_keep_task;
		bool m_task_finished = true;
	};

	class TaskManager
	{
	public:

		TaskManager() {}
		~TaskManager() {}

		void add_task(size_t id, std::function<void(void)>&& async_fun, bool keep_task);

		bool check_task_finished(size_t id);
		void wait_all_finished() noexcept;

		void launch_task(size_t id);

		void clear();

	private:

		std::list<Task> m_tasks;

		std::list<Task>::iterator search_task(size_t id);
	};
}




