#ifndef INCLUDE_FRAME_H
#define INCLUDE_FRAME_H

#include <cstddef>
#include <cstdint>
#include <vector>

#include "interpreter.h"

class Frame final {
public:
  Frame *prev_frame_;

  Frame(size_t size = vm_numbers::VM_DEFAULT_FRAME,
        Frame *prev_frame = nullptr) {
    uint8_t *mem = new uint8_t[size];
    start_frame_ = reinterpret_cast<int *>(mem);
    current_frame_ = start_frame_ + bytesForRegs();
    end_ = start_frame_ + size;
    prev_frame_ = prev_frame;
    pc_ = 0;
  }

  ~Frame() { delete[] reinterpret_cast<uint8_t *>(start_frame_); }

  void newFrame(Frame *prev_frame = nullptr);

  void *regPtr(size_t reg_id);

  void *freeMemory(size_t size);

  size_t bytesForRegs() const;

  int getPC() { return pc_; }

  void setPC(int new_pc) {
    pc_ = new_pc;
    return;
  }

  int *getStartMem() { return start_frame_; }

private:
  int *start_frame_;
  int *current_frame_;
  int *end_;
  int pc_;
};

#endif // INCLUDE_DECODER_HPP
