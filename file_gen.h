#ifndef FILE_GEN_H
#define FILE_GEN_H

#include<string>
#include<vector>


struct map_op{
    char op;
    int key;
    std::string value;
    std::string result;

    map_op(char op, int key): op(op), key(key) {/**/ }

    map_op(char op, int key, std::string val): op(op), key(key),value(val) {/**/}
};

std::vector<map_op> file2map_ops(std::string fn);
void map_ops_sim(std::vector<map_op>& map_ops);
void map_op_results2file(std::vector<map_op>& map_ops, std::string fn);

void run_st_sim(std::string fn, bool justLoad=false);

bool write_test(std::string fn, const int inserts, const int deletes, const int lookups, int n_threads);

#endif //FILE_GEN_H