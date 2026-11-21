#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

class File {
public:
    File();
    ~File();

    std::vector<unsigned char> &getElf();
    void setElf(const std::string& filepath);

private:
    std::vector<unsigned char> open_file;
};

#endif
