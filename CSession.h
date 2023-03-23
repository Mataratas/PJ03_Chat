#pragma once
#include "CMessage.h"
#include "CUser.h"
#include "tiarray.h"
#include "tiarray.cpp"
#include <fstream>
//-----------------------------------------------------------------------------------------
enum MainMenuItem {
	eSendToUser,
	eSendToAll,
	eNothing,
	eQuit
};

using MMI = MainMenuItem;
//-----------------------------------------------------------------------------------------
class CSession{
public:
	CSession();
	~CSession() noexcept;
	auto start_session() -> bool;
	auto send(const std::string&) -> MMI;
	auto broadcast() -> MMI;

private:
	void init();
	void close();
	void save_user_data() const;
	auto login() -> bool;
	auto show_main_menu() -> MMI;
	auto show_msg_menu(const std::string&) -> MMI;
	void show_user_msg();


	auto is_valid_user(const std::string&) -> bool;
	auto send_msg(const std::string&, const std::string&, const std::string&) -> bool;
	auto get_current_user() -> CUser* const { return current_user; }
	template<class T> auto get_user(T& vn) -> CUser* const {
		CUser* res_ptr = nullptr;
		for (size_t i = 0; i < arr_users.size(); i++) {
			if (arr_users[i]->get_name() == vn) {
				res_ptr = arr_users[i];
				break;
			}
		}
		return res_ptr;
	}

	CUser* current_user{ nullptr };
	CTypeArray<CUser*> arr_users;
	CTypeArray <CMessage*> arr_msg;

	CSession(const CSession&);
	auto operator=(const CSession&)->CSession&;

	const char *user_fn{"users.txt"};
	const char* msg_fn{"msg.txt"};

	bool first_launch{ false };
	bool is_new_user{ false };
};

