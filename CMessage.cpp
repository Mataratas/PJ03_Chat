#include "CMessage.h"
//---------------------------------------------------------------------------------------------
CMessage::CMessage(const char* msg, const std::string& snd, const std::string& rcv):body(msg), reciever(rcv), sender(snd), timestamp(std::time(nullptr)){

}
//---------------------------------------------------------------------------------------------
CMessage::CMessage(const char* msg) {
	if (msg) {
		std::string s_tmp(msg);
		size_t pos = s_tmp.find('|');
		std::string arr_parts[5];
		size_t cnt{ 0 };

		while (pos!=std::string::npos) {
			arr_parts[cnt++] = s_tmp.substr(0, pos);
			s_tmp.erase(0, pos+1);
			pos = s_tmp.find('|');
		}
		arr_parts[cnt] = s_tmp;
		timestamp = strtoul(arr_parts[0].c_str(), nullptr, 10);
		sender = arr_parts[1];
		reciever = arr_parts[2];
		body = arr_parts[3];		
		type = std::stoi(s_tmp)==1 ? eOld : eNew;
	}
}
//---------------------------------------------------------------------------------------------
auto CMessage::show_time() -> const std::string{
	#pragma warning(suppress : 4996)
	struct tm* u = localtime(&timestamp);
	std::string dtm;
	char s[40]{};
	try {
		int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);
		dtm = s;
	}
	catch (const std::exception& e) {
		std::cout << "Date conversion failed:" << e.what() << std::endl;
	}
	return dtm;
}
//---------------------------------------------------------------------------------------------
void CMessage::show(){
	std::cout << "\t\t---------------------------------------------------\n";
	std::cout << "\t\tDate [" << show_time() << "]" << " From:" << sender << " To:" << reciever << std::endl;
	std::cout << "\t\t---------------------------------------------------\n";
	std::cout << "\t\tMessage:\n\t\t" << body << std::endl;

}
//---------------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, CMessage& rhs) {

	out << "\t\t***************************************************\n";
	out <<"\t\tDate ["<< rhs.show_time() <<"]" << " From:" << rhs.get_sender() << " To:" << rhs.get_recv() << std::endl;
	out << "\t\t***************************************************\n";
	out << "\t\tMessage:\n\t\t" << rhs.get_body()<< std::endl;

	return out;
}
//---------------------------------------------------------------------------------------------
void CMessage::set_recv(const std::string& usr) {
	reciever = usr;
}
//---------------------------------------------------------------------------------------------
void CMessage::set_sender(const std::string& usr) {
	sender = usr;
}
//---------------------------------------------------------------------------------------------
void CMessage::set_msg_read() {
	type = eOld;
}

