#include "p2.h"

using namespace std;

//each cache line has size 32 bytes 
int direct(vector<long long> addr, vector<string> func, int size){
	long long cache[size];
	int valid[size];
	int hit = 0;
	for(int i = 0; i<size; i++){
		cache[i] = -1;
		valid[i] = 0;
	}
	for(int i = 0; i<addr.size(); i++){
		int block_addr = addr[i]/32;
		int index = addr[i]%size;
		
		if(valid[index] == 0){
			valid[index] = 1;
			cache[index] = block_addr;
		}
		if(valid[index] == 1){
			if(cache[index] == block_addr)
				hit++;
			else
				cache[index] = block_addr;
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
		cout << direct(address, strld, sizes[i]) << ","<< address.size() << "; ";
	}
	cout << endl;
  return 0;
}

