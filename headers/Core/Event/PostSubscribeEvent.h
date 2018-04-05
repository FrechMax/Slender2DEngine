#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <typeindex>
#include <bitset>
#include <assert.h>

#include "Utility/IdDispatcher.h"

namespace S2D {
	namespace Event {
	
		//////////////////////////////////////////
		// EventType Id Dispatch
		//////////////////////////////////////////

		using EventId = size_t;

		EventId _getIdHelper()
		{
			static EventId id = 0;
			return id++;
		}

		template <typename EventType>
		const EventId _getEventId()
		{
			static EventId eventId = _getIdHelper();
			return eventId;
		}

		//////////////////////////////////////////
		// EventHandler Declaration
		//////////////////////////////////////////
		
		template <typename TEventType>
		using HandlerFunction = std::function<void(TEventType&)>;

		/*
		* Wrapper for a function object that captures functions named "void handle(EventType& e)" from the subject
		*/
		template <typename TEventType>
		class EventHandler
		{

		public:
			EventHandler()
			{
				mHandlerFunction = [](TEventType) {};
				mDebugName = "";
				mId = reinterpret_cast<size_t>(this);

			}

			~EventHandler()
			{

			}

			template <typename Subject>
			void insertHandlerFunction(Subject* subject)
			{
				// define general handle func for the EventType
				size_t storedId = mId;
				mHandlerFunction = [storedId,subject, this](TEventType& e)
				{
					if (storedId == reinterpret_cast<size_t>(this))
					{
						subject->handle(e);
					}
				};

				// store meta info
				mDebugName = "Subscriber - " + std::string(typeid(Subject).name()) + ", Event Type - " + std::string(typeid(TEventType).name());
			}

			void operator() (TEventType& e)
			{
				mHandlerFunction(e);
			}

			size_t getId()
			{
				return mId;
			}

		private:
			size_t mId;
			HandlerFunction<TEventType> mHandlerFunction;

			std::string mDebugName;
		};

		//////////////////////////////////////////
		// Subscriber Declaration
		//////////////////////////////////////////

		template <typename TDerived>
		class Subscriber
		{
			// The first and second entry represent Event Id and Handler Id respectively
			using EventHandlerPair = std::pair<int, int>;

		public:
			Subscriber()
			{
			}

			~Subscriber()
			{
			}

			template <typename TEventType>
			void subscribeFor()
			{
				// initialize handler
				EventHandler<TEventType> handler;
				handler.insertHandlerFunction<TDerived>(static_cast<TDerived*>(this));

				// add to event
				int handlerId = Publisher<TEventType>::addHandler(handler);
				assert(handlerId != NULL);
				
				subscriptions.push_back(EventHandlerPair(_getEventId<TEventType>(), handlerId));
			}

			template <typename TEventType>
			void unsubscribeFor()
			{
				for (EventHandlerPair& handlerPair : subscriptions) 
				{
					if (handlerPair.first == _getEventId<TEventType>()) 
					{
						Publisher<TEventType>::removeHandler(handlerPair.second);
					}
				}
			}

			std::vector<EventHandlerPair> subscriptions;
		};

		//////////////////////////////////////////
		// Publisher Declaration
		//////////////////////////////////////////

		/**
		* Global publisher class for certain types of events. 
		*/
		template <typename TEvent>
		class Publisher
		{
		public:
			using EventHandlerList = std::vector<std::pair<size_t, EventHandler<TEvent>>>;

			~Publisher() {}

			template <typename... EventArgs>
			static void invoke(EventArgs... eventArgs)
			{
				TEvent es = TEvent(eventArgs...);
				Publisher<TEvent>::invoke(es);
			}

			static void invoke(TEvent& event)
			{
				for (auto& eh : mEventHandlerList)
				{
					eh.second(event);
				}
			}

			static int addHandler(EventHandler<TEvent>& handler)
			{
				// get address of handler
				int ptrAddress = reinterpret_cast<size_t>(&handler);

				for (auto& eventHandle : mEventHandlerList)
				{
					if (eventHandle.first == ptrAddress) return NULL;
				}
				mEventHandlerList.push_back(std::make_pair(ptrAddress, handler));

				return ptrAddress;
			}

			static void removeHandler(int handlerId)
			{
				for (auto it = mEventHandlerList.begin(); it != mEventHandlerList.end(); it++)
				{
					if (handlerId == (*it).first)
					{
						mEventHandlerList.erase(it);
					}
				}
			}

			static size_t getSubscriberCount()
			{
				return mEventHandlerList.size();
			}

			static EventId getEventTypeId() { return _getEventId<TEvent>(); }

		private:
			Publisher() {}

			// list of pairs of id and handler
			static EventHandlerList mEventHandlerList;
		};

		template <typename TEvent>
		typename Publisher<TEvent>::EventHandlerList Publisher<TEvent>::mEventHandlerList;
}
}