/*
 *
 * AXI_mem_access.c
 *
 */

#include "AXI_mem_access.h"


unsigned int axi_read(const unsigned int address)
{
  return *((unsigned int*)address);
}

void axi_write(const unsigned int address, const unsigned int data)
{
  *((unsigned int*)address) = data;
}

