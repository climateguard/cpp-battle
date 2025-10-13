#pragma once

#include "details/PrintFieldVisitor.hpp"
#include <iostream>
#include <typeindex>
#include <unordered_map>

namespace sw
{
	class EventLog
	{
	public:
		template <class TEvent>
		void log(uint64_t tick, TEvent&& event)
		{
			using EventType = std::remove_cv_t<std::remove_reference_t<TEvent>>;
			std::cout << "[" << tick << "] " << EventType::Name << " ";
			PrintFieldVisitor visitor(std::cout);
			const_cast<EventType&>(event).visit(visitor);
			std::cout << std::endl;
		}
	};
}
