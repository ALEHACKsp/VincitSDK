#pragma once

/// Thought about wrapping certain WinAPI functions to keep syntax consistancy 
/// also handy when obfuscating these calls :)
namespace wrapper {

	/// Simple wrapper for CreateThread
	inline HANDLE spawn_thread(LPTHREAD_START_ROUTINE routine) {
		return CreateThread(0, 0, routine, 0, 0, 0);
	}

	/// Simple wrapper to show MessageBox
	const inline void show_message(std::string msg) {
		MessageBoxA(nullptr, msg.c_str(), xorstr_("[dbg] gasper"), MB_OK);
	}

	/// Simple wrapper to handle issues
	const inline bool handle_issue(const char* name, void* ptr)
	{
		/// Constant formatting message
		const auto s_formatting = xorstr_("%s not intialized, value -> 0x%X");

		/// Value isn't initialized
		if (!ptr)
		{
			size_t len = std::snprintf(nullptr, 0, s_formatting, name, ptr) + 1; /// Had issues with terminator

			if (len <= 0)
				show_message(xorstr_("Issue formatting issue_handler wrapper::#handle_issue#26"));

			/// Allocate buffer with lenght of message 
			std::unique_ptr<char[]> buffer(new char[len]);

			/// Format the message to buffer
			std::snprintf(buffer.get(), len, s_formatting, name, ptr);

			/// Show the message
			show_message(std::string(buffer.get(), buffer.get() + len - 1));

			/// Show that an issue occured
			return true;
		}

		/// Everything gucci
		return false;
	}

	/// Wrapper to get address of module
	inline void* get_module_handle(const char* name) {
		auto h_module = GetModuleHandleA(name);

		if (handle_issue(name, h_module))
			std::exit(0);

		return h_module;
	}

	/// Wrapper to get procedure address
	inline void* get_proc_address(const char* name, void* handle)
	{
		auto p_procedure = GetProcAddress(reinterpret_cast<HMODULE>(handle), name);

		if (handle_issue(name, p_procedure))
			std::exit(0);

		return p_procedure;
	}

	/// Wrapper for GetAynscKeyState
	inline short get_async_keystate(int key)
	{
		return GetAsyncKeyState(key);
	}

}