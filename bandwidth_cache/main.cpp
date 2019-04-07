#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <cstdlib>
#include <cstring>
using namespace std;

#define KB(x) ((size_t) (x) << 10)

int main(){
    vector<size_t> sizes_KB;
    for (int i = 1; i < 30; i++) {
        sizes_KB.push_back(1 << i);
    }

    random_device rd;
    mt19937 gen(rd());
    double total_time = 0.0;
    for (size_t size: sizes_KB) {
        uniform_int_distribution<> dis(0, KB(size)/8 - 1);
        long long int *ptr = (long long int *) malloc(KB(size));
        memset(ptr, 1, KB(size));
        long long int dummy = 0;

        clock_t begin = clock();
        for (long long int i=0; i< KB(size)/8; i++) {
            dummy += ptr[dis(gen)];
            // ptr[i] = 0;
        }
        clock_t end = clock();

        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        cout << size << " KB, " << elapsed_secs << "secs, dummy: " << dummy << "      " << size/1024.0/1024.0 /elapsed_secs << "GB/s " << endl;
    }
}   