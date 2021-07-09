#include "include_data.h"


void include_main(std::vector < std::string > & argv) {
    include_data P;
    
    boost::program_options::options_description opt_basic ("\x1B[32mBasics\33[0m");
	opt_basic.add_options()
		("help", "Produces option description")
		("seed", boost::program_options::value< unsigned int >()->default_value(123456), "Random number seed. Useful to replicate runs.");
    
    boost::program_options::options_description opt_files ("\x1B[32mI/O\33[0m");
	opt_files.add_options()
		("bed", boost::program_options::value< std::string >(), "Phenotypes in BED format.")
		("out", boost::program_options::value< std::string >(), "Output file.");

    boost::program_options::options_description opt_parallel ("\x1B[32mParallelization\33[0m");
	opt_parallel.add_options()
		("sample", boost::program_options::value< std::string >(), "sample to include")
        ("sample-filelist", boost::program_options::value< std::string >(), "file containing sample(s) to be included. One sample per line");
        
    P.option_descriptions.add(opt_basic).add(opt_files).add(opt_parallel);

    //-------------------
	// 2. PARSE OPTIONS
	//-------------------
	try {
		boost::program_options::store(boost::program_options::command_line_parser(argv).options(P.option_descriptions).run(), P.options);
		boost::program_options::notify(P.options);
	} catch ( const boost::program_options::error& e ) {
		std::cerr << "Error parsing [null] command line :" << string(e.what()) << std::endl;
		exit(0);
	}

    //---------------------
	// 3. PRINT HELP/HEADER
	//---------------------
    std::cout << "\n" << "\x1B[32;1m" << "FILTER BED " << "\033[0m" << endl;
    if(P.options.count("help")) {
        std::cout << P.option_descriptions << std::endl;
        exit(0);
    }

    //-----------------
	// 4. COMMON CHECKS
	//-----------------
    if (!P.options.count("bed")) std::cout <<"Phenotype data needs to be specified with --bed [file.bed]" << std::endl;
    if (!P.options.count("out")) std::cout << "Output needs to be specified with --out [file.out]" << std::endl;
    std::string outFile = P.options["out"].as<std::string>();

    //-------------
    // RUN ANALYSIS
    //-------------

    if(P.options.count("sample")){
        P.samplesToKeep.push_back(P.options["sample"].as<std::string>());
        P.readPhenotypes(P.options["bed"].as<std::string>(),outFile);
    }else if(P.options.count("sample-filelist")){
        P.readInclusion(P.options["sample-filelist"].as<std::string>());
        P.readPhenotypes(P.options["bed"].as<std::string>(),outFile);
    }else{
        std::cout << "You need to either specify a sample to keep from bed with --s or a file containing sample(s) to be kept" << std::endl;
        exit(-1);
    }

}