#include "app.h"
#include "const.h"

App* LoginApp = 0;

int main(int argc, char** argv)
{
	LoginApp = new App(argc, argv);
	LoginApp->Run();
	return 0;
}

