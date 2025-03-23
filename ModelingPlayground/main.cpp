#include "Application/ApplicationManager.h"

int main()
{
	auto app = ApplicationManager();
	int exitCode = app.Run();
	return exitCode;
}
