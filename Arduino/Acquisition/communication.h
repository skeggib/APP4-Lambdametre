#ifndef HEADER_COMMUNICATION
#define HEADER_COMMUNICATION

float readSerialFloat()
{
  float f;
  for (int i = 0; i < 4; ++i)
  {
    int b;
    do
    {
      b = Serial.read();
    } while (b == -1);
    ((char*)&f)[i] = b;
  }
  return f;
}

#endif
