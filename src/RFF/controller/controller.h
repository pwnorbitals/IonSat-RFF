#pragma once

#include <list>
#include <tuple>
#include <memory>
#include <ctime>
#include <type_traits>
#include <any>

#include "mode/mode.h"
#include "module/module.h"
#include "OS/OS.h"
#include "FFS.h"

#include "unique_function.h"

namespace FFS {

	template <typename T> struct Tag {
		using type = T;
	};

	class Controller {
	protected:
		OSSettings settings;
		std::tuple<FFS::Mode> modes;
		unique_function<void (std::any) > func;


	public:

		template<typename ...modules, typename eventTags>
		Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<modules...>&& _modules, eventTags event_tags) : settings{_settings}, modes{_modes} {

			// From : https://stackoverflow.com/questions/62652638
			func = [this, event_tags, mods{std::move(_modules) }](std::any && any_ev) mutable {      // invoked _modules' copy constructor which tries to copy the event handlers and the Tasks inside => fail.
				auto success = false;

				// Switch to generalized lambda capture from https://stackoverflow.com/questions/8640393
				auto f = [this, &any_ev, &mods](auto tag) -> bool {
					using EventType = typename decltype(tag) ::type;
					try {
						auto ev = std::any_cast<EventType> (any_ev); // TODO : get rid of exceptions, replace with if constexpr any_ev.type()
						auto full_ev = FFS::Event<EventType> {ev, this};
						std::apply([&, this](auto & ... module) {
							((module.callHandlers(full_ev)), ...);
						}, mods);
						return true;
					} catch(std::bad_any_cast const& e) {};

					return false;

				};
				std::apply([&f, &success](auto & ... tags) {
					if((f(tags) || ...)) {
						success = true;
					}
				}, event_tags);

				if(!success) {
					std::cout << "no match :( " << std::endl;
				} else {
					std::cout << "matched" << std::endl;
				}
			};
		}

		template<typename evt_t>
		void emit(evt_t&& event) {
			func(std::move(event));
		}
	};
}
