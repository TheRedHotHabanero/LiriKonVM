#ifndef INCLUDE_ARRAY_H
#define INCLUDE_ARRAY_H

#include <cstdlib>
#include <stdint.h>

class Array final
{
public:
    Array *newArray(size_t size, void *memory);
    Array() = default;
    ~Array() = default;

    int getValue(int index);
    void setValue(int index, int value);

    Array(size_t size, void *memory)
    {
        elem_ = size;
        pointer_ = memory;
    };

private:
    uint32_t elem_;
    uint32_t word_;

    size_t getOffset(int index);

    void *pointer_;
};

#endif // INCLUDE_DECODER_HPP
