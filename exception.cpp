//
// Created by XiaoLi on 25-8-16.
//

#include "exception.h"

namespace lvm
{
    VMException::VMException(std::string message): std::runtime_error(message)
    {
    }
}
