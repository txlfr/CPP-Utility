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

	void do_timed(DWORD milliseconds, std::function<void()> callback);

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

	template<typename Func>
	inline void do_once(Func&& func) 
	{
		auto id = std::type_index(typeid(func));
		if (!g_executed_map[id])
		{
			func();
			g_executed_map[id] = true;
		}
	}

	// Ik They are in std::string but sometimes if you are working with const char* :/
	template <typename... Args>
	const char* combine_strings(Args&&... args)
	{
		auto combine = [](const auto&... strs) -> std::string
	    {
			return (std::string(strs) + ...); 
		};
		std::string combined = combine(std::forward<Args>(args)...);
		char* result = new char[combined.size() + 1];
		std::strcpy(result, combined.c_str());
		return result;
	}
}
