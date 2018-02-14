/**
 * Functions for computing the lambda
 */

#ifndef HEADER_LAMBDA
#define HEADER_LAMBDA

const double SLOT_STEP = 10. * 2.;

double getLambda(unsigned long fringeCount, unsigned long slotCount)
{
  if (slotCount == 0) return -1;
  return 2. * (SLOT_STEP * (double)slotCount) / (double)fringeCount; // longueur d'onde du laser (en µm)
}

#endif
