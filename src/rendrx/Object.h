#pragma once

#include <vector>
namespace rendrx {
class Object {
  public:
    virtual const std::vector<float> flatten() = 0;
};
} // namespace rendrx
