#ifndef _CFG_H_
#define _CFG_H_

#include <string>
#include <map>
#include <vector>

#define INPUT_MAXLENGTH_NAME	30
#define INPUT_MAXLENGTH_PASSWD  50

#define CFGFILE SYSCONFDIR"/crystal.conf"
#define THEMESDIR PKGDATADIR"/themes"
#define THEMESFILE "/crystal.theme"

class Cfg {

public:
	Cfg();
	~Cfg();

	bool readConf(std::string configfile);
	std::string parseOption(std::string line, std::string option);
	const std::string& getError() const;
	std::string& getOption(std::string option);
	int getIntOption(std::string option);
	std::string getWelcomeMessage();

	static int absolutepos(const std::string &position, int max, int width);
	static int string2int(const char *string, bool *ok = 0);
	static void split(std::vector<std::string> &v, const std::string &str, 
					  char c, bool useEmpty=true);
	static std::string Trim(const std::string &s);

    std::pair<std::string,std::string> nextSession();

private:
	void fillSessionList();

private:
	std::map<std::string,std::string> options;
    std::vector<std::pair<std::string,std::string> > sessions;
	int currentSession;
	std::string error;
};

#endif /* _CFG_H_ */
