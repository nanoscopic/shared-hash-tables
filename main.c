// Copyright (C) 2014 David Helkowski
// License GNU AGPLv3

#include"shared_hashing.h"
#include"sh_shared.h"
#include<stdio.h>
#include<stdlib.h>

typedef sh_page_manager pageman;
typedef sh_bighash bighash;
#define pageman( a, ... ) sh_page_manager__ ## a( __VA_ARGS__ )

int main( int argc, char *argv[] ) {
  //printf("Size of page %li\n", sizeof( ipage ) );
  
  pageman *man = pageman( new );
  bighash *ob  = pageman( new_bighash, man, 1200000 );
  //hash *ob = man->new_hash();
  
  char str[20];
  char *astr;
  srand(0);
  u4 i;
  char *keys = (char *) malloc( 22000000 );
  u4 j;
  for( j=0;j<(22000000);j+=6 ) {
      //keys[ j ] = rand();// % 256;
      
      u4 j2 = j;
      u1 b1 = j2 % 256;
      j2 -= b1; j2 /= 256;
      u1 b2 = j2 % 256;
      j2 -= b2; j2 /= 256;
      u1 b3 = j2 % 256;
      j2 -= b3; j2 /= 256;
      keys[ j ] = b1;
      keys[ j + 1 ] = b2;
      keys[ j + 2 ] = b3;
      keys[ j + 3 ] = j2;
      keys[ j + 4 ] = b1 + 3;
      //keys[ j + 5 ] = b2 + 7;
  }
  
  for( i=0;i<1000000;i++ ) {
      //astr = (char *) malloc( 8 );//new char[14];
      astr = &keys[ 6*i ];
      //if( !astr ) {
      //    printf("Cannot allocate\n");
      //    exit(1);
      //}
      
      //sprintf( astr, "%i", i );
      //int len = strlen( astr );
      int res = pageman( store_bighash, man, ob, astr, 5, i );
      if( res == -1 ) {
          printf("Fail to store %i\n", i );
      }
  }
  srand(0);
  for( i=0;i<1000000;i++ ) {
      u1 j;
      
      //for( j=0;j<8;j++ ) {
      //    str[ j ] = rand();// % 256;
      //}
      char *xstr = &keys[ 6*i ];
      
      //sprintf( str, "%i", i );
      //int len = strlen( str );
      
      u4 *valp = (u4 *) pageman( fetch_bighash, man, ob, xstr, 5 );
      if( valp ) {
          uint32_t val = *valp;
          if( val != i ) {
              printf("Mismatch %i - !%i\n", i, val );
          }
      }
      else {
          printf("Could not fetch %i\n", i );
      }
  }
  
  //pageman( debug, man );
  //ob->debug();
  
  //man->store( ob, "test", 4, 1000 );
  
  //printf("test\n");
  
  //int val = man->fetch( ob, "test", 4 );
  //printf("%i\n",val);
  
  return 0;
}