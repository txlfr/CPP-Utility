#include "Global.h"

using namespace app;

void FiberLoop()
{
    while (true)
    {
        util::do_once([] {
            console::sub_header("Gradient message sent once"_gradient);
        });

        static int delay = 500;
        util::do_timed(delay, [] {
            console::sub_header("This is sent every 0.5 seconds !");
        });

        Fiber::get()->yield();
    }
}

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    Fiber::ensure_thread_is_a_fiber();

    console::create(APP_NAME);
    console::sub_header("Me when i am bored");

    fiber_manager::add_fiber("Example", &FiberLoop);

    while (true)
    {
        fiber_manager::tick();

        std::this_thread::sleep_for(0ms);

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            break;
        }
    }

    console::raw_send("________________________________________________\n");

    MessageBoxA(NULL, "Press OK to exit.", "Close Application", MB_OK);

    console::destroy();
    return 0;
}
