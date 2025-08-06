#ifndef UI_H
#define UI_H

#include <vector>

class Frontend
{
public:
    void init_frontend();

private:
    std::vector<unsigned char> open_file;
};

#endif
