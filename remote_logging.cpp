#include "remote_logging.hpp"

#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define BEEN_HERE std::cout << __PRETTY_FUNCTION__ << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl

using namespace tmacam;

int MakeSocketFromAddr(const char* hostname, const char* servname) {
	const int ERROR = -1;

	/* most code from getaddrinfo manpage */
	int sfd = ERROR; // socket file destriptor
	int s; // for getaddrinfo return
	struct addrinfo hints;
	struct addrinfo *result, *rp;  // will point to the results

	/* Obtain address(es) matching host/port */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_flags = 0;
	hints.ai_protocol = 0;          /* Any protocol */

	s = getaddrinfo(hostname, servname, &hints, &result);
	if (s != 0) {
		std::ostringstream err_msg;
		err_msg << "getaddrinfo() for address '" << hostname << 
			":" << servname << "': " << gai_strerror(s);

		std::cerr << err_msg.str() << std::endl;
		return -1;
	}

	/* getaddrinfo() returns a list of address structures.
	   Try each address until we successfully connect(2).
	   If socket(2) (or connect(2)) fails, we (close the socket
	   and) try the next address. */
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,
				rp->ai_protocol);
		if (sfd == ERROR) {
			continue;
		}
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
			break;                  /* Success */
		}
		close(sfd);
	}
	if (rp == NULL) {               /* No address succeeded */
		std::ostringstream err_msg;
		err_msg << "Could not connect to '" << hostname << ":" <<
			servname << "'.";
		std::cerr << err_msg.str() << std::endl;
		return ERROR;
	}

	freeaddrinfo(result);		/* No longer needed */

	return sfd;
}


std::string RemoteLogging::prefix() const { return prefix_; }

RemoteLogging& RemoteLogging::set_prefix(const std::string& prefix ) {
	prefix_ = prefix;
	return *this;
}

RemoteLogging& RemoteLogging::set_loghost(const char* host, const char* port) {
	skt_ = MakeSocketFromAddr(host, port);
}

RemoteLogging& RemoteLogging::operator<<(const std::string& msg) {
	std::string log_msg = prefix_ + msg;
	if (skt_ ) {
		int res;
		res = send(skt_, log_msg.c_str(), log_msg.size(), 0);
		if(res < 0) {
			std::cerr << "LOGGING Oops! send() returned "
				<< res << "." << std::endl;
			std::cout << log_msg << std::endl;
		}
	} else {
		std::cout << log_msg << std::endl;
	}
	return *this;
}


