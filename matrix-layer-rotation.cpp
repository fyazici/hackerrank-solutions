#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <tuple>
using namespace std;

/* I have seen another solution to this problem on github, but it was O(N*M*R) in time.
 * My proposed solution below makes a coordinate transformation (ij to chain-pos) to 
 * achieve single step action on every matrix element. Also, storing matrix elements in a linear
 * data structure (with half-of-the-time sequential access) allows better cache line
 * utilization. This solution is in-place, O(1) in memory and O(N*M) in time.
 * 300, 300, 99999999 test case takes under 40msecs.
 */

int chain_number(int i, int j, int M, int N) {
    return min(min(i, M-i-1), min(j, N-j-1));
}

int chain_length(int c, int M, int N) {
    int mp = M-2*c, np = N-2*c;
    return np + mp + np + mp - 4;
}

pair<int, int> ij_to_cl(int i, int j, int M, int N) {
    int chain = chain_number(i, j, M, N);
    int ip = i-chain, jp = j-chain;
    int mp = M-2*chain, np = N-2*chain;
    int lpos;
    
    if (ip == 0) {
        lpos = jp;
    } else if (jp == (np - 1)) {
        lpos = ip + np - 1;
    } else if (ip == (mp - 1)) {
        lpos = chain_length(chain, M, N) - mp - jp + 1;
    } else if (jp == 0) {
        lpos = chain_length(chain, M, N) - ip;
    }
    
    return make_pair(chain, lpos);
}

pair<int, int> cl_to_ij(int c, int l, int M, int N) {
    int mp = M-2*c - 1, np = N-2*c - 1;
    int ip, jp;
    
    if (l < np) {
        ip = 0; jp = l;
    } else if (l < (np + mp)) {
        ip = l - np; jp = np;
    } else if (l < (np + mp + np)) {
        ip = mp; jp = np + mp + np - l;
    } else if (l < (np + mp + np + mp)) {
        ip = np + mp + np + mp - l; jp = 0;
    }
    
    return make_pair(ip + c, jp + c);
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    
    int M, N, R;
    cin >> M >> N >> R;
    
    vector<int> matrix;
    matrix.reserve(M * N);
    
    for (int i = 0; i < M * N; ++i) {
        int x;
        cin >> x;
        matrix.emplace_back(x); // linear storage of matrix
        // better cache line management for sequential access
        // (i, j)-th element -> (i * N + j)-th element
    }
    
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            auto pos = ij_to_cl(i, j, M, N);
            int clength = chain_length(pos.first, M, N);
            pos.second = (pos.second + R) % clength;
            auto coord = cl_to_ij(pos.first, pos.second, M, N);
            cout << matrix[coord.first * N + coord.second] << " ";
        }
        cout << endl;
    }
    
    return 0;
}
