#ifndef _PANEL_H_
#define _PANEL_H_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xft/Xft.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <string>

#ifdef NEEDS_BASENAME
#include <libgen.h>
#endif

#include "switchuser.h"
#include "log.h"
#include "image.h"

struct Rectangle {
	int x;
	int y;
	unsigned int width;
	unsigned int height;

	Rectangle() : x(0), y(0), width(0), height(0) {};
	Rectangle(int x, int y, unsigned int width,
					unsigned int height) :
		x(x), y(y), width(width), height(height) {};
	bool is_empty() const {
		return width == 0 || height == 0;
	}
};

class Panel {
public:
	enum ActionType {
		Login,
		Lock,
		Console,
		Reboot,
		Halt,
		Exit,
		Suspend
	};

	enum FieldType {
		Get_Name,
		Get_Passwd
	};

	enum PanelType {
		Mode_DM,
		Mode_Lock
	};

	Panel(Display *dpy, int scr, Window root, Cfg *config,
		  const std::string& themed, PanelType panel_mode);
	~Panel();
	void OpenPanel();
	void ClosePanel();
	void ClearPanel();
	void WrongPassword(int timeout);
	void Message(const std::string &text);
	void Error(const std::string &text);
	void EventHandler(const FieldType &curfield);
	std::string getSession();
	ActionType getAction(void) const;

	void Reset(void);
	void ResetName(void);
	void ResetPasswd(void);
	void SetName(const std::string &name);
	const std::string& GetName(void) const;
	const std::string& GetPasswd(void) const;
	void SwitchSession();
private:
	Panel();
	void Cursor(int visible);
	unsigned long GetColor(const char *colorname);
	void OnExpose(void);
	void EraseLastChar(string &formerString);
	bool OnKeyPress(XEvent& event);
	void ShowText();
	void ShowSession();

	void CrystalDrawString8(XftDraw *d, XftColor *color, XftFont *font,
							int x, int y, const std::string &str,
							XftColor *shadowColor,
							int xOffset, int yOffset);

	Rectangle GetPrimaryViewport();
	void ApplyBackground(Rectangle = Rectangle());

	/* Private data */
	PanelType mode; /* work mode */
	Cfg *cfg;
	Window Win;
	Window Root;
	Display *Dpy;
	int Scr;
	int X, Y;
	GC TextGC;
	GC WinGC;
	XftFont *font;
	XftColor inputshadowcolor;
	XftColor inputcolor;
	XftColor msgcolor;
	XftColor msgshadowcolor;
	XftFont *msgfont;
	XftColor introcolor;
	XftFont *introfont;
	XftFont *welcomefont;
	XftColor welcomecolor;
	XftFont *sessionfont;
	XftColor sessioncolor;
	XftColor sessionshadowcolor;
	XftColor welcomeshadowcolor;
	XftFont *enterfont;
	XftColor entercolor;
	XftColor entershadowcolor;
	ActionType action;
	FieldType field;
	//Pixmap   background;
	
	/* Username/Password */
	std::string NameBuffer;
	std::string PasswdBuffer;
	std::string HiddenPasswdBuffer;

	/* screen stuff */
	Rectangle viewport;

	/* Configuration */
	int input_name_x;
	int input_name_y;
	int input_pass_x;
	int input_pass_y;
	int inputShadowXOffset;
	int inputShadowYOffset;
	int input_cursor_height;
	int welcome_x;
	int welcome_y;
	int welcome_shadow_xoffset;
	int welcome_shadow_yoffset;
	int session_shadow_xoffset;
	int session_shadow_yoffset;
	int intro_x;
	int intro_y;
	int username_x;
	int username_y;
	int username_shadow_xoffset;
	int username_shadow_yoffset;
	int password_x;
	int password_y;
	std::string welcome_message;
	std::string intro_message;

	/* Pixmap data */
	Pixmap PanelPixmap;

	Image *image;

	/* For thesting themes */
	bool testing;
	std::string themedir;

	/* Session handling */
	std::string session_name;
    std::string session_exec;
};

#endif /* _PANEL_H_ */
