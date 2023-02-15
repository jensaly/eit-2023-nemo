#ifndef EIT_FILEHANDLER_H
#define EIT_FILEHANDLER_H

#include "ferry.h"

struct FileHandler {
    FileHandler();
    void Write(Ferry& f);
};

#endif //EIT_FILEHANDLER_H
