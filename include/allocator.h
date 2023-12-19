#ifndef INCLUDE_ARENA_ALLOCATOR_H
#define INCLUDE_ARENA_ALLOCATOR_H

#include "instruction.h"
#include "vm.h"

#include <iostream>
#include <memory>

class ArenaAllocator {

public:
  ArenaAllocator(size_t capacity = vm_numbers::VM_DEFAULT_ARENA);
  ~ArenaAllocator();

  size_t getCapacity() const { return capacity_; }

  size_t getSize() const { return size_; }

  void *allocate(size_t size);

private:
  uint8_t *heap_{nullptr};
  uint8_t *next_allocation_{nullptr};

  size_t size_{0};
  size_t capacity_{0};
};

#endif // INCLUDE_ARENA_ALLOCATOR_H
