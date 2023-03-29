#pragma once
#include <ctime>
#include "CUser.h"

enum MsgType {
	eNew,
	eOld
};

class CMessage{
public:
	CMessage(const char*, const std::string&, const std::string&);
	explicit CMessage(const char*);
	~CMessage()=default;
	CMessage(const CMessage&)=delete;
	CMessage& operator=(const CMessage&)=delete;
	friend std::ostream& operator<<(std::ostream& out, const CMessage&);	
	template<class S> void set_body(S& b) { body = b;}
	auto get_recv()-> std::string const { return reciever; };
	auto get_sender()-> std::string const { return sender; };
	auto get_ts() -> std::time_t const { return timestamp; };
	auto get_body() -> std::string const { return body; };
	auto get_type() -> MsgType const { return type; };
	auto show_time() -> const std::string;
	void show();

	void set_recv(const std::string&);
	void set_sender(const std::string&);
	void set_msg_read();


private:
	std::time_t timestamp{};
	std::string sender;
	std::string reciever;
	std::string body;
	MsgType type{eNew};
};

