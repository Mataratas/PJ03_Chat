#pragma once
#include <iostream>
#include <string>


class CUser{
public:
	CUser()=default;
	explicit CUser(const char*);
	CUser(const CUser&) = delete;
	~CUser() = default; 
	CUser& operator=(const CUser&) = delete;

	template<class S> void set_name(S nm) { name = nm; }
	template<class S> void set_pwd(S p) { pwd = p; }

	std::string get_name() const { return name; }
	std::string get_pwd() const { return pwd; }
	bool operator==(const CUser& rhs); 
	bool operator!=(const CUser& rhs);

	//template<class T> bool is_equal(T& rhs) const { return name == rhs.get_name() && pwd == rhs.get_pwd(); }; //Error	C2228	left of '.get_name' must have class / struct / union
	
	friend std::ostream& operator<<(std::ostream& out, const CUser&);
private:
	std::string name;
	std::string pwd;
};

