#ifndef CONTROLLER_H_INC
#define CONTROLLER_H_INC

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


// From : https://stackoverflow.com/questions/25330716
template<typename T>
class unique_function : public std::function<T>
{
    template<typename Fn, typename En = void>
    struct wrapper;

    // specialization for CopyConstructible Fn
    template<typename Fn>
    struct wrapper<Fn, std::enable_if_t< std::is_copy_constructible<Fn>::value >>
    {
        Fn fn;

        template<typename... Args>
        auto operator()(Args&&... args) { return fn(std::forward<Args>(args)...); }
    };

    // specialization for MoveConstructible-only Fn
    template<typename Fn>
    struct wrapper<Fn, std::enable_if_t< !std::is_copy_constructible<Fn>::value
        && std::is_move_constructible<Fn>::value >>
    {
        Fn fn;

        wrapper(Fn&& fn) : fn(std::forward<Fn>(fn)) { }

        wrapper(wrapper&&) = default;
        wrapper& operator=(wrapper&&) = default;

        // these two functions are instantiated by std::function
        // and are never called
        wrapper(const wrapper& rhs) : fn(const_cast<Fn&&>(rhs.fn)) { throw 0; } // hack to initialize fn for non-DefaultContructible types
        wrapper& operator=(wrapper&) { throw 0; }

        template<typename... Args>
        auto operator()(Args&&... args) { return fn(std::forward<Args>(args)...); }
    };

    using base = std::function<T>;

public:
    unique_function() noexcept = default;
    unique_function(std::nullptr_t) noexcept : base(nullptr) { }

    template<typename Fn>
    unique_function(Fn&& f) : base(wrapper<Fn>{ std::forward<Fn>(f) }) { }

    unique_function(unique_function&&) = default;
    unique_function& operator=(unique_function&&) = default;

    unique_function& operator=(std::nullptr_t) { base::operator=(nullptr); return *this; }

    template<typename Fn>
    unique_function& operator=(Fn&& f)
    { base::operator=(wrapper<Fn>{ std::forward<Fn>(f) }); return *this; }

    using base::operator();
};

namespace FFS {

    template <typename T> struct Tag { using type = T; };

    class Controller {
        protected:
            OSSettings settings;
            std::tuple<FFS::Mode> modes;
            unique_function<void(std::any)> func;
            
            
        public:
            
            template<typename ...modules, typename eventTags>
            Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<modules...>&& _modules, eventTags event_tags) : settings{_settings}, modes{_modes} { 
                static_assert(sizeof...(modules) != 0);
                
                // From : https://stackoverflow.com/questions/62652638
                func = [this, event_tags, mods{std::move(_modules)}](std::any any_ev) mutable { // invoked _modules' copy constructor which tries to copy the event handlers and the Tasks inside => fail. 
                                                                        // Switch to generalized lambda capture from https://stackoverflow.com/questions/8640393
                    auto f = [&, this, mods2{std::move(mods)}](auto tag) mutable {
                        using EventType = typename decltype(tag)::type;
                        try {
                            auto ev = std::any_cast<EventType>(any_ev);
                            std::apply([&, this] (auto&... module) {((module.callHandlers(FFS::Event<EventType>{ev, this})), ...);}, mods2);
                        } catch(std::bad_any_cast const& e) {};
                        
                    };
                    std::apply([&f](auto&... tags){ (f(tags), ...); }, event_tags);
                };
            }
            
            template<typename evt_t> 
            void emit (evt_t event) {
                func(event);
            }

            void start() {
                std::cout << "Starting controller" << std::endl;
                OSStart();
                auto start = std::clock();
                while(true) {
                    auto duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

                    if(duration > 30) {
                        std::cout << "End of controller loop" << std::endl;
                        return;
                    }
                }
            }
            
    };


};

#endif
