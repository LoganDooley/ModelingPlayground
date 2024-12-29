#include "Application/ApplicationManager.h"

int main() 
{
	ApplicationManager app = ApplicationManager();
	int exitCode = app.Run();
	return exitCode;
}