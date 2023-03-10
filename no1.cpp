#include "ypglpk.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <cstdlib>
using namespace std;

typedef pair<int, int> Mypair;	//from, to

deque<Mypair> outEdge[81];		//vertex, weight
deque<Mypair> inEdge[81];
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
		outEdge[u].push_back({v, w});
		inEdge[v].push_back({u, w});
		order.push_back({u, v});
		weight[i] = w;
	}

	ypglpk::set_output(true);
	vector<vector<double>> A(m + 3*n, vector<double>(m+n));
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
    			if((*that).first == (*it).first && (*that).second == i && weight[count] == (*it).second){
    				A[index][count] = 1;
    				break;
    			}
    			count++;
    		}
    	}

    	for(auto it=outEdge[i].begin(); it!=outEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == i && (*that).second == (*it).first && weight[count] == (*it).second){
    				A[index][count] = -1;
    				break;
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
    	for(auto it=inEdge[i].begin(); it!=inEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == (*it).first && (*that).second == i && weight[count] == (*it).second){
    				A[index][count] = -1;
    				break;
    			}
    			count++;
    		}
    	}

    	for(auto it=outEdge[i].begin(); it!=outEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == i && (*that).second == (*it).first && weight[count] == (*it).second){
    				A[index][count] = 1;
    				break;
    			}
    			count++;
    		}
    	}

    	if(i == 1)
    		b[index] = 1;
    	else if(i == n)
    		b[index] = -1;
    	else
    		b[index] = 0;

    	index++;
    }

    for(int i=1; i<=n; i++){
    	for(auto it=outEdge[i].begin(); it!=outEdge[i].end(); ++it){
    		int count = 0;
    		for(auto that=order.begin(); that!=order.end(); ++that){
    			if((*that).first == i && (*that).second == (*it).first && weight[count] == (*it).second){
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

    for(int i=0; i<25; i++){
    	for(int j=0; j<15; j++){
    		printf("%lf ", A[i][j]);
    	}
    	printf("%lf\n", b[i]);
    }
   	for(int i=0; i<25; i++){
   		printf("%lf\n", c[i]);
   	}

    pair<double, vector<double>> res;
    vector<int> vartype(m+n);
    for(int i=0; i<m; i++){
    	vartype[i] = GLP_IV;
    }
    for(int i=m; i<n+m; i++){
    	vartype[i] = GLP_CV;
    }
    res = ypglpk::mixed_integer_linear_programming(A, b, c, vartype);

    cout << "\033[1;36m" << "MILP: max=" << res.first << endl;
    if(res.first == -ypglpk::INF){
    	printf("-1\n");
    }
    else{
    	printf("done\n");
    }

	return 0;
}
/*
5 10
1 3 906394
3 2 548103
2 5 745834
1 2 1
3 1 0
1 4 0
3 1 1
1 5 1
2 1 1
3 2 1
*/
