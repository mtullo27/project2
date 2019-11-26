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
  int LRU[sets[way];
  for(int i = 0; i<sets; i++){
		for(int j = 0; j<ways; j++){
			LRU[i][j] = -1;
			cache[i][j] = -1;
		}
	}
	for(int i = 0; i<addr.size(); i++){
		long long tag = addr>>5;
		int lruP = 0;
		int lruQ = 0;
		bool isSet = false;
		int lru = LRU[0][0];
		for(int p = 0; p<sets; p++){
			for(int q = 0; q<way; q++){
				if(cache[p][q] == tag){
					hit++;
					isSet = true;
					LRU[p][q] = i;
				}
				if(cache[p][q] == -1){
					cache[p][q] = tag;
					LRU[p][q] = i;
					isSet = true;
					p = sets;
					q = way;
				}
				if(LRU[p][q]<lru){
					lruP = p;
					lruQ = q;
					lru = LRU[p][q];
				}
			}
		}
		if(!(isSet)){
			cache[lruP][lruQ] = addr[i];
			LRU[lruP][lruQ] = i;
		}
	}
  return hit;
}

int Fully_Associative(vector<long long> addr1){
  int blocks = 512;
  long long cache[blocks];
  int temperature[blocks];
  int hit = 0;
  int volume = 0;
  vector<long long> addr = addr1;
  for(int i = 0; i<blocks; i++){
    cache[i] = -1;
    temperature[i] = -1;
  }
  for(int i = 0; i<addr.size(); i++){
    addr[i] = addr[i]>>5;
    if(volume >= blocks){
      int min = temperature[0];
      int minI = 0;
      bool isHit = true;
      for(int p = 1; p<blocks; p++){
	if(temperature[p]<=min){
	  min = temperature[p];
	  minI = p;
	}
	if(cache[p] == addr[i]){
	  hit++;
	  isHit = false;
	  temperature[p]=i;
	}
      }
      if(isHit){
	cache[minI] = addr[i];
	temperature[minI] = i;
      }
    }
    if(volume < blocks){
      bool isHit = false;
      for(int j = 0; j<=volume; j++){
	if(cache[j] == addr[i]){
	  hit++;
	  isHit = true;
	  temperature[j] = i;
	}
      }
      if(!(isHit)){
	cache[volume] = addr[i];
	temperature[volume] = i;
	volume++;
      }
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
  //	for(int i = 0; i<4; i++){
  //	  cout << set_associative(address, sets[i]) << "," << address.size() << "; ";
  //	}
  //	cout << endl;
  cout << Fully_Associative(address) << "," << address.size() << endl;
  return 0;
}

