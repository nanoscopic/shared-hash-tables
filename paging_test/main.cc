#include"hash.h"
#include<stdio.h>
typedef pageman<uint16_t> ipageman;
typedef hash<uint16_t> ihash;

int main( int argc, char *argv[] ) {
  ipageman *man = new ipageman;
  ihash *ob = man->newhash();
  man->store( ob, "test", 1000 );
  int val = man->fetch( ob, "test" );
  printf("%i\n",val);
  return 0;
}