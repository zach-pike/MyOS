#pragma once
#include "types.h"

void Zmemcpy(void *dest, void *src, size_t n) {
   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       ((char *)dest)[i] = ((char *)src)[i];
}