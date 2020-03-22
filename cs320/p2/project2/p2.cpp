#include "p2.h"

using namespace std;

//each cache line has size 32 bytes 
int direct(vector<long long> addr, int size){
  int blocks = size/32;
  long long cache[blocks];
  int valid[blocks];
  int hit = 0;
  for(int i = 0; i<size; i++){
    cache[i] = -1;
    valid[i] = 0;
  }
  for(int i = 0; i<addr.size(); i++){
    long long addrB = addr[i]>>5;
    int index = addrB%(blocks);
    int tag = addrB/blocks;
    if(valid[index] == 1){
      if(cache[index] == tag)
	hit++;
      else
	cache[index] = tag;
    }	
    if(valid[index] == 0){
      valid[index] = 1;
      cache[index] = tag;
    }
  }
  return hit;
}

int set_associative(vector<long long>addr, int way){
  int sets = 512/way;
  long long cache[sets][way];
  int hit = 0;
  int LRU[sets][way];
  int full[sets];
  int volume = 0;
  for(int i = 0; i<sets; i++){
    for(int j = 0; j<way; j++){
      LRU[i][j] = -1;
      cache[i][j] = -1;
    }
  }
  for(int i = 0; i<addr.size(); i++){
    long long temp = addr[i]>>5;
    int tag = temp/sets;
    int set = temp%sets;
    bool cached = false;
    for(int j = 0; j<way; j++){
      if(cache[set][j] == -1){
	cache[set][j] = tag;
	LRU[sets][j] = i;
	cached = true;
	j = way;
      }
      else if(cache[set][j] == tag){
	hit++;
	LRU[set][j] = i;
	cached = true;
      }
    }
    if(!cached){
      int lru = LRU[set][0];
      int lruIndex = 0;
      for(int j = 0; j<way; j++){
	if(LRU[set][j] < lru){
	  lruIndex = j;
	}
      }
      LRU[set][lruIndex] = i;
      cache[set][lruIndex] = tag;
    }
  }
  return hit;
}

int Fully_Associative(vector<long long> addr1){
  int blocks = 512;
  long long cache[blocks];
  int LRU[blocks];
  int hit = 0;
  vector<long long> addr = addr1;
  for(int i = 0; i<blocks; i++){
    cache[i] = -1;
    LRU[i] = -1;
  }
  for(int i = 0; i<addr.size(); i++){
    addr[i] = addr[i]>>5;
    bool cached = false;
    for(int j = 0; j<blocks; j++){
      if(cache[j] == -1){
	cache[j] = addr[i];
	LRU[j] = i;
	cached = true;
	j = blocks;
      }
      else if(cache[j] == addr[i]){
	hit++;
	LRU[j] = i;
	cached = true;
      }
    }
    if(!cached){
      int lru = LRU[0];
      int lruI = 0;
      for(int j = 0; j<blocks; j++){
	if(LRU[j] < lru){
	  lru = LRU[j];
	  lruI = j;
	}
      }
      cache[lruI] = addr[i];
      LRU[lruI] = i;
    }
  }
  return hit;
}
int write_miss(vector<long long>addr, vector<string> behave, int way){
  int sets = 512/way;
  long long cache[sets][way];
  int hit = 0;
  int LRU[sets][way];
  int full[sets];
  int volume = 0;
  for(int i = 0; i<sets; i++){
    for(int j = 0; j<way; j++){
      LRU[i][j] = -1;
      cache[i][j] = -1;
    }
  }
  for(int i = 0; i<addr.size(); i++){
    long long temp = addr[i]>>5;
    int tag = temp/sets;
    int set = temp%sets;
    bool cached = false;
    for(int j = 0; j<way; j++){
      if(cache[set][j] == -1){
	if(behave[i] == "L"){
	  cache[set][j] = tag;
	  LRU[sets][j] = i;
	  cached = true;
	  j = way;
	}
      }
      else if(cache[set][j] == tag){
	hit++;
	LRU[set][j] = i;
	cached = true;
      }
    }
    if(behave[i] == "L" && !cached){
      int lru = LRU[set][0];
      int lruIndex = 0;
      for(int j = 0; j<way; j++){
	if(LRU[set][j] < lru){
	  lruIndex = j;
	}
      }
      LRU[set][lruIndex] = i;
      cache[set][lruIndex] = tag;
    }
    return hit;
  }
  int prefetch(vector<long long>addr, int way){
    int sets = 512/way;
    long long cache[sets][way];
    int hit = 0;
    int LRU[sets][way];
    int full[sets];
    int volume = 0;
    for(int i = 0; i<sets; i++){
      for(int j = 0; j<way; j++){
	LRU[i][j] = -1;
	cache[i][j] = -1;
      }
    }
    for(int i = 0; i<addr.size()-1; i++){
      long long temp = addr[i]>>5;
      int tag = temp/sets;
      int set = temp%sets;
      long long temp2 = addr[i+1];
      int tagP = temp2/sets;
      int setP = temp2%sets;
      bool cached = false;
      bool cachedP = false;
      for(int j = 0; j<way; j++){
	if(cache[set][j] == -1 && !cached){
	  cache[set][j] = tag;
	  LRU[sets][j] = i;
	  cached = true;
	}
	else if(cache[set][j] == tag){
	  hit++;
	  LRU[set][j] = i;
	  cached = true;
	}
	if(cache[setP][j] == -1 && !cachedP){
	  cache[setP][j] = tagP;
	  LRU[setP][j] = i;
	  cachedP = true;
	}

	if(!cached){
	  int lru = LRU[set][0];
	  int lruIndex = 0;
	  for(int j = 0; j<way; j++){
	    if(LRU[set][j] < lru){
	      lruIndex = j;
	    }
	  }
	  LRU[set][lruIndex] = i;
	  cache[set][lruIndex] = tag;
	}
	if(!cachedP){
	  int lru = LRU[setP][0];
	  int lruIndex = 0;
	  for(int j = 0; j<way; j++){
	    if(LRU[setP][j] < lru){
	      lruIndex = j;
	    }
	  }
	  LRU[setP][lruIndex] = i;
	  cache[setP][lruIndex] = tag;
	}
      }
      return hit;
    }

    int main(int argc, char *argv[]) {

      // Temporary variables
      unsigned long long addr;
      string behavior;

      // Open file for reading
      ifstream infile(argv[1]);

      //storage for variables
      vector<long long> address;
      vector<string> strld;
      // The following loop will read a hexadecimal number and
      // a string each time and then output them
      while(infile >>  behavior >> hex >> addr) {
	address.push_back(addr);
	if(behavior == "L") 
	  strld.push_back("L");
	else 
	  strld.push_back("S");
      }
      //open file to write to
      ofstream cout(argv[2]);
      vector<int> sizes{1024, 4096, 16384, 32768};
      for(int i = 0; i<4; i++){
	cout << direct(address, sizes[i]) << ","<< address.size() << "; ";
      }
      cout << endl;
      vector<int>sets{2, 4, 8, 16};
      for(int i = 0; i<4; i++){
	cout << set_associative(address, sets[i]) << "," << address.size() << "; ";
      }
      cout << endl;
      cout << Fully_Associative(address) << "," << address.size() << endl;
      cout << "This does not work" << endl;
      for(int i = 0; i<4; i++){
	cout << write_miss(address, strld, sets[i]) << "," << address.size() << "; ";
      }
      cout << endl;
      for(int i = 0; i<4; i++){
	cout << prefetch(address, sets[i]) << "," << address.size() << "; ";
      }
      return 0;
    }

