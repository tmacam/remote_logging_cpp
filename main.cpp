#include "remote_logging.hpp"

int main(int argc, char* argv[]) {
	std::string input;
	tmacam::RemoteLogging LOG;
	
	LOG.set_loghost("localhost", "3000").set_prefix("LOGGING ");

	std::cout << "Started" << std::endl;
	LOG << "This is " << 1 << " typical example log usage" << std::endl;
	while (std::cin >> input) {
		LOG << input << std::endl;
	}
}
