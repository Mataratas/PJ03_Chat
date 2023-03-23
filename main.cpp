#include "CSession.h"
//#include <chrono>
#include <stdlib.h>
//======================================================================================
bool settime(struct tm* u, std::string& dtm )
{
	char s[40]{};
	try {
		int length = strftime(s, 40, "%d.%m.%Y %H:%M:%S, %A", u);
		dtm = s;
	}
	catch (const std::exception& e) {
		std::cout << "Date conversion failed:" << e.what() << std::endl;
		return false;

	}
	return(true);
}



void test_tm() {

	std::string iv("1679339210");
	
	unsigned long nsec = strtoul(iv.c_str(),nullptr,10);//static_cast<size_t>(iv.c_str());

	time_t timer{ nsec };
	
	#pragma warning(suppress : 4996)
	struct tm* u = localtime(&timer);

	bool res = settime(u, iv);



	//std::chrono::system_clock::time_point tp{ std::chrono::seconds{nsec} };
	//auto tm = std::chrono::system_clock::to_time_t(tp);
	


	//std::cout << "Initial value:" << iv << std::endl;

	if(res)
		std::cout << "Time:" << iv << std::endl;

	//char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
	//#pragma warning(suppress : 4996)
	//std::strftime(std::data(timeString), std::size(timeString), "%FT%TZ", std::gmtime(&tm));
	//std::cout << timeString << '\n';







}


int main(){
	

	//test_tm();

	//return 0;

	CSession CS;
	while (CS.start_session());

	
	return 0;

}