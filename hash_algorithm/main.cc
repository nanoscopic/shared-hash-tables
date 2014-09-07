#include<stdio.h>
#include"hash.h"
#include<memory.h>
#include<stdlib.h>
#include<time.h>

int main( int argc, char *argv[] ) {
  //printf("xml %i\n", hash( "x" ) );
  //printf("value %i\n", hash( "value" ) );
  
  #define TOTT 9973
  char index[ TOTT ];
  memset( index, 0, TOTT );
  int k = 1;
  int tottrys = 0;
  char word[20];
  srand( time(0) );
  
  for( int i=1;i<((float)TOTT * 0.95);i++ ) {
    //printf("xml %i\n", hashstr( "x", i ) );
    int trys = 0;
    for( int j=0;j<8;j++ ) {
      word[j] = (rand()%26)+'a';
    }
    word[8] = 0;
    //printf("%s\n",word);
    while( trys < 500 ) {
      
      int hash = hashstr( word, k );
      if( !index[ hash ] ) {
        index[ hash ] = 1;
        break;
      }
      k++;
      trys++;
      tottrys++;
    }
    k++;
    if( trys == 500 ) {
      printf("Error");
      break;
    }
    if( !(i%5) ) printf("%i,%i\n",i/65,tottrys);
  }
  //printf("Total Trys: %i\n", tottrys );
}