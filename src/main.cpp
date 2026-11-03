#include <iostream>
#include <string>
using namespace std;

int main(int argc , char* argv[]){
	if (argc > 1 && (strcmp(argv[1] , "init" ) == 0)){
		cout << "initializing init...."<< endl;
	}else{
		cout << "Something Went Wrong " << endl;
	}

	return 0;




}
