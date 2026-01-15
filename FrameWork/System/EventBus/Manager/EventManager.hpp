#pragma once
#include"System/Singleton/Singleton.hpp"
#include"System/EventBus/Connection/EventConnection.hpp"

#include<functional>
#include<cstdint>
#include<any>
#include<unordered_map>
#include<vector>
#include<typeindex>

namespace Engine::System
{
	/// <summary>
	/// イベントの登録、即時発行、遅延発行
	/// </summary>
	class EventManager : public Singleton<EventManager>
	{
		GENERATE_SINGLETON_BODY(EventManager);

		using CallBackID = uint64_t;

		struct Listener 
		{
			CallBackID id;
			std::function<void(const std::any&)> wrapper;
		};

		/// <summary>
		/// イベントの購読解除
		/// </summary>
		/// <param name="TypeIdx"></param>
		/// <param name="ID"></param>
		void Unsubscribe(std::type_index TypeIdx, CallBackID ID);

	public:
		/// <summary>
		/// イベントの購読
		/// </summary>
		/// <typeparam name="E">イベント構造体</typeparam>
		/// <param name="Callback"></param>
		/// <returns></returns>
		template<typename E>
		[[nodiscard]] EventConnection Subscribe(std::function<void(const E&)> Callback);

		/// <summary>
		/// イベントの即時発行
		/// </summary>
		template<typename E>
		void TriggerEvent(const E& Event);

		/// <summary>
		/// 遅延発行イベントの追加
		/// </summary>
		/// <typeparam name="E"></typeparam>
		/// <param name="Event"></param>
		template<typename E>
		void QueueEvent(E Event);

		/// <summary>
		/// 遅延型イベントの処理
		/// フレーム末で処理をします。
		/// </summary>
		void ProcessEvents();
	private:
		/// <summary>
		/// 登録しているイベント
		/// </summary>
		std::unordered_map<std::type_index, std::vector<Listener>> mListeners;

		/// <summary>
		/// フレーム末で発行する遅延型イベント
		/// </summary>
		std::vector<std::function<void()>> mEventQueue;

		/// <summary>
		/// 次の発行ID
		/// </summary>
		CallBackID mNextID = 0;
	};

	/// <summary>
	/// イベントの購読
	/// </summary>
	/// <typeparam name="E">イベント構造体</typeparam>
	/// <param name="Callback"></param>
	/// <returns></returns>
	template<typename E>
	inline EventConnection EventManager::Subscribe(std::function<void(const E&)> Callback)
	{
		CallBackID id = mNextID++;
		auto TypeIdx = std::type_index(typeid(E));

		//	型を隠ぺいして保存
		mListeners[TypeIdx].push_back({ id, [Callback](const std::any& eventData) {
						Callback(std::any_cast<const E&>(eventData));
					}});
		
		return EventConnection([this,TypeIdx,id]()
			{this->Unsubscribe(TypeIdx, id); });
	}

	/// <summary>
	/// イベントの即時発行
	/// </summary>
	template<typename E>
	inline void EventManager::TriggerEvent(const E& Event)
	{
		//	検索
		auto it = mListeners.find(typeid(E));
		if (it == mListeners.end())
		{
			return;
		}

		//	ループ中の削除でクラッシュするのを防ぐためコピー作成
		//	コピーコストはまったく重くない
		auto targets = it->second;
		for(auto& lisener : targets)
		{
			lisener.wrapper(Event);
		}
	}


	/// <summary>
	/// 遅延発行イベントの追加
	/// </summary>
	/// <typeparam name="E"></typeparam>
	/// <param name="Event"></param>
	template<typename E>
	inline void EventManager::QueueEvent(E Event)
	{
		//	保存して後で実行
		mEventQueue.push_back([this, Event = std::move(Event)]() {
			this->TriggerEvent(Event);
		});
	}
}

