#include <iostream>
#include <iomanip>
#include <fstream>
#include <string> 
#include <stdio.h>

using namespace std;

class VMsim {
	public:
		int nextSpot;           //next spot to input address into ram
		
		int pagecount;
		unsigned ram[4];		//current pages in ram (max 4)
		unsigned long pages[16];		//total pages that are added in first 16 lines on input
		bool used[4];
		
		//initializing ram, pages to 0
		VMsim() {
			nextSpot = 0;
			pagecount = 0;
			for (int i = 0; i < 4; i++) {
				ram[i] = 0;
				used[i] = false;
			}
			for (int i = 0; i < 16; i++) {
				pages[i] = 0;
			}
		}

		//add the right address into the correct page array
		void addPages(int c) {
			pages[pagecount] = c;
			pagecount++;
		}

		//add or replace
		void addRam(int n) {
			if (!checkRam(n)) {
				
				updateSpot();
				ram[nextSpot] =  n;
				nextSpot = ++nextSpot % 4;
			}
						
		}

		//checks if the adress is already loaded in RAM
		bool checkRam(int n) {
			if (ram[0] == n){
				used[0] = true;
				return true;
			}	else if (ram[1] == n){
					used[1] = true;
					return true;
			} else if (ram[2] == n){
					used[2] = true;
					return true;
			} else if (ram[3] == n){
					used[3] = true;
					return true;
			}
			return false;
		}
		
		/*void findNextSpot(){
			used[nextSpot] = false;
			//nextSpot++;
						
			while(used[nextSpot-1]==true){
				used[nextSpot] = false;
				nextSpot++;
				if(nextSpot=4)
					nextSpot = 0;
				
			}//end while
						
				
		}*/
		
		//increases the counter and keeps track of where the next replace should go
		void updateSpot() {
			bool keepGoing = true;
			int i = nextSpot;
			while(keepGoing){
				//if used bit is 0
				if(used[i] == false){
					keepGoing = false;
					nextSpot = i;
					used[i] = true;
				}
				//if used bit is 1
				else{
					used[i] = false;
					i = ++i % 4;
				}
			}
		}
};

int main(int argc, char* argv[]) {
	ifstream input(argv[1]);
	std::ofstream output;
	output.open("vm-out.txt");
	char pages[16];
	VMsim vm;
	unsigned address;
	unsigned mask = 0xFFFFF000;
	int setup = 0;
	int count = 0;
	
	while (!input.eof()) {
		input>> hex >>address;
		if(input.eof()) break;
		//read in first 16 lines to get pages set
		if (setup<16) {
			vm.addPages(address);

		} else { //start adding pages to RAM
			vm.addRam(address&mask);		//to put 000 at the end
			//output<< hex << vm.ram[0]<<" "<< vm.ram[1] <<" "<< vm.ram[2] <<" "<< vm.ram[3]<<endl;
			output<<hex<<vm.ram[0];
			for(int i = 1; i<4; i++){
				if(vm.ram[i]==0){
					i=4;
				}else{
					output<<hex<<" "<<vm.ram[i];
				}
			}
			//output<<endl;
			//cout<< count<< " ";
			//cout<< hex << vm.ram[0]<<" "<< vm.ram[1] <<" "<< vm.ram[2] <<" "<< vm.ram[3] << " "<< vm.nextSpot<< " "<<address<<" "<< vm.used[0]<< " "<< vm.used[1]<< " "<< vm.used[2]<< " "<< vm.used[3]<< " "<<endl;
			count++;
		}
		setup++;
		
	}

	input.close();
	return 0;
}
