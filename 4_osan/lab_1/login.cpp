#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>

// scp -P 8022 morikov@sustavovs.ru:~/login.cpp ~/gitproj/labs_4/lab_1/

enum ArgCode {
	setPsswd,
	checkPsswd
};

int hashArg(std::string& arg) {
	if (arg == "--set")
		return ArgCode::setPsswd;
	if (arg == "--check")
		return ArgCode::checkPsswd;
	return -1;
}

int main(int argc, char* argv[]) {
	if (4 < argc || argc > 5) {
		std::cout << "Wrong number of arguments " << argv << " insteard 2" << std::endl; 
	}

	std::string flag(argv[1]);
	std::string folder("passwords/");
	std::string usrname(argv[2]);
	std::string fullpath(folder + usrname);
	std::string psswd(argv[3]);

	//struct stat file_s = {0};
	//if (stat(fullpath.c_str(), &file_s) != 0) {
	//	std::cout << strerror(errno) << std::endl;
	//}
	//std::cout << file_s.st_mode << std::endl;

	switch(hashArg(flag)) {
		case(ArgCode::setPsswd):
		{
			if ( access(fullpath.c_str(), F_OK) != -1 ) {
				std::cout << "password already exists" << std::endl;
				std::cout << strerror(errno) << std::endl;
				return 1;
			}
		
			std::ofstream npsswd(fullpath.c_str());	
			npsswd << psswd << std::endl;
			npsswd.close();
			std::cout << "password has set" << std::endl;
		}
		break;
		case(ArgCode::checkPsswd):
		{
			//if (false && access(fullpath.c_str(), F_OK) == -1 ) {
			//	std::cout << "Password doesnt exist" << std::endl;
			//	std::cout << strerror(errno) << std::endl;
			//	return 1;
			//}
			
			std::ifstream epsswd(fullpath.c_str());
			
			//std::cout << epsswd.good() << std::endl;
			
			std::string right_psswd;
			epsswd >> right_psswd;
			if (right_psswd == psswd && epsswd.good())
				std::cout << "Password correct!" << std::endl;
			else
				std::cout << "Password incorrect!" << std::endl;
			epsswd.close();
		}
		break;
	}

	return 0;
}
