#ifndef INCLUDE_FRAME_H
#define INCLUDE_FRAME_H

#include <cstddef>
#include <vector>
#include <cstdint>

#include "interpreter.h"

class Frame final
{
public:
    Frame(size_t size = vm_numbers::VM_DEFAULT_FRAME)
    {
        uint8_t *mem = new uint8_t[size];
        start_frame_ = reinterpret_cast<int *>(mem);
        current_frame_ = start_frame_ + bytesForRegs();
        end_ = start_frame_ + size;
    }

    ~Frame()
    {
        delete[] reinterpret_cast<uint8_t*>(start_frame_);
    }

    Frame *getCurrentFramePtr()
    {
        return state_frame_;
    }

    void newFrame();

    void deleteFrameFromBack();

    std::vector<Frame *> *frames()
    {
        return &frames_;
    }

    void *regPtr(size_t reg_id);

    void *freeMemory(size_t size);

    size_t bytesForRegs() const;

private:
    Frame *state_frame_;
    std::vector<Frame *> frames_;
    int *start_frame_;
    int *current_frame_;
    int *end_;
};

#endif // INCLUDE_DECODER_HPP