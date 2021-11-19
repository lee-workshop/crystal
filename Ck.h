#ifndef _CK_H_
#define _CK_H_

#include <string>

#include <ck-connector.h>
#include <dbus/dbus.h>

namespace Ck {
	class Exception {
	public:
	std::string func;
	std::string errstr;
	Exception(const std::string &func, const std::string &errstr);
	};

	class Session {
	private:
	CkConnector *ckc;
	DBusError error;

	const char * get_x11_device(const std::string &display);
	dbus_bool_t ck_connector_open_graphic_session(const std::string &display,
		uid_t uid);
	public:
	const char * get_xdg_session_cookie();
	void open_session(const std::string &display, uid_t uid);
	void close_session();

	Session();
	~Session();
	};
}

std::ostream &operator<<(std::ostream &os, const Ck::Exception &e);

#endif /* _CK_H_ */
