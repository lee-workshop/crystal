#ifndef _UTIL_H__
#define _UTIL_H__

#include <string>

namespace Util {
	bool add_mcookie(const std::string &mcookie, const char *display,
		const std::string &xauth_cmd, const std::string &authfile);

	void srandom(unsigned long seed);
	long random(void);

	long makeseed(void);
}

#endif /* _UTIL_H__ */
