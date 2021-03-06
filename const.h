#ifndef _CONST_H_
#define _CONST_H_

#define APPNAME	"crystal"
#define DISPLAY	":0.0"

#define CONSOLE_STR	 "console"
#define HALT_STR		"halt"
#define REBOOT_STR	  "reboot"
#define EXIT_STR		"exit"
#define SUSPEND_STR	 "suspend"

#define HIDE		0
#define SHOW		1

#define GET_NAME	0
#define GET_PASSWD  1

#define OK_EXIT	 0
#define ERR_EXIT	1

/* duration for showing error messages,
 * as "login command failed", in seconds 
 */
#define ERROR_DURATION  5

/* variables replaced in login_cmd */
#define SESSION_VAR	 "%session"
#define THEME_VAR	   "%theme"

/* variables replaced in pre-session_cmd and post-session_cmd */
#define USER_VAR	   "%user"

/* max height/width for images */
#define MAX_DIMENSION 10000

#endif /* _CONST_H_ */
