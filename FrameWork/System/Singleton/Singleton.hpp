#pragma once

#ifdef GENERATE_SINGLETON_BODY
#undef GENERATE_SINGLETON_BODY
#endif // GENERATE_SINGLETON_BODY

#define GENERATE_SINGLETON_BODY(Class)\
friend class Singleton<Class>;\
Class()noexcept = default;\
virtual ~Class()noexcept = default;\
Class(const Class&) = delete;\
Class(Class&&) = delete;\
Class& operator=(const Class&) = delete;\
Class& operator=(Class&&) = delete;\

/// <summary>
/// シングルトンステート
/// </summary>
template<typename T>
class Singleton
{
public:
	/// <summary>
	/// インスタンスの生成
	/// </summary>
	/// <returns>true:成功</returns>
	static bool Create()
	{
		if (mInstance == nullptr)
		{
			mInstance = new T();
			mInstance->OnCreate();
			return true;
		}
		return false;
	}
	
	/// <summary>
	/// インスタンスの破棄
	/// </summary>
	/// <returns>true:成功</returns>
	static void Release()
	{
		if (mInstance != nullptr)
		{
			mInstance->OnDestory();
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	static T* GetInstance()
	{
		return mInstance;
	}
protected:
	Singleton()noexcept = default;
	virtual ~Singleton()noexcept = default;

	virtual void OnCreate() {};
	virtual void OnDestory() {};
private:
	static T* mInstance;

	Singleton(const Singleton&) = delete;
	Singleton(Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(Singleton&&) = delete;
};

template<typename T>
T* Singleton<T>::mInstance = nullptr;