#include <stdio.h>
#include "co.h"

void entry(void *arg) {
  int num=0;
  while (num--) {
    printf("%s\n", (const char *)arg);
    co_yield();
  }
}
int main() {
  co *co1 = co_start("co1", entry, "a");
  co *co2 = co_start("co2", entry, "b");
  co_wait(co1); // never returns
  co_wait(co2);
}