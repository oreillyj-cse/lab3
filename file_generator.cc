#include<unordered_map>
#include<iostream>
#include<fstream>
#include <algorithm>
#include <random>
#include<sstream>
#include<chrono>
#include "file_gen.h"
//include<cstdarg> //https://www.tutorialspoint.com/variable-number-of-arguments-in-cplusplus

//Maybe in this(your) version, make it take arguments
int main(int argc, char *argv[]){
    using namespace std;
    
    if(argc !=5 && argc!= 6){
        cout<<"file_generator expects 4 or 5 arguments"<<endl;
        cout << " the order is <file name without \".txt\"> <num inserts> <num deletes> <num lookups> [<num threads>]"<<endl;
        cout << "if num threads is not given, defaults to 4"<<endl;
        return -1;
    }

    string fn_base = string(argv[1]);
    int n_insert  = atoi(argv[2]);
    int n_delete  = atoi(argv[3]);
    int n_lookup  = atoi(argv[4]);
    int n_threads = argc == 5 ? 5 : atoi(argv[5]);

    write_test(fn_base,n_insert,n_delete,n_lookup,n_threads);


}
