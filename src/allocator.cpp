#include "allocator.h"

#include <iostream>

ArenaAllocator::ArenaAllocator(size_t capacity) : capacity_(capacity) {
  if (capacity <= 0) {
    std::cerr << "Invalid heap size, can not allocate heap" << std::endl;
    return;
  }

  heap_ = new uint8_t[capacity_];

  next_allocation_ = heap_;
}

ArenaAllocator::~ArenaAllocator() { delete[] heap_; }

void *ArenaAllocator::allocate(size_t size) {
  if (size > capacity_ - size_) {
    std::cerr << "Failed to allocate" << std::endl;
    return nullptr;
  }

  void *alloc_ptr = next_allocation_;

  next_allocation_ += size;
  size_ += size;

  return alloc_ptr;
}
