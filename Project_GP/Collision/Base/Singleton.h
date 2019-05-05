#pragma once

namespace Physics
{

	template <class T>
	class Singleton
	{
	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		static std::shared_ptr<T> m_Instance;
		static std::once_flag	  m_OnceFlag;

	public:
		static T Instance()
		{
			std::call_once(Singleton<T>::m_OnceFlag, []()
			{
				Singleton<T>::m_Instance = std::make_shared<T>();
				if (!Singleton<T>::m_Instance)
					throw std::bad_alloc();
			});
			return Singleton<T>::m_Instance;
		}
	};
};