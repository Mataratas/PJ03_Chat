#include "CUser.h"
//---------------------------------------------------------------------------------------
CUser::CUser(const char* nm){
	std::string stmp(nm);
	size_t pos = stmp.find(',');
	if (pos != std::string::npos) {
		name = stmp.substr(0, pos);
		pwd = stmp.substr(pos+1);
	}
	else
		name = nm;
}
//---------------------------------------------------------------------------------------
bool CUser::operator==(CUser& rhs) {
	return name == rhs.get_name() && pwd == rhs.get_pwd();
}
//---------------------------------------------------------------------------------------
bool CUser::operator!=(CUser& rhs) {
	return name != rhs.get_name() || pwd != rhs.get_pwd();
}
//---------------------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& out, const CUser& usr) {
	out << "Name:" << usr.name << " Password:" << usr.pwd << std::endl;
	return out;
}
//---------------------------------------------------------------------------------------