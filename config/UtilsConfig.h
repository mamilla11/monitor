#ifndef CONFIG_UTILS_H_
#define CONFIG_UTILS_H_

#include "stdint.h"

namespace utils {
namespace config {
static constexpr uint32_t MAX_INF_SOURCES=10;
static constexpr uint32_t MAX_INF_STRING_LEN = 250;


static constexpr uint32_t MAX_CONF_FILE_PARAM_LEN = 32;
static constexpr uint32_t MAX_CONF_FILE_CHILDS = 10;


//#define INF_WORK_WITH_SEMAPHORE 
}
}

#endif //CONFIG_UTILS_H_
