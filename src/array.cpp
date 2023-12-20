#include "array.h"

Array *Array::newArray(size_t size, void *memory) {
  Array *arr = new Array(size, memory);
  return arr;
}

size_t Array::getOffset(int index) { return index * word_; }

int Array::getValue(int index) {
  return *reinterpret_cast<char *>(pointer_) + getOffset(index);
}

void Array::setValue(int index, int value) {
  int *index_pointer = reinterpret_cast<int *>(pointer_) + getOffset(index);
  *index_pointer = value;
}