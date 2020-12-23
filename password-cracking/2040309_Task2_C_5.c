#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int count = 0; 
struct crypt_data *crypmem1, *crypmem2;
pthread_t first_thread, second_thread;

void substr(char *dest, char *src, int start, int length)
{
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

void *kernel_function_1(void *salt_and_encrypted)
{
  int x, y, z;
  char salt[7];
  char plain[7];
  char *enc;

  substr(salt, salt_and_encrypted, 0, 6);

  for (x = 'A'; x <= 'M'; x++)
  {
    for (y = 'A'; y <= 'Z'; y++)
    {
      for (z = 0; z <= 99; z++)
      {
        sprintf(plain, "%c%c%02d", x, y, z);
        enc = (char *)crypt_r(plain, salt, crypmem1);
        count++;
        if (strcmp(salt_and_encrypted, enc) == 0)
        {
          printf("Password Found!!!!\n");
          printf("#%-8d%s %s\n", count, plain, enc);
          pthread_cancel(first_thread);
          pthread_cancel(second_thread);
          return 0;
        } else {
          printf("Bruteforcing... please wait.... \n");
          printf("#%-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
  pthread_exit(NULL);
}

void *kernel_function_2(void *salt_and_encrypted)
{
  int x, y, z;
  char salt[7];
  char plain[7];
  char *enc;

  substr(salt, salt_and_encrypted, 0, 6);

  for (x = 'N'; x <= 'Z'; x++)
  {
    for (y = 'A'; y <= 'Z'; y++)
    {
      for (z = 0; z <= 99; z++)
      {
        sprintf(plain, "%c%c%02d", x, y, z);
        enc = (char *)crypt_r(plain, salt, crypmem2);
        count++;
        if (strcmp(salt_and_encrypted, enc) == 0)
        {
          printf("Password Found!!!!\n");
          printf("#%-8d%s %s\n", count, plain, enc);
          pthread_cancel(first_thread);
          pthread_cancel(second_thread);
          return 0;
        } else {
          printf("Bruteforcing... please wait.... \n");
          printf("#%-8d%s %s\n", count, plain, enc);
        }
      }
    }
  }
}

int time_difference(struct timespec *begin, struct timespec *end, long long int *time_taken) {
  long long int ds = end -> tv_sec - begin -> tv_sec;
  long long int dn = end -> tv_nsec - begin -> tv_nsec;

  if (dn < 0) {
    ds --;
    dn += 1000000000;
  }
  *time_taken = ds * 1000000000 + dn;
  return !(*time_taken > 0);
}

int main(int argc, char *argv[])
{
  struct timespec begin, end;
  long long int time_taken;
  char hashed_password[] = "$6$AS$Q9y4aEAHPnUDfJx2HsLI3TFfWqFlY..ncb767oAexfxdwV7TzH9yA1WxJdvdD/Cx0Gg2sqoK/a/bQ660wbrkF/";

  // Thread definition
  int thread_status;

  crypmem1 = (struct crypt_data *)malloc(sizeof(struct crypt_data));
  crypmem1->initialized = 0;
 
  crypmem2 = (struct crypt_data *)malloc(sizeof(struct crypt_data));
  crypmem2->initialized = 0;

  clock_gettime(CLOCK_MONOTONIC, &begin);

  thread_status = pthread_create(&first_thread, NULL, kernel_function_1, (void *) hashed_password);
  if (thread_status) printf("Failed to create: Thread 1");

  thread_status = pthread_create(&second_thread, NULL, kernel_function_2, (void *) hashed_password);
  if (thread_status) printf("Failed to create: Thread 2");

  pthread_join(first_thread, NULL);
  pthread_join(second_thread, NULL);

  clock_gettime(CLOCK_MONOTONIC, &end);
  printf("%d solutions explored\n", count);
  time_difference(&begin, &end, &time_taken);
  printf("Total decryption took: %lldns\n", time_taken);
  return 0;
}
