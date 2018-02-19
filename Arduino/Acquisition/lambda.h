/**
 * Functions for computing the lambda
 */

#ifndef HEADER_LAMBDA
#define HEADER_LAMBDA

float getLambda(unsigned long fringeCount, unsigned long slotCount, float slotStep)
{
  if (slotCount == 0) return -1;
  return 2. * (slotStep * 2. * (double)slotCount) / (double)fringeCount; // longueur d'onde du laser (en µm)
}

double getStep(unsigned long fringeCount, unsigned long slotCount, float lambda)
{
  if (slotCount == 0) return -1;
  return ((double)fringeCount * lambda) / (4. * (double)slotCount);
}

#endif
