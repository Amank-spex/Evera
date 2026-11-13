#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <stdexcept>

namespace fs = std::filesystem;



// So that there is no MissTyping or mistake ---> but for Strings
constexpr const char* EVERA = ".evera"; 
constexpr const char* DEFAULT_BRANCH = "main";

bool isValidating(const fs::path& everaDir){
	return fs::is_directory(everaDir / "objects") &&
			fs::is_directory(everaDir / "refs" / "heads") &&
			fs::is_regular_file(everaDir / "HEAD");
}





// Checking and Initallizing the Evera Directory
int initialize(const fs::path& targetDir){


	fs::path everaDir = targetDir / EVERA;


	/*Think of try / throw / catch like a fire alarm system:

	1. try = “I’m going to do some risky work.”
	2. throw = pulling the fire alarm: “something went wrong, stop everything!”
	3. catch = the person who responds to the alarm and decides what to do.*/



	try {
		//checking whether the working directory exist or not
		if (!fs::exists(targetDir)){
			fs::create_directories(targetDir);
		}else if(!fs::is_directory(targetDir)){
			std::cerr << "\033[31m Error :" << targetDir.string() << " is not a directory\033[0m\n";
			return 1;
		}

		if (fs::exists(everaDir)){
			if (!fs::is_directory(everaDir)){
				std::cerr << "\033[31m Error :" << EVERA << " is not a directory\033[0m\n";
				return 1;
			}

			if (isValidating(everaDir)){
				std::cout <<targetDir.string() << " is already a evera repo\n";
				return 0;
			}else{
				std::cerr << "\033[31m Error :" << EVERA<<" exists but its incomplete or corrupted\033[0m\n";
				return 1;
			}
		}



		//Creating all the Directories 
		fs::create_directories(everaDir / "objects");            // we didn't check whether it was created or not, why
		fs::create_directories(everaDir / "refs" / "heads");     // ---> Some functions pull the alarm by themselves when they fail, like create_directories

		//creating all the file (HEAD)
		{
			std::ofstream headFile(everaDir/"HEAD" , std::ios::trunc);

			if(!headFile){
				
				// here Creating OR Writing to a file does not pull the ALARM by themselves. It fails silently and only sets a flag you can check
				// So here, we pull the alarm ourselves
				throw std::runtime_error(" could not create the HEAD file");
			}

			headFile << "ref: refs/heads/" << DEFAULT_BRANCH<<'\n';
			headFile.close();

			//Now checking whether it failed after writing 
			if (headFile.fail()){                                            // We can check both error here alltogether
				throw std::runtime_error(" Could not write the HEAD file");
			}


			std::cout << "Initialized Sucsessfully.....\n";
			return 0;
		}

	}catch(const std::exception& e){    //std::exception is the parent type of all standard errors i.e runtime_error (ours) and filesystem_error (from create_directories).
		std::cerr << "\033[31mError \033[0m:" << e.what()<<'\n';

		// Something Smart is happening here

		std::error_code AnotherErr;              // We are creating a varible that can hold error but does not throw it like throm()
		fs::remove_all(everaDir , AnotherErr);   // If by chance another error happen on top of first while deleting,
												 // it store the error in AnotherErr and stays quiet
		return 1;
	}
	return 1;

}








int main(int argc , char* argv[]){

	//No Command passed
	if (argc < 2){
		std::cerr << "\033[31m ERROR : no command found\033[0m \n";
		return 1;
	}



	// Storing the command
	std::string str = argv[1];




	//Comparing the command 
	if ( str == "init"){
		// absolute()  == if path already exist -> ignore it |OR| current_path() / argv[2]
		// const fs::path p = fs::absolute((argc >= 3) ? fs::path(argv[2]) : fs::current_path());   

		// we need the relative path
		const fs::path p = (argc >= 3) ? fs::path(argv[2]) : fs::current_path();
		return initialize(p);
	}


	std::cerr << "\033[31m Unknown Command\033[0m \n";
	return 1;

}
