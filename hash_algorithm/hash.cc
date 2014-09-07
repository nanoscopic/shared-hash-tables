#include"hash.h"
#include<Stdlib.h>
#include<stdint.h>
/*char scramble[] = {11,7,1,17,23,16,21,19,2,4,0,8,15,10,6,18,25,12,24,3,9,20,22,5,14,13,26,27,28,29};
int hashstr( const char *str, int sum ) {
  //for( int i=0;i<len;i++ ) {
  int i = 0;
  while( 1 ) {
    if( ! *(str + i) ) break;
    char let = ( *( str + i ) - 'a' + i ) % 29;
    sum += scramble[ let ];
    sum *= 29;
    i++;
  }
  while( i < 5 ) {
    sum += scramble[ i ];
    sum *= 29;
    i++;
  }
  sum = sum / 29;
  sum = sum % 10000;
  //}
  return sum;
}*/

// used http://markknowsnothing.com/cgi-bin/primes.cgi
// scramble via rng
/*int hashstr( const char *str, int seed ) {
  unsigned long sum = seed;//4,294,967,295 / 29 = 148,102,320 - 148,102,303 ( nearest prime )
  while( 1 ) {
    if( ! *str ) break;
    sum += *str - 'a' + 1;
    if( sum > 148102303 ) sum -= 148102303; // prevent overflow
    sum *= 29;
    str++;
  }
  //return sum % 9973; // nearest prime < 10k
  //return sum % 29989;
  return sum % 79999;
}*/

struct entry {
  unsigned id : 13;
  unsigned value : 11;
};

uint32_t hashstr( const char *str, uint32_t seed ) {
  uint32_t sum = seed;//4,294,967,295 / 257 = 148,102,320 - 148,102,303 ( nearest prime )
  const char *pos = str;
  while( 1 ) {
    if( ! *pos ) break;
    sum += *pos + 1;
    if( sum > 16711661 ) sum -= 16711661; // prime - prevent overflow - 16711678 is max
    sum *= 257; // prime
    pos++;
  }
  if( (pos - str) < 6 ) { // ensure seed is scrambled some
    for( char i=0;i<( 6 - (pos - str) );i++ ) {
      sum += 137; // prime
      if( sum > 16711661 ) sum -= 16711661; // prime - prevent overflow - 16711678 is max
      sum *= 257; // prime
    }
  }
  return sum % 9973; // nearest prime < 10k
  //return sum % 29989;
  //return sum % 79999; // prime
  //return sum % 65521; ( we need this as well for the verify )
  //return sum % 251;
}