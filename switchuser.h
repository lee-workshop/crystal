#ifndef _SWITCHUSER_H_
#define _SWITCHUSER_H_

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <paths.h>
#include <cstdio>
#include <iostream>
#include "log.h"
#include "cfg.h"


class SwitchUser {
public:
	SwitchUser(struct passwd *pw, Cfg *c, const std::string& display,
			   char** _env);
	~SwitchUser();
	void Login(const char* cmd, const char* mcookie);

private:
	SwitchUser();
	void SetEnvironment();
	void SetUserId();
	void Execute(const char* cmd);
	void SetClientAuth(const char* mcookie);
	Cfg* cfg;
	struct passwd *Pw;

	std::string displayName;
	char** env;
};

#endif /* _SWITCHUSER_H_ */
