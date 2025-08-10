#ifndef FILE_H
#define FILE_H

#include <vector>

class File {
public:
    File();
    ~File();

    std::vector<unsigned char> &getElf();
    void setElf();

private:
    std::vector<unsigned char> open_file;
};

#endif
