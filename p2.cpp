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
  int blocks = 512;
  long long cache[blocks][way];
  int hit = 0;
  int LRU[blocks][way];
  for(int i = 0; i < blocks; i++){
    for(int j = 0; j < way; j++){
			cache[i][j] = (-1);
      LRU[i][j] = (-1);
    }
	}
  for(int i = 0; i<addr.size(); i++){
		addr[i] = addr[i]>>5;
    int tag = addr[i]/blocks;
		int set = addr[i]%blocks;
		int full = 0;
		for(int j = 0; j<way; j++){
			if(full!=1){
				if(cache[set][j] !=-1){
					if(cache[set][j] == tag){
						hit++;
						LRU[set][j] = i;
						full = 1;
					}
				}
			}
			else if(cache[set][j] == -1){
				cache[set][j] = tag;
				LRU[set][j] = i;
				full = 1;
			}
			
			else if(full == 1){
				int minJ = 0;
				int min = LRU[set][0];
				for(int x = 1; x<set; set++){
					if(LRU[set][x]<min){
						min = LRU[set][x];
						minJ = x;
					}
				}
				cache[set][minJ] = tag;
				LRU[set][minJ] = i;
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
	for(int i = 0; i<4; i++){
	  cout << set_associative(address, sizes[i]) << "," << address.size() << "; ";
	}
	cout << endl;
  return 0;
}

