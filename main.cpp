#include <critical.hpp>

#include <hw/System.hpp>

#include <tasks/GreenLedTask.h>
#include <tasks/RedLedTask.h>
#include <tasks/YellowLedTask.h>

namespace rtos = cpp_freertos;

void* malloc(size_t size)
{
    return pvPortMalloc(size);
}

void* calloc(size_t num, size_t size)
{
    return pvPortMalloc(num * size);
}

void free(void* ptr)
{
    return vPortFree(ptr);
}

void* operator new(size_t sz)
{
    return pvPortMalloc(sz);
}

void* operator new[](size_t sz)
{
    return pvPortMalloc(sz);
}

void operator delete(void* p)
{
    vPortFree(p);
}

void operator delete[](void* p)
{
    vPortFree(p);
}

extern "C" {
void* __dso_handle;
void* _fini;
}

int main()
{
    hw::system::clock_init();
    hw::system::systick_init();
    hw::system::irq_disable();

	auto green_led_task  = new tasks::GreenLedTask();
	auto red_led_task    = new tasks::RedLedTask();
	auto yellow_led_task = new tasks::YellowLedTask();

    rtos::Thread::StartScheduler();

    while (true);

    return 0;
}

void vAssertCalled(const char* pcFile, unsigned long ulLine)
{
    rtos::CriticalSection::Enter();
    {
        while (true)
        {
            portNOP();
        }
    }
    rtos::CriticalSection::Exit();
}
