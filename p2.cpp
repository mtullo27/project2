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
  int blocks = (16*1024)/32;
  long long cache[blocks][way];
  int hit = 0;
  long long LRU[blocks][way];
  for(int i = 0; i < blocks; i++){
    for(int j = 0; j < way; j++){
      cache[i][j] = -1;
      LRU[i][j]=-1;
    }
  }
  for(int x = 0; x<addr.size(); x++){
    int tag = addr[x]/blocks;
    for(int i = 0; i < blocks; i++){
      for(int j = 0; j<way; j++){
	if(cache[i][j] == tag){
	  LRU[i][j] = x;
	  hit++;
	}
	else if(cache[i][j] == -1){
	  cache[i][j] = tag;
	  LRU[i][j] = x;
	}
	else{
	  int min = LRU[0][0];
	  int minI;
	  int minJ;
	  for(int y = 0; y<blocks;y++){
	    for(int q = 0; q<way; q++){
	      if(LRU[y][q]<min){
		min = LRU[y][q];
		minI = y;
		minJ = q;
	      }
	    }
	  }
	  cache[minI][minJ] = cache[i][j];
	  LRU[minI][minJ] = x;
	}
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
        vector<int>sets{2,4,8,16};
	  cout << endl;
	  for(int i = 0; i<4; i++)
	    cout << set_associative(address, sizes[i]) << "," << address.size() << "; ";
	  cout << endl;
  return 0;
}

