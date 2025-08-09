#ifndef FILE_H
#define FILE_H

#include <vector>

class File {
public:
    File() : open_file(100) {}

    explicit File(const int initialValue) : open_file(5, initialValue) {}

private:
    std::vector<unsigned char> open_file;
};

#endif //FILE_H
