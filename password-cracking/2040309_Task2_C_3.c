#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>

int count = 0;

void substr(char *dest, char *src, int start, int length)
{
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void crack(char *salt_and_encrypted)
{
  int i, x, y, z;
  char salt[7];
  char plain[7];
  char *enc;
  substr(salt, salt_and_encrypted, 0, 6);
  for (i = 'A'; i <= 'Z'; i++)
  {
    for (x = 'A'; x <= 'Z'; x++)
    {
      for (y = 'A'; y <= 'Z'; y++)
      {
        for (z = 0; z <= 99; z++)
        {
          sprintf(plain, "%c%c%c%02d", i, x, y, z);
          enc = (char *)crypt(plain, salt);
          count++;
          if (strcmp(salt_and_encrypted, enc) == 0)
          {
            printf("#%-8d%s %s\n", count, plain, enc);
            return;
          }
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  crack("$6$AS$x/61I1fxKhcaR4rBjWoRLflT0NNyHnSQjmRrT7yub2rvdSMqGxTkz049LJT0KS1mZu95IdHQpRDxgxlnz2cnF0");
  printf("%d solutions explored\n", count);
  return 0;
}
