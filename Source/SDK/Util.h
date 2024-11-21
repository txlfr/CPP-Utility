#pragma once
#include "Global.h"

namespace app 
{
	enum class Convert
	{
		title_case,
		small_case,
		caps
	};
}

namespace app::util
{
	std::filesystem::path get_path();

	uint8_t char_to_byte(char c);
	std::optional<std::uint8_t> char_to_hex(const char c);

	void error_box(std::string error, std::string title = "App");

	std::string convert(const std::string& text, Convert mode);
	std::string hackify(const std::string& text, Convert mode);

	template <typename Func>
	void do_timed(DWORD milliseconds, Func&& callback)
	{
		static_assert(std::is_invocable_v<Func>, "callback must be callable."); // Using std::function for something that can be done in compile time is unnecessary overhead again thanks bomuld :3

		void* ptr = reinterpret_cast<void*>(+std::forward<Func>(callback));
		using Clock = std::chrono::steady_clock;

		static std::map<void*, std::pair<DWORD, Clock::time_point>> timed_functions_map;

		auto& timer_data = timed_functions_map[ptr];
		auto current_time = Clock::now();

		if (timer_data.second == Clock::time_point{}) {
			timer_data = { milliseconds, current_time };
		}
		if (std::chrono::duration_cast<std::chrono::milliseconds>(current_time - timer_data.second).count() >= timer_data.first) {
			callback();
			timer_data.second = current_time;
		}
	}

	uint32_t joaat(const std::string_view& text);

	// Used for formatting stuff    
	// I Have to use inline because of the template 
	template<typename... Args>
	inline std::string format_message(const std::string& message, const Args&... args)
	{
		return std::vformat(message, std::make_format_args(args...));
	} 

	inline void error(const std::string& message, const auto&... args)
	{
		std::string logMessage = format_message(message, args...);
		error_box(logMessage);
	}

	inline std::unordered_map<std::type_index, bool> g_executed_map; 

    #define do_once(block)                     \
    do {                                   \
        static std::once_flag init_flag;   \
        std::call_once(init_flag, block);  \
    } while (0)

	// Ik They are in std::string but sometimes if you are working with const char* :/
	// Thanks bomuld <3 -- Fixed a memory leak
	template <typename... Args>
	std::string combine_strings(Args&&... args) {
		static auto combine = [](const auto&... strs) -> std::string {
			return (std::string(strs) + ...);
	    };
		return combine(std::forward<Args>(args)...);
	}
}
