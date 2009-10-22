#ifndef REMOTE_LOGGING_HPP_
#define REMOTE_LOGGING_HPP_

#include <iostream>
#include <sstream>
#include <string>

namespace tmacam {

/** Make a UDP client socket connected to hostname:servname
 *
 * Errors returned by the syscalls used by this function are reported to
 * stderr.
 *
 * @return -1 in case of errors, a valid socket file descriptor otherwise.
 */
int MakeSocketFromAddr(const char* hostname, const char* servname);


/**Remote logging made easy with a std. stream-like interface.
 *
 * This class is to be used in place of std::cout for logging. But instead of
 * outputting (only) to the stdout, it will output to a remote
 * udp server. If no server is supplied or setup than it will just output
 * to std::cout.
 *
 * An optional prefix can be set and will be prepended to every message "logged"
 * using this class.
 *
 * Notice that, just as logging w/ printf's std::cout, this is a blocking
 * logging facility.
 *
 */
class RemoteLogging {

	int skt_; // the socket file descriptor
	std::string prefix_;
	std::ostringstream msg_buffer_;

public:
	//!Default constructor -- forces logging to stdout.
	RemoteLogging() : skt_(0), prefix_() {}

	/**Constructor
	 *
	 * @param host Hostname of the logging server.
	 * @param port Port or Serv. name of the logging server.
	 *
	 */
	RemoteLogging(const char* host, const char* port) 
		: skt_(MakeSocketFromAddr(host, port)), prefix_() {}

	//!Destructor
	~RemoteLogging() {
		if (skt_) {
			close(skt_);
		}
	}

	std::string prefix() const;
	RemoteLogging& set_prefix(const std::string& prefix );

	RemoteLogging& set_loghost(const char* host, const char* port);

	/** Add msg to the log buffer.
	 *
	 * Output will be delayed untill Flush() is called. Applying a
	 * std::endl to us has the same effect of calling Flush().
	 *
	 */
	template <class T> RemoteLogging& operator<<(const T& msg) {
		msg_buffer_ << msg;
		return *this;
	}

	//! Force outputing the log buffer contents.
	RemoteLogging& Flush();

	// Deals with << endl in a sane manner.
	// http://bytes.com/topic/c/answers/128046-std-endl-type-unknown
	inline RemoteLogging& operator<<( std::ostream& (*f)(std::ostream&) ) {
		return this->Flush(); 
	}

private:
	// Disallow evil constructors
	RemoteLogging(const RemoteLogging&);
	RemoteLogging& operator=(const RemoteLogging&);
};

}; // namespace tmacam

#endif /* REMOTE_LOGGING_HPP_ */
