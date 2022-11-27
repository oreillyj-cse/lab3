#include<unordered_map>
#include<iostream>
#include<fstream>
//include<string>
//include<vector>
#include <algorithm>
#include <random>
#include<sstream>
#include<chrono>
#include "file_gen.h"

std::vector<map_op> file2map_ops(std::string fn){
    using namespace std;
    std::vector<map_op> map_ops;

    ifstream in;
    in.open(fn);

    //https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    string line;
    getline(in,line);
    map_ops.push_back(map_op('N', stoi(line.substr(2)) ));

    while(getline(in,line)){

        char action;
        int key;
        int first_quote = line.find('"'); 
        
        action = line.at(0);
        key = stoi(line.substr(2,first_quote-2));

        if(action=='I') {
            string rest = line.substr(first_quote,line.length()-first_quote+1);
            map_ops.push_back(map_op(action,key,rest));
        } else{
            map_ops.push_back(map_op(action,key));
        } 
    }
    
    in.close();

    return map_ops;

}

void map_ops_sim(std::vector<map_op>& map_ops){
    using namespace std;
    unordered_map<int,string> dict;

    map_ops[0].result = "Using " + to_string(map_ops[0].key) + " threads";

    for(int i = 1 ; i<map_ops.size(); ++i){
        char action = map_ops[i].op;
        int key = map_ops[i].key;
        
        
        if(action=='I') {
            string value = map_ops[i].value;

            if(dict.find(key)==dict.end()){
                dict.insert({key,value});
                map_ops[i].result = "OK";
            } else{
                map_ops[i].result ="Fail";
            }
        } else if (action =='L'){
            auto search = dict.find(key);
            if(search==dict.end()){
                map_ops[i].result = "No "+ to_string(key);
            } else{
                string value = search->second;
                map_ops[i].result = value.substr(1,value.length()-2); 
            }
        } else if (action == 'D'){
            auto search = dict.find(key);
            if(search==dict.end()){
                map_ops[i].result ="Fail";
            } else{
                dict.erase(key);
                map_ops[i].result ="OK";
            }
        }
    }
    
}

void map_op_results2file(std::vector<map_op>& map_ops, std::string fn){
    using namespace std;
    ofstream out;
    out.open(fn.substr(0,fn.length()-4)+"_out.txt");

    for(int i = 0; i<map_ops.size();++i){
        out<<map_ops[i].result<<endl;
    }

    out.close();
}

void run_st_sim(std::string fn, bool justLoad){
    //std::ios_base::sync_with_stdio(false);
    using namespace std;

    ifstream in;
    ofstream out;

    in.open(fn);
    out.open(fn.substr(0,fn.length()-4)+"_out.txt"); 

    unordered_map<int,string> dict;

    //https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    string line;
    getline(in,line);
    out<<"Using " << stoi(line.substr(2))<< " threads"<<endl;

    //define BUFFER_OUT //went down the rabbit hole... I/O slow on WSL, 
    // and majority cost for Hash Table version
    #ifdef BUFFER_OUT
        stringstream  buff_out;
        const int FLUSH_FREQ = 10000;
        int line_ct = 0;
        #define OUT buff_out
        #define ENDL '\n'
        
    #else
        #define OUT out
        #define ENDL std::endl

    #endif //BUFFER_OUT

    while(getline(in,line)){
        if (justLoad==true) {
                OUT<<"TEST"<<ENDL;
                #ifdef BUFFER_OUT
                    ++line_ct;
                    if(line_ct=FLUSH_FREQ){
                        line_ct=0;
                        out<<buff_out.str();
                        out.flush();
                        buff_out.clear();
                        buff_out.str(std::string());
                    }
                #endif //BUFFER_OUT
                continue;
            }

        char action;
        int key;
        int first_quote = line.find('"'); //honestly, should just parse the string, using first " index
        
        action = line.at(0);
        key = stoi(line.substr(2,first_quote-2));

        if(action=='I') {
            string rest = line.substr(first_quote,line.length()-first_quote+1);
            if(dict.find(key)==dict.end()){
                dict.insert({key,rest});
                OUT<<"OK"<<ENDL;
            } else{
                OUT<<"Fail"<<ENDL;
            }
        } else if (action =='L'){
            auto search = dict.find(key);
            if(search==dict.end()){
                OUT<<"No "<<key<<ENDL;
            } else{
                string value = search->second;
                OUT<<value.substr(1,value.length()-2)<<ENDL; 
            }
        } else if (action == 'D'){
            auto search = dict.find(key);
            if(search==dict.end()){
                OUT<<"Fail"<<ENDL;
            } else{
                dict.erase(key);
                OUT<<"OK"<<ENDL;
            }
        }

        #ifdef BUFFER_OUT
        ++line_ct;
        if(line_ct=FLUSH_FREQ){
            line_ct=0;
            out<<buff_out.str();
            out.flush();
            buff_out.clear();
            buff_out.str(std::string());
        }
        #endif //BUFFER_OUT

    }
    
    #ifdef BUFFER_OUT
        out<<buff_out.str();
        out.flush();
    #endif //BUFFER_OUT
    

    out.close();
    in.close();
}
/*
//this version doesn't have the buffer stuff, but has a few things that need t
// be cleaned
void run_st_sim(std::string fn, bool justLoad){


    using namespace std;

    ifstream in;
    ofstream out;

    in.open(fn);
    out.open(fn.substr(0,fn.length()-4)+"_out.txt"); //

    unordered_map<int,string> dict;

    //https://stackoverflow.com/questions/7868936/read-file-line-by-line-using-ifstream-in-c
    string line;
    getline(in,line);
    out<<"Using " << stoi(line.substr(2))<< " threads"<<endl;


    while(getline(in,line)){
        // cout<<line<<endl;
        // stringstream ssline(line);
        if (justLoad==true) {
                //out<<"TEST"<<endl;
                continue;
            }

        char action;
        int key;
        int first_quote = line.find('"'); //honestly, should just parse the string, using first " index

        // ssline>>action>>skip>>key;
        
        action = line.at(0);
        key = stoi(line.substr(2,first_quote-2));

        // cout<<action <<" "<<key<<endl;
        if(action=='I') {
            string rest = line.substr(first_quote,line.length()-first_quote+1);

            // if (justLoad==true) {
            //     out<<"TEST"<<endl;
            //     continue;
            // }
            if(dict.find(key)==dict.end()){
                dict.insert({key,rest});
                out<<"OK"<<endl;
            } else{
                out<<"Fail"<<endl;
            }
        } else if (action =='L'){
            // if (justLoad==true) {
            //     out<<"TEST"<<endl;
            //     continue;
            // }
            auto search = dict.find(key);
            if(search==dict.end()){
                out<<"No "<<key<<endl;
            } else{
                string value = search->second;
                out<<value.substr(1,value.length()-2)<<endl; 
            }
        } else if (action == 'D'){
            // if (justLoad==true) continue;
            auto search = dict.find(key);
            if(search==dict.end()){
                out<<"Fail"<<endl;
            } else{
                dict.erase(key);
                out<<"OK"<<endl;
            }
        }
    }
    

    out.close();
    in.close();
}
*/
bool write_test(std::string fn, const int inserts, const int deletes, const int lookups, int n_threads){
    using namespace std;
    std::ofstream out;
    std::ofstream test_out;

    std::vector<int> insertVec;
    for(int i = 0; i<inserts; ++i){
        insertVec.push_back(i);
    }

    //https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(insertVec), std::end(insertVec), rng);

    std::vector<int> deleteVec;
    for(int i = 0; i<deletes;++i){
        deleteVec.push_back(rand()%inserts);
    }

    std::vector<int> lookupVec;
    for(int i = 0; i<lookups;++i){
        lookupVec.push_back(rand()%inserts);
    }

    out.open(fn+".txt");
    test_out.open(fn+"_expected.txt");

    out<<"N "<<n_threads<<std::endl;
    test_out<<"Using " << n_threads << " threads"<<std::endl;
    //https://stackoverflow.com/questions/27024269/select-random-element-in-an-unordered-map
    std::unordered_map<int,std::string> dict;

    int n_insert = 0;
    int n_delete = 0;
    int n_lookup = 0;

    for(     ; n_insert < inserts/2 ; ++n_insert){
        int n = insertVec.at(n_insert);
        std::string s = "key"+to_string(n);
        dict.insert({n,s});
        out<<"I "<<n<<" \""<<s<<"\""<<std::endl;
        test_out<<"OK"<<std::endl;

    }
    std::cout<<"in loop"<<std::endl;
    while(n_insert<inserts || n_delete<deletes || n_lookup<lookups){
        int option = rand()%3;
        
        if(option == 0){
            if (n_insert == inserts) continue;
            int n = insertVec.at(n_insert);
            ++n_insert;
            std::string s = "key"+to_string(n);
            
            out<<"I "<<n<<" \""<<s<<"\""<<std::endl;
            if(dict.find(n) == dict.end()){
                dict.insert({n,s});
                test_out<<"OK"<<std::endl;
            } else {
                test_out<<"Fail"<<std::endl;
            }
        } else if (option ==1){
            if(n_delete == deletes) continue;
            int n = deleteVec.at(n_delete);
            ++n_delete;
            out<<"D "<<n<<std::endl;
            if(dict.find(n) == dict.end()){
                test_out<<"Fail"<<std::endl;
            } else{
                dict.erase(n);
                test_out<<"OK"<<std::endl;
            }
        } else{
            if(n_lookup == lookups) continue;
            int n = lookupVec.at(n_lookup);
            ++n_lookup;
            out<<"L "<<n<<std::endl;
            auto search = dict.find(n);
            if(search == dict.end()){
                test_out<<"No "<<n<<std::endl;
            } else{
                test_out<<search->second<<std::endl;
            }
        }
    }

    out.close();
    test_out.close();

    return true;
}

//Maybe in this(your) version, make it take arguments
//int main(){
    // std::cout<<"????"<<std::endl;
    // // write_test("basicTests",10,5,20,3);
    // // run_ss_sim("basicTestss.txt",false);

     //std::string fn = "basicTests1M";
    
    // write_test(fn,1e6,5e5,1e7,3);
    // std::cout<<"Wrote "<<fn<<" files"<<std::endl;
    
    // // write_test("basicTests1M",1e6,5e5,1e7,3);
    // // std::cout<<"Wrote 1M file"<<std::endl;

    // auto justLoadBegin = std::chrono::high_resolution_clock::now();
    // std::cout<<"starting justLoad==true"<<std::endl;
    // run_st_sim(fn+".txt",true);
    // auto justLoadEnd = std::chrono::high_resolution_clock::now();
    // auto regBegin = std::chrono::high_resolution_clock::now();
    // std::cout<<"starting justLoad==false"<<std::endl;
    // run_st_sim(fn+".txt");
    // auto regEnd = std::chrono::high_resolution_clock::now();

    // // auto durationJustLoad = std::chrono::duration_cast<std::chrono::milliseconds>(justLoadEnd - justLoadBegin);
    // auto durationRegular  = std::chrono::duration_cast<std::chrono::milliseconds>(regEnd - regBegin);

    // // std::cout<<"Just Load Time = "<<durationJustLoad.count()<<"ms "<<std::endl;
    // std::cout<<"Regular   Time = "<<durationRegular.count()<<"ms "<<std::endl;

    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    // std::vector<map_op> copy_map_ops = map_ops;
    // auto mapOpsBegin = std::chrono::high_resolution_clock::now();
    // map_ops_sim(map_ops);
    // auto mapOpsEnd = std::chrono::high_resolution_clock::now();
    // map_op_results2file(map_ops, fn+".txt");
    // std::cout<<"map_ops[0].result = "<<map_ops[0].result<<std::endl;
    // std::cout<<"copy_map_ops[0].result = "<<copy_map_ops[0].result<<std::endl;

    // auto durationMapOps  = std::chrono::duration_cast<std::chrono::milliseconds>(mapOpsEnd - mapOpsBegin);

    // std::cout<<"Map_ops Duration = "<<durationMapOps.count()<<"ms "<<std::endl;


//}
