#include "remote_logging.hpp"

int main(int argc, char* argv[]) {
	std::string input;
	RemoteLogging LOG;
	
	LOG.set_loghost("localhost", "3000").set_prefix("LOGGING ");

	std::cout << "Started" << std::endl;
	while (std::cin >> input) {
		LOG << input << std::endl;
	}
}
