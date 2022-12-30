#include "ypglpk.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <cstdlib>
using namespace std;

typedef pair<int, int> Mypair;	//from, to

deque<int> outEdge[81];
deque<int> inEdge[81];
deque<Mypair> order;
double weight[91];

int main(int argc, char const *argv[])
{
	int n, m;
	int u, v;
	double w;

	scanf("%d%d", &n, &m);

	for(int i=0; i<m; i++){
		scanf("%d%d%lf", &u, &v, &w);
		outEdge[u].push_back(v);
		inEdge[v].push_back(u);
		order.push_back({u, v});
		weight[i] = w;
	}

	ypglpk::set_output(true);
	vector<std::vector<double>> A(m + 3*n, std::vector<double>(m+n));
    vector<double> b(m + 3*n), c(m+n);

    int index = 0;
    for(auto it=order.begin(); it!=order.end(); ++it){
    	A[index][index] = 10000;
    	A[index][(*it).first + m - 1] = 1;
    	A[index][(*it).second + m - 1] = -1;
    	b[index] = 9999;
    	index++;
    }

    for(int i=1; i<=n; i++){
    	for(auto it=inEdge[i].begin(); it!=inEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == (*it) && (*that).second == i){
    				A[index][count] = 1;
    			}
    			count++;
    		}
    	}

    	for(auto it=outEdge[i].begin(); it!=outEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == i && (*that).second == (*it)){
    				A[index][count] = -1;
    			}
    			count++;
    		}
    	}

    	if(i == 1)
    		b[index] = -1;
    	else if(i == n)
    		b[index] = 1;
    	else
    		b[index] = 0;

    	index++;
    }

    for(int i=1; i<=n; i++){
    	for(auto it=outEdge[i].begin(); it!=outEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == i && (*that).second == (*it)){
    				A[index][count] = 1;
    			}
    			count++;
    		}
    	}

    	if(i == n)
    		b[index] = 0;
    	else
    		b[index] = 1;

    	index++;
    }

    for(int i=0; i<m; i++){
    	c[i] = weight[i];
    }
    

    if(index != m+3*n){
    	printf("index = %d\n", index);
    	exit(1);
    }

    pair<double, std::vector<double>> res;
    vector<int> vartype(n);
    vartype[0] = GLP_CV;
    vartype[1] = GLP_IV;
    vartype[2] = GLP_IV; // y,z should be integers
    res = ypglpk::mixed_integer_linear_programming(A, b, c, vartype);

    if(res.first == -ypglpk::INF){
    	printf("-1");
    }
    else{
    	cout << res.first;
    }

	return 0;
}
