#ifndef TEXT_STREAM_H
#define TEXT_STREAM_H

#include <sstream>

extern std::stringstream outputBuffer;

#define printf(...)                             \
do {                                            \
    char buf[1024];                             \
    snprintf(buf, sizeof(buf), __VA_ARGS__);    \
    outputBuffer << buf;                        \
} while(0)                                      \


#endif
