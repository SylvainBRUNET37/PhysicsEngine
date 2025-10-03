#include "pch.h"

#include <tchar.h>

#include "rendering/application/WindowsApplication.h"
#include "systems/MainEngine.h"

int APIENTRY _tWinMain(const HINSTANCE hInstance,
                       HINSTANCE,
                       LPTSTR,
                       int)
{
    WindowsApplication application(hInstance);

    if (!application.Init())
        return EXIT_FAILURE;

    const MainEngine mainEngine{ WindowsApplication::ProcessWindowMessages };

    mainEngine.Run();

    return EXIT_SUCCESS;
}
