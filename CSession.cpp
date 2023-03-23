#include "CSession.h"
//===================================================================================
CSession::CSession(){
	init();
}
//--------------------------------------------------------------------------------------
CSession::~CSession() {
	//std::cout << "Destructor of session:"<< std::endl;
	close();
}
//--------------------------------------------------------------------------------------
void CSession::init() {
	std::fstream in;
	in.open(user_fn, std::ios::in);

	if (!in.is_open()) {
		in.clear();
		in.open(user_fn, std::ios::out); // create file	
		first_launch = true;
	}else{
		while (!in.eof()) {
			char arr_ch[128]{};
			in.getline(arr_ch, 128);
			if (strlen(arr_ch) == 0)
				continue;
			CUser* uptr = new CUser(arr_ch);
			arr_users.add(uptr);
		}
	}
	in.close();

	in.open(msg_fn, std::ios::in);
	if (!in.is_open()) {
		in.clear();
		in.open(msg_fn, std::ios::out); // create file	
	}else{
		while (!in.eof()) {
			char arr_ch[128]{};
			in.getline(arr_ch, 128);
			if (strlen(arr_ch) == 0)
				continue;
			CMessage* mptr = new CMessage(arr_ch);
			arr_msg.add(mptr);
		}
	}
	in.close();
}
//--------------------------------------------------------------------------------------
void CSession::close() {
	std::fstream out;
	out.open(msg_fn, std::ios::out);
	if (out.is_open()) {
		for (size_t i = 0; i < arr_msg.size(); i++)	{
			out << arr_msg[i]->get_ts() << '|' << arr_msg[i]->get_sender() << '|' << arr_msg[i]->get_recv() << '|' << arr_msg[i]->get_body() << '|' << arr_msg[i]->get_type() << std::endl;
		}
	}	
	out.close();
}
//--------------------------------------------------------------------------------------
auto CSession::show_main_menu()->MMI {
	std::cout << "Choose action: \n\twrite to user (r)\n\twrite to all (a)\n\tQuit (q):\n";
	std::string input;
	std::getline(std::cin, input, '\n');
	switch (input.at(0)) {
		case 'r': {
			if (arr_users.size()) {
				std::cout << "Available users(type user name):\n";
				for (size_t i = 0; i < arr_users.size(); i++) {
					if (current_user != arr_users[i])
						std::cout << "\t" << arr_users[i]->get_name() << std::endl;
				}
				return eSendToUser;
			}
			else {
				std::cout << "No available users:\n";
				return eNothing;
			}
			break;
		}
		case 'a': {
			if (arr_users.size()) {
				return eSendToAll;
			}else {
				std::cout << "No available users:\n";
				return eNothing;
			}
		}
		case 'q': {
			return eQuit;
		}
		default:
			return eQuit;
	}
}
//--------------------------------------------------------------------------------------
void CSession::save_user_data() const {
	std::fstream out;
	out.open(user_fn,std::ios::app);
	if (!out.is_open()) {
		out.clear();
		out.open(user_fn, std::ios::out); // create file	
	}
	if(current_user)
		out << current_user->get_name() << ',' << current_user->get_pwd() << std::endl;

	out.close();
}
//--------------------------------------------------------------------------------------
auto CSession::login()->bool{
	std::string u_name;
	std::string u_pwd;
	std::string input;

	if (first_launch) {
		std::cout << "Welcome to the chat session. Choose Login (Enter to exit):\n";
		std::getline(std::cin, u_name, '\n');

		if (u_name.size()) {
			while(is_valid_user(u_name)) {
				std::cout << "This login is used. Choose another:\n";
				std::getline(std::cin, u_name, '\n');
			}

			std::cout << "Choose Password:\n";
			std::getline(std::cin, u_pwd, '\n');

			while (!u_pwd.size()) {
				std::cout << "Empty password not allowed. Choose one:\n";
				std::getline(std::cin, u_pwd, '\n');
			}

			try {
				current_user = new CUser();
			}catch (const std::exception& e) {
				std::cout << "Failed to create user instance:" << e.what() << std::endl;
				return false;
			}
		
			current_user->set_name(u_name);
			current_user->set_pwd(u_pwd);
			save_user_data();
			is_new_user = true;
			return true;
		}else 
			return false;
		
	}else{
		std::cout << "Welcome to the chat session. New user?(y/n. Enter to exit):\n";
		std::getline(std::cin, input, '\n');
		if (input.size()) {
			switch (input.at(0)) {
			case 'y': {
				std::cout << "Choose Login:\n";
				std::getline(std::cin, u_name, '\n');
				while (is_valid_user(u_name)) {
					std::cout << "This login is used. Choose another:\n";
					std::getline(std::cin, u_name, '\n');
				}

				std::cout << "Choose Password:\n";
				std::getline(std::cin, u_pwd, '\n');

				while (!u_pwd.size()) {
					std::cout << "Empty password not allowed. Choose one:\n";
					std::getline(std::cin, u_pwd, '\n');
				}

				try {
					current_user = new CUser();
				}
				catch (const std::exception& e) {
					std::cout << "Failed to create user instance:" << e.what() << std::endl;
					return false;
				}
				current_user->set_name(u_name);
				current_user->set_pwd(u_pwd);
				save_user_data();
				is_new_user = true;

				return true;

				break;
			}
			case 'n': {
				std::cout << "Login:\n";
				std::getline(std::cin, u_name, '\n');
				int found(-1);

				for (size_t i = 0; i < arr_users.size(); i++) {
					if (arr_users[i]->get_name() == u_name) {
						found = i;
						break;
					}
				}

				if (found != -1) {
					std::cout << "Password:\n";
					std::getline(std::cin, u_pwd, '\n');

					if (arr_users[found]->get_pwd() == u_pwd) {
						std::cout << "You have succesfully logged in\n";
						current_user = arr_users[found];
						return true;
					}else {
						std::cout << "Wrong password(3 more attempts):\n";
						bool ok(false);						
						for (size_t i = 3; i > 0; i--){
							std::getline(std::cin, u_pwd, '\n');
							if (arr_users[found]->get_pwd() == u_pwd) {
								ok = true;
								break;
							}
							if (!ok && i>1) {
								std::cout << "Wrong password.(" << i-1 << " more attempts):\n";
							}else
								break;
						}

						if (ok) {
							std::cout << "You have succesfully logged in\n";
							current_user = arr_users[found];
							return true;
						}else {
							std::cout << "Login failed.\n";
							return false;
						}						
					}
				}
				else {
					std::cout << "Wrong user name.(3 more attempts):\n";
					for (size_t i = 3; i > 0; i--){
						std::getline(std::cin, u_name, '\n');
						found = -1;
						for (size_t j = 0; j < arr_users.size(); j++) {
							if (arr_users[j]->get_name() == u_name) {
								found = j;
								break;
							}
						}
						if (found == -1 && i>1) {
							std::cout << "Wrong user name.("<<i-1<<" more attempts):\n";
						}
					}

					if (found != -1) {
						std::cout << "Password:\n";
						std::getline(std::cin, u_pwd, '\n');

						if (arr_users[found]->get_pwd() == u_pwd) {
							std::cout << "You have succesfully logged in\n";
							current_user = arr_users[found];
							return true;
						}
						else {
							std::cout << "Wrong password(3 more attempts):\n";
							bool ok(false);
							for (size_t i = 3; i > 0; i--) {
								std::getline(std::cin, u_pwd, '\n');
								if (arr_users[found]->get_pwd() == u_pwd) {
									ok = true;
									break;
								}
								if (!ok && i > 1) {
									std::cout << "Wrong password.(" << i - 1 << " more attempts):\n";
								}
								else
									break;
							}

							if (ok) {
								std::cout << "You have succesfully logged in\n";
								current_user = arr_users[found];
								return true;
							}
							else {
								std::cout << "Login failed.\n";
								return false;
							}
						}
					}else {
						std::cout << "Login failed.\n";
						return false;
					}
				}		
				break;
			}
			default: {
				std::cout << "Wrong character: type y or n \n";
				return login();
			}

			}
		}else {
			return false;
		}
	}
}
//--------------------------------------------------------------------------------------
auto CSession::send_msg(const std::string& msg, const std::string& sender, const std::string& rcv) -> bool {
	try {
		CMessage* msg_ptr = new CMessage(msg.c_str(), sender, rcv);
		arr_msg.add(msg_ptr);
		return true;
	}catch (const std::exception& e) {
		std::cout << "Failed to send message:"<<e.what()<<std::endl;
		return false;
	}
}
//--------------------------------------------------------------------------------------
auto CSession::send(const std::string& uname) -> MMI {
	return show_msg_menu(uname);
}
//--------------------------------------------------------------------------------------
auto CSession::broadcast() -> MMI {
	std::string msg;
	std::cout << "Type your message to all:\n";
	std::getline(std::cin, msg);
	for (size_t i = 0; i < arr_users.size(); i++){
		if (current_user == arr_users[i])
			continue;
		try{
			if (send_msg(msg, current_user->get_name(), arr_users[i]->get_name())) 
				std::cout << "Message sent to: "<< arr_users[i]->get_name()<<"\n";
		}catch ( ... ) {
			std::cout << "Broadcast failed!!!\n";
			break;
		}
	}
	return eNothing;
}
//--------------------------------------------------------------------------------------
void CSession::show_user_msg(){
	if (!first_launch && !is_new_user && arr_msg.size() > 0) {		
		size_t msg_cnt(0);
		std::string cusr = current_user->get_name();
		for (size_t i = 0; i < arr_msg.size(); i++) {
			if (arr_msg[i]->get_recv() == cusr && arr_msg[i]->get_type() == eNew)
				msg_cnt++;
		}

		if (msg_cnt) {
			std::cout << "You have new messages:" << std::endl;
			for (size_t i = 0; i < arr_msg.size(); i++){
				if (arr_msg[i]->get_recv() == cusr && arr_msg[i]->get_type()==eNew){
					//std::cout << arr_msg[i] << std::endl;
					arr_msg[i]->show();
					arr_msg[i]->set_msg_read();
				}
			}
		}else
			std::cout << "You have no new messages." << std::endl;
	}else{
		if(!is_new_user)
			std::cout << "You have no new messages." << std::endl;
	}
		
}
//--------------------------------------------------------------------------------------
auto CSession::is_valid_user(const std::string& uname) -> bool {
	int found(-1);
	for (size_t i = 0; i < arr_users.size(); i++) {
		if (arr_users[i]->get_name() == uname) {
			found = i;
			break;
		}
	}
	return  found == -1 ? false : true;
}
//--------------------------------------------------------------------------------------
auto CSession::start_session() -> bool {
	if (login()) {
		show_user_msg();
		MMI res = show_main_menu();
		while (res != eQuit) {
			std::cin.clear();
			std::cin.sync();
			switch (res) {
				case eSendToUser: {
					std::string u_rec;
					std::getline(std::cin, u_rec);
					if (is_valid_user(u_rec)) {
						res = send(u_rec);
					}
					break;
				}
				case eSendToAll: {
					res = broadcast();
					break;
				}
				case eNothing: {
					res = show_main_menu();
					break;
				}

			}
		}

		if(eQuit==res)
			return false;
		else
			return true;
	}else
		return false;

}
//--------------------------------------------------------------------------------------
auto CSession::show_msg_menu(const std::string& uname) -> MMI {
	std::string msg,cusr, choice;
	std::cout << "Type your message to " << uname << ":\n";
	std::getline(std::cin, msg,'\n');
	MMI res(eNothing);
	cusr = get_current_user()->get_name();
	if (send_msg(msg, cusr, uname)) {
		std::cout << "Message sent\n";
		std::cout << "Send another? (y/n):\n";
		std::getline(std::cin, choice, '\n');
		while (choice == "y") {
			std::cout << "Type your message:\n";
			std::getline(std::cin, msg, '\n');
			if(send_msg(msg, cusr, uname))
				std::cout << "Message sent\n";
			std::cout << "Send another? (y/n):\n";
			std::getline(std::cin, choice, '\n');
		}
		res = show_main_menu();
	}else {
		std::cout << "Failed to send message\n";
	}
	return res;
}
//--------------------------------------------------------------------------------------