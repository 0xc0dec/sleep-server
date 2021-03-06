#include <httplib.h>
#include <Windows.h>
#include <iostream>
#include <future>

void sleepWindows()
{
    HANDLE token;
    OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);

    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    LookupPrivilegeValueA("", SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
    AdjustTokenPrivileges(token, false, &tp, sizeof(tp), nullptr, 0);

    SetSystemPowerState(true, true);
}

int main()
{
    using namespace httplib;

    Server server;

    server.Get("/sleep", [](const Request &req, Response &resp)
    {
        resp.set_content("Zzz...", "text/plain");
        sleepWindows();
    });

    server.set_base_dir(".");

    server.listen("0.0.0.0", 13456);
}
