/* Testing Code */

#define TMin (-2147483647-1)
#define TMax 2147483647

int test_bitAnd(int x, int y)
{
  return x&y;
}
int test_bitMask(int highbit, int lowbit)
{
  int result = 0;
  int i;
  for (i = lowbit; i <= highbit; i++)
    result |= 1 << i;
  return result;
}
int test_bitXor(int x, int y)
{
  return x^y;
}
int test_getByte(int x, int n)
{
  union {
    int word;
    unsigned char bytes[4];
  } u;
  int test = 1;
  int littleEndian = (int) *(char *) &test;
  u.word = x;
  return littleEndian ? (unsigned) u.bytes[n] : (unsigned) u.bytes[3-n];
}



int test_isNonZero(int x)
{
  return x!=0;
}
int test_isPower2(int x) {
  int i;
  for (i = 0; i < 31; i++) {
    if (x == 1<<i)
      return 1;
  }
  return 0;
}
int test_abs(int x) {
  return (x < 0) ? -x : x;
}
int test_addOK(int x, int y)
{
  int sum = x+y;
  return !(x < 0 && y < 0 && sum >= 0) && !(x > 0 && y > 0 && sum <= 0);
}
int test_copyLSB(int x)
{
  return (x & 0x1) ? -1 : 0;
}
int test_isLessOrEqual(int x, int y)
{
  return x <= y;
}
int test_multFiveEights(int x)
{
  return (x*5)/8;
}
int test_sm2tc(int x) {
  int sign = x < 0;
  int mag = x & TMax;
  return sign ? -mag : mag;
}
int test_tmax(void) {
  return TMax;
}
