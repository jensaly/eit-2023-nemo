#ifndef EIT_FILEHANDLER_H
#define EIT_FILEHANDLER_H

#include "ferry.h"

struct Yard;

struct FileHandler {
    FileHandler();
    void Write(Ferry& f, Yard& yard);
};

#endif //EIT_FILEHANDLER_H
