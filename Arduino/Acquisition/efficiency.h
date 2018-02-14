/**
 * Functions for computing the efficiency
 */

#ifndef HEADER_EFFICIENCY
#define HEADER_EFFICIENCY

double getEfficiency(unsigned long highCount, unsigned long lowCount)
{
  if (highCount + lowCount == 0) return -1;
  return 100. * (double)highCount / (double)(highCount + lowCount);
}

#endif
