#include<stdint.h>
uint32_t hashstr( const char *str, uint32_t seed = 0 );
/*
class hash {
  char page; // page used by this hash
  int id; // id used within the page
};

class pageman {
private:
  page *pages[255];
  int curpage;
public:
  pageman() { curpage = 0; }
  hash *newhash(); // create a new hash
};

struct entry {
  int owner;
  char index;
};

class page {
private:
  int offset;
  int curid;
  entry set[10000];
public:
  page() { this->offset = 0; curid = 0; };
}*/