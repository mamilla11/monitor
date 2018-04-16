#include <critical.hpp>

#include <hw/System.hpp>

#include <tasks/HttpTask.h>
#include <tasks/LoggerTask.h>
#include <tasks/MonitorTask.h>
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

BEGIN_DECLS
void* __dso_handle;
void* _fini;
END_DECLS

int main()
{
    hw::system::clock_init();
    hw::system::systick_init();
    hw::system::irq_disable();

    auto logger_task     = new tasks::LoggerTask();

#ifdef MONITOR
    auto monitor_task    = new tasks::MonitorTask();
#endif
	auto green_led_task  = new tasks::HttpTask();
	//auto red_led_task    = new tasks::RedLedTask();
	auto yellow_led_task = new tasks::YellowLedTask();

    rtos::Thread::StartScheduler();

    while (true);

    return 0;
}

#if configGENERATE_RUN_TIME_STATS == 1
void vConfigureTimerForRunTimeStats(void)
{
    tasks::MonitorTask::config_timer();
}

unsigned long vGetTimerForRunTimeStats(void)
{
    return (tasks::MonitorTask::get_counter_value());
}
#endif

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

BEGIN_DECLS
void vApplicationMallocFailedHook()
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

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
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
END_DECLS
