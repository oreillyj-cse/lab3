#include "file_gen.h"
#include <gtest/gtest.h>
#include <iostream>
#include<fstream>

bool filesIdentical(std::string fn1, std::string fn2);
bool map_ops_indentical(std::vector<map_op>& mo1, std::vector<map_op>& mo2);

class LargeFile : public ::testing::Test{
    protected:
    std::vector<map_op> map_ops;
    std::vector<map_op> expected_res;

    LargeFile(){
        map_ops = file2map_ops("basicTests1M.txt");
        expected_res = map_ops;
        map_ops_sim(expected_res);
    }
};

class SmallFile : public ::testing::Test{
    protected:
    std::vector<map_op> map_ops;
    std::vector<map_op> expected_res;

    SmallFile(){
        map_ops = file2map_ops("basicTests10.txt");
        expected_res = map_ops;
        map_ops_sim(expected_res);
    }
};

// TEST(MapOpsTests, JustOps) {
//     std::string fn = "basicTests1M";
//     // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
// }

TEST_F(SmallFile, JustOps) {
    std::string fn = "basicTests10";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    
}

TEST_F(SmallFile, Ops2Sim) {
    std::string fn = "basicTests10";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    map_ops_sim(map_ops);
}

TEST_F(LargeFile, JustOps) {
    std::string fn = "basicTests1M";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    
}

TEST_F(LargeFile, Ops2Sim) {
    std::string fn = "basicTests1M";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    map_ops_sim(map_ops);
}

TEST_F(LargeFile, Ops2SimPlusTest) {
    std::string fn = "basicTests1M";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    map_ops_sim(map_ops);
    bool successful = map_ops_indentical(map_ops,expected_res);
    ASSERT_TRUE(successful);
}

TEST_F(LargeFile, Ops2Sim2File) {
    std::string fn = "basicTests1M";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    map_ops_sim(map_ops);
    map_op_results2file(map_ops, fn+".txt");
}

TEST_F(LargeFile, Ops2Sim2FilePlusTest) {
    std::string fn = "basicTests1M";
    // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
    map_ops_sim(map_ops);
    map_op_results2file(map_ops, fn+".txt");
    bool successful = filesIdentical(fn+"_expected.txt",fn+"_out.txt");
    ASSERT_TRUE(successful);
}



// TEST(MapOpsTests, Ops2Sim2File) {
//     std::string fn = "basicTests1M";
//     // std::vector<map_op> map_ops= file2map_ops(fn+".txt");
//     // map_ops_sim(map_ops);
//     // map_op_results2file(map_ops, fn+".txt");
// }


bool map_ops_indentical(std::vector<map_op>& mo1, std::vector<map_op>& mo2){
    if(mo1.size()!=mo2.size()) return false;
    for(int i = 0; i<mo1.size(); ++i){
        if( mo1[i].op != mo2[i].op
           || mo1[i].key != mo2[i].key
           || mo1[i].value != mo2[i].value
           || mo1[i].result != mo2[i].result
        ) return false;
    }

    return true;
}

bool filesIdentical(std::string fn1, std::string fn2){
    using namespace std;
    ifstream in1;
    ifstream in2;

    in1.open(fn1);
    in2.open(fn2);

    bool identical = true;

    string line1;
    string line2;
    while(true){
    
        istream& ret1 = getline(in1,line1);
        istream& ret2 = getline(in2,line2);

        if(!ret1&& !ret2)//both finished, ok
            break;

        if(!ret1 || !ret2){//one is longer than other
            identical = false;
            break;
        }

        if(line1!=line2){
            identical = false;
            break;
        }
    }
    

    in1.close();
    in2.close();

    return identical;
}