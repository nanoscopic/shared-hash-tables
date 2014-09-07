#ifndef HASH_H
#define HASH_H
#include<stdlib.h>
#include<memory.h>
#include<stdint.h>

uint32_t hashstr( const char *str, uint32_t seed = 0 );
//int hashstr( const char *str, unsigned long offset = 0 );
long gettot();
void setinc( int newinc );
void cleartot();
//void scrambleit();
//void printscramble();

template <class T>
struct entry {
  int owner;
  T index;
  uint16_t ver;
};

template <class T>
class pageman;

template <class T>
class page {
private:
  int offset;
  uint32_t curid;
  entry<T> set[10000];
  int stored;
public:
  int newid() { return this->curid++; }
  uint32_t newoffset();
  page();
  int store( int id, char *name, uint32_t offset, T value );
  T fetch( int id, char *name, uint32_t offset  );
  int full();
};

struct storedstr {
  storedstr *next;
  char *str;
};

template <class T>
class hash {
  int id; // id used within the page
  int offset;
  hash<T> *next;
public:
  char mypage; // page used by this hash
  storedstr *head;
  int numstored;
  hash( char pagenum, page<T> *pageob );
  hash( char pagenum, page<T> *pageob, int id, int32_t offset );
  int store( pageman<T> *man, page<T> *pageob, char *name, T value );
  T fetch( pageman<T> *man, page<T> *pageob, char *name );
};

template <class T>
class pageman {
private:
  page<T> *pages[255];
  int curpage;
public:
  pageman();
  ~pageman();
  hash<T> *newhash(); // create a new hash
  page<T> *getpage();
  page<T> *getpage( char pagenum );
  int store( hash<T> *hashob, char *name, T value );
  T fetch( hash<T> *hashob, char *name );
};

// implementation

#include<stdio.h> 
#include"hash.h"
#include<time.h>
//#define DEBUG
//http://markknowsnothing.com/cgi-bin/primes.cgi

/*
Usage:
pageman *man = new pageman;
hash *ob = man->newhash();
man->store( ob, "test", 10 );
int val = man->fetch( ob, "test" );
*/

uint32_t hashstr( const char *str, uint32_t seed ) {
  uint32_t sum = seed;//4,294,967,295 / 257 = ... ( nearest prime )
  const char *pos = str;
  while( 1 ) {
    if( ! *pos ) break;
    sum += *pos;
    if( sum > 16711661 ) sum -= 16711661; // prime - prevent overflow - 16,711,678 is max
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
  return sum;
}

template <class T>
hash<T>::hash( char pagenum, page<T> *pageob, int id, int32_t offset ) {
  #ifdef DEBUG
  printf("init hash\n");
  #endif
  this->next = 0;
  this->mypage = pagenum;
  this->id = id;
  this->offset = offset;
  this->head = 0;
  this->numstored = 0;
}

template <class T>
hash<T>::hash( char pagenum, page<T> *pageob ) {
  #ifdef DEBUG
  printf("init hash\n");
  #endif
  this->next = 0;
  this->mypage = pagenum;
  this->id = pageob->newid();
  this->offset = pageob->newoffset();
  this->head = 0;
  this->numstored = 0;
}

long tottry = 0;
int inc = 6793;
void cleartot() { tottry = 0;}
long gettot() { return tottry; }
void setinc( int newinc ) { inc = newinc; }

template <class T>
int hash<T>::store( pageman<T> *man, page<T> *pageob, char *name, T value ) {
  #ifdef DEBUG
  printf("store\n");
  #endif
  
  int res;
  uint32_t offset = this->offset;
  
  // first try
  res = pageob->store( this->id, name, offset, value );
  if( res != -1 ) {
    return res;
  }
  int trys = 1;
  
  trys++;
  tottry++;
  
  // hand off to the next 'sub' hash if there is one
  if( this->next ) return this->next->store( man, pageob, name, value );
  
  int id = pageob->newid();
  offset = pageob->newoffset();
  
  while( trys <= 200 ) { // find the offset that works
    res = pageob->store( id, name, offset, value );
    if( res != -1 ) {
      // success; create new next object
      this->next = new hash( this->mypage, pageob, id, offset );
      return res;
    }
    //this->nextpage( pageman *man );
    offset = pageob->newoffset();
    trys++;
    tottry++;
  }
  return -1;
}

//void hash::nextpage() {
  // call something in pageman to indicate a page was skipped over
  // so many skips of a page and that page will be abandoned
  
  // go through all the currently stored strings; removing them from
  // the current page
  
  // then increment to the next page
  // then re-store all of the various stored strings
//}

template <class T>
T hash<T>::fetch( pageman<T> *man, page<T> *pageob, char *name ) {
  // attempt initial fetch
  int res = pageob->fetch( this->id, name, this->offset );
  if( res != -1 ) { return res; }
  
  if( this->next ) {
    return this->next->fetch( man, pageob, name );
  }
  
  return -1;
}

template <class T>
int pageman<T>::store( hash<T> *hashob, char *name, T value ) {
  #ifdef DEBUG
  printf("\nPagenum:%i\n",hashob->mypage );
  #endif
  page<T> *pageob = this->getpage( hashob->mypage );
  return hashob->store( this, pageob, name, value );
}

template <class T>
T pageman<T>::fetch( hash<T> *hashob, char *name ) {
  return hashob->fetch( this, this->getpage( hashob->mypage ), name );
}

template <class T>
pageman<T>::pageman() {
  this->curpage = 0;
  memset( this->pages, 0, 255*sizeof( page<T> * ) );
  this->pages[0] = new page<T>;
}

template <class T>
pageman<T>::~pageman() {
  delete this->pages[0];
}

template <class T>
page<T> *pageman<T>::getpage( char pagenum ) {
  page<T> *ref = this->pages[ (int)pagenum ];
  if( !ref ) {
    return this->pages[ (int)pagenum ] = new page<T>;
  }
  return ref;
}

template <class T>
page<T> *pageman<T>::getpage() {
  #ifdef DEBUG
  printf("Attempt to fetch curpage\n");
  printf("%i\n",this->curpage);
  #endif
  return this->getpage( this->curpage );
}

template <class T>
int page<T>::full() {
  return ( this->stored >= 7000 );
}

template <class T>
hash<T> *pageman<T>::newhash() {
  #ifdef DEBUG
  printf("Attempt to create hash\n");
  #endif
  page<T> *pageob = this->getpage();
  if( pageob->full() ) {
    this->curpage++;
    pageob = this->getpage();
  }
  return new hash<T>( this->curpage, pageob );
}

template <class T>
page<T>::page() {
  this->stored = 0;
  this->offset = 0;
  curid = 1;
  memset( set, 0, 10000*sizeof( entry<T> ) );
}

int primes[10] = { 0, 0, 104717, 104723, 104729, 92707 };

template <class T>
int page<T>::store( int id, char *name, uint32_t offset, T value ) {
  uint32_t hval = hashstr( name, offset );
  
  uint16_t ver = hval % 65521;
  int setid = hval % 9973;
  //int trys = 1;
  //while( trys <= 4 ) {
    entry<T> *eptr = &this->set[ setid ];
    if( !eptr->owner ) {
      eptr->owner = id;
      eptr->index = value;
      eptr->ver = ver;
      this->stored++;
      return 0; // success { //&& ( eptr->owner != id ) ) {
    }
    //trys++;
    //tottry++;
    //offset += inc;
    //setid = hashstr( name, offset ) % 9973;
  //}
  return -1;
}

template <class T>
uint32_t page<T>::newoffset() {//3,4=9007,6=7062,8=9545,11=8922
  // smaller than 16,711,661
  
  this->offset += 46817;//53467335;//148141639;
  //this->offset = this->offset % 16744379;
  return this->offset;
}

template <class T>
T page<T>::fetch( int id, char *name, uint32_t offset ) {
  uint32_t hval = hashstr( name, offset );
  int setid = hval % 9973;
  uint16_t ver = hval % 65521;
  #ifdef DEBUG
  printf("Fetching at pos %i with hash id %i\n", setid, id);
  #endif
  
  int trys = 1;
  while( trys <= 4 ) {
    entry<T> *eptr = &this->set[ setid ];
    if( eptr->owner == id && eptr->ver == ver ) { // success
      return eptr->index;
    }
    trys++;
    offset += inc;
    setid = hashstr( name, offset ) % 9973;
  }
  #ifdef DEBUG
  printf("Failed attempting to fetch item %i\n",setid);
  #endif
  return -1;
  
}

#endif



