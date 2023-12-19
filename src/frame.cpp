#include "frame.h"
#include "vm.h"

#include <iostream>

void Frame::newFrame()
{
    Frame *new_frame = new Frame();
    frames_.push_back(new_frame);
    state_frame_ = new_frame;
}

void Frame::deleteFrameFromBack()
{
    delete state_frame_;
    frames_.pop_back();
    if (frames_.size() == 0)
    {
        state_frame_ = nullptr;
    }
    else
    {
        state_frame_ = frames_[frames_.size() - 1];
    }
}

void *Frame::regPtr(size_t reg_id)
{
    return reinterpret_cast<void *>(start_frame_ + (reg_id * sizeof(int)));
}

void *Frame::freeMemory(size_t size)
{
    void *mem = reinterpret_cast<void *>(current_frame_);
    current_frame_ += size;
    return mem;
}

size_t Frame::bytesForRegs() const
{
    return sizeof(int) * (vm_numbers::REG_NUM + vm_numbers::REG_NUM);
}