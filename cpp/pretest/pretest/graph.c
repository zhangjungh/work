/******************************************************************************
Problem   
The empire has a capitol and a number of cities. Some cities are connected to other cities. A route 
connecting two cities can transport a message in both directions. All cities are reachable using some path 
from the capitol city. The connections among all cities ar e described in an adjacency matrix with the format 
shown below. At the start, a message is sent from the  capitol city to all other cities, and we want to know 
what's the minimal time for a message to spr ead out from the capitol to the whole empire. 

Input (Adjacency Matrix)  
The first line of the input file will be  n , the number of cities, 1 <= n  <= 100.  The rest of the input defines 
an adjacency matrix,  A.  The adjacency matrix  is square and of size  n ¡Án .  The value of A( i,  j) indicates the 
time required to travel from city i  to city  j.  A value of character ' x'  for  A(i,  j) indicates that there is no route 
between city i  to city  j.  Since a message sent to itself doesn't require time, and the route between cities is 
two way, we always have  A( i,  i) = 0 and  A( i,  j) =  A( j,  i). Thus only the entries on the (strictly) lower 
triangular portion of A will be supplied as input, an d the second line of input will contain one entry,  A(2 ,  1). 
The next line will contain two entries, A(3 ,  1 ) and A(3 ,  2 ), and so on.  
  
Output  
Program should output the minimum time required before a message sent from the capitol (city #1) is 
known throughout the empire, i.e.  the time it is received in the last city to get the message.  

Approach
The problem is a typical Dijkstra shortest path issue, single source (from the captial) 
and nonnegative weight for edges(travel time always positive)
the undirected graph in the case is easy to change to directed graph

Known Issues:
Input overflow
Waiting for user input if the elements are less then expected
Integar overflow

Tested:
CPU: Pentium Dual-Core T4200 2.0G
OS: Win7 Home x86
IDE: VC++ 2010 Express

Author: Zhang Jun (zhangjun.cn@gmail.com)

Time Spent: close to 6 hours including testing
******************************************************************************/
//includes
#include <stdio.h>		//printf, scanf, system
#include <malloc.h>		//malloc, free
#include <assert.h>		//assert

//defines
#define INT_MAX 0x7fffffff	//max integer for 32bit int
#define MIN_CITIES 1
#define MAX_CITIES 100
#define SIZE 64				//string buffer size
#define RUN_TEST 0			//run test or get input from user

//functions declaration
int read_input(int *n, int **adjMtx);
int solve(int n, int *adjMtx);
void dijkstra_shortest(int n, int v, int *adjMtx, int *dist, int *setv, int *path);
int run_test(void);



//check if the character is digit or not
static int _isdigit(char ch)
{
	return (ch >= '0' && ch <= '9') ? 1 : 0;
}

//check if all the characters are digit or not
static int _isalldigit(char *str)
{
	char *p = str;
	assert(str && "null string!");
	while (*p != '\0')
	{
		if (_isdigit(*p) == 0)
			return 0;
		++p;
	}
	return (p != str) ? 1 : 0;
}

//check if the string is 'x' or 'X'
static int _isonlyx(char *str)
{
	int c = 0;
	char *p = str;
	assert(str && "null string!");
	while (*p != '\0')
	{
		if (*p != 'x' && *p != 'X')
			return 0;
		++p;
		++c;
	}
	//only one x
	return c==1 ? 1 : 0;
}

//transfer a string to number, assume all characters are digit and no sign included
static int _atoi(char *str)
{
	int ret = 0;
	char *p = str;
	assert(str && "null string!");
    while (*p != '\0')  
    {  
        ret = ret * 10 + (*p - '0');  //possible overflow
        ++p;  
    }
	return ret;
}

//transfer input string to number, assume only x or a number string input
static int _inputtoi(char *str)
{
	return (*str == 'x' || *str == 'X') ? INT_MAX : _atoi(str);
}

//check if it is a expected input
static int _check_input(char* str)
{
	return (_isalldigit(str) || _isonlyx(str)) ? 1 : 0;	
}

/******************************************************************************
// Read user input from standard in
// Input:
// Output:
// n: number of cities
// adjMtx: the adjacency matrix, note the memory should be freed later
// Return:
// 1 success; 0 failed alloc memory
******************************************************************************/
int read_input(int *n, int **adjMtx)
{
	int i;
	int value;
	int idx = 0;
	int cnt = 0;
	int sz = INT_MAX;
	char str[SIZE]; //possible overflow

	//read number of cities
	while (1)
	{
		printf("please input the cities:\n");
		scanf("%s", str);

		sz = INT_MAX;
		if (_isalldigit(str) == 1)
		{
			sz = _atoi(str);
		}
		//the first parameter should be a number between min cities and max cities
		if (sz <= MIN_CITIES || sz >= MAX_CITIES)
		{
			printf("first parameter %s isn't in [%d, %d]\n", str, MIN_CITIES, MAX_CITIES);
		}
		else
		{
			break;
		}
	}
	//init number of cities
	*n = sz;
	
	//build adjMtx
	*adjMtx = (int*)malloc(sz*sz*sizeof(int));
	if (*adjMtx == NULL)
	{
		printf("not enough memory\n");
		return 0;
	}

	//read other inputs
	while (++idx < sz)
	{
		while (1)
		{
			printf("please input [%d] elements for [%d]th line of half matrix:\n", idx, idx);
			cnt = 0;
			i = idx;
			while (i--)
			{
				scanf("%s", str);
				if (_check_input(str) == 0)
				{
					printf("input must be a number without sign or character x: %s\n", str);
					scanf("%*[^\n]"); //discard other inputs in this line if error occurs
					break;
				}
				else
				{
					value = _inputtoi(str);
					(*adjMtx)[idx*sz + cnt] = value; //A(i, j)
					(*adjMtx)[cnt*sz + idx] = value; //A(j, i)
					++cnt;
				}
			}
			if (cnt == idx) //inputs for this line are complete
			{
				scanf("%*[^\n]"); //discard more in the tail
				break;
			}
		}
	}
	//init diagnol matrix values
	for (i = 0; i < sz; ++i)
	{
		(*adjMtx)[i*sz + i] = 0;
	}

	//check all values
	//for (i = 0; i < sz * sz; ++i)
	//{
	//	printf("%d\t", (*adjMtx)[i]);
	//	if ((i+1)%sz == 0)
	//		printf("\n");
	//}

	return 1;
}

/******************************************************************************
// Solve minimal travel time issue
// Input:
// n: number of cities
// adjMtx: adjacency matrix represents time travel. INT_MAX means no route. 1D array simulates 2D array
// Output:
// Return:
// 1 success; 0 Matrix wrong
******************************************************************************/
int solve(int n, int *adjMtx)
{
	int i;
	int all = 1;
	int max = 0;
	int *dist = (int*)malloc(n*sizeof(int));
	int *setv = (int*)malloc(n*sizeof(int));
	int *path = (int*)malloc(n*sizeof(int));

	if (dist == NULL || setv == NULL || path == NULL)
	{
		printf("not enough memory\n");
		if (dist) free(dist);
		if (setv) free(setv);
		if (path) free(path);
		return 0;
	}

	dijkstra_shortest(n, 0, adjMtx, dist, setv, path);

	for (i = 0; i < n; ++i)
	{
		if (setv[i] == 0)
		{
			all = 0;
			break;
		}
		//printf("%d\t", setv[i]);
	}
	//printf("\n");
	for (i = 0; i < n; ++i)
	{
		if (max < dist[i])
			max = dist[i];
		//printf("%d\t", dist[i]);
	}
	//printf("\n");
	//for (i = 0; i < n; ++i)
	//{
	//	printf("%d\t", path[i]);
	//}
	//printf("\n");

	free(dist);
	free(setv);
	free(path);

	if (all == 0)
	{
		printf("input matrix wrong, not all cities could be reached!\n");
		return 0;
	}
	else
	{
		printf("minimal time: %d\n", max);
		return 1;
	}
}

/******************************************************************************
// Test the correctness of dijkstra algorithm with sample data
// return value: 1 pass test; 0 fail test or other error
******************************************************************************/
int run_test(void)
{
	int i;
	int max = -1;	//max travel time
	int all = 1;	//1 all visited or 0 not all visited
	int n = 5;
	int adjMtx[25] = {
		0,		50,		30,		100,	10,
		50,		0,		5,		20,		INT_MAX,
		30,		5,		0,		50,		INT_MAX,
		100,	20,		50,		0,		10,
		10,		INT_MAX,INT_MAX,10,		0,
	};

	int *dist = (int*)malloc(n*sizeof(int));
	int *setv = (int*)malloc(n*sizeof(int));
	int *path = (int*)malloc(n*sizeof(int));

	if (dist == NULL || setv == NULL || path == NULL)
	{
		assert(0 && "not enough memory");
		if (dist) free(dist);
		if (setv) free(setv);
		if (path) free(path);
		return 0;
	}

	dijkstra_shortest(n, 0, adjMtx, dist, setv, path);

	
	for (i = 0; i < n; ++i)
	{
		if (setv[i] == 0)
		{
			all = 0;
			break;
		}
		//printf("%d\t", setv[i]);
	}
	//printf("\n");
	for (i = 0; i < n; ++i)
	{
		if (max < dist[i])
			max = dist[i];
		//printf("%d\t", dist[i]);
	}
	//printf("\n");
	//for (i = 0; i < n; ++i)
	//{
	//	printf("%d\t", path[i]);
	//}
	//printf("\n");
	assert(max == 35 && all == 1 && "algorithm wrong, can't pass the test!");

	free(dist);
	free(setv);
	free(path);

	return (max == 35 && all == 1) ? 1 : 0;
}

/******************************************************************************
// Dijkstra algorithm to calculate shortest path with positive weight value
// Input:
// n: number of cities
// v: captial city index
// adjMtx: adjacency matrix represents time travel. INT_MAX means no route. 1D array simulates 2D array
// Output:
// dist: the minimal time travel from v to each city
// setv: cities that has been visited or not
// path: last city that link to current city with minimal time travel
******************************************************************************/
void dijkstra_shortest(int n, int v, int *adjMtx, int *dist, int *setv, int *path)
{
	int i;
	int j;
	int cur; //hold current city index with minimal travel time
	int min; //minimal travel time
	for (i = 0; i < n; ++i)	//init dist and path array from city v
	{
		setv[i] = 0;
		dist[i] = adjMtx[v*n + i];
		if (i != v && dist[i] < INT_MAX)
			path[i] = v;
		else
			path[i] = -1;	// -1 means no route
	}
	setv[v] = 1;	//add city v to visited list
	dist[v] = 0;	//no time need to travel itself
	for (i = 0; i < n-1; ++i)	//check other n-1 cities
	{
		min = INT_MAX;
		cur = v;
		for (j = 0; j < n; ++j)	//get current not-visited city with minimal travel time
		{
			if (setv[j] == 0 && dist[j] < min)
			{
				cur = j;
				min = dist[j];
			}
		}
		setv[cur] = 1;	//add this city to the visied list
		for (j = 0; j < n; ++j)	//update path and dist with the added city
		{
			if (setv[j] == 0 && adjMtx[cur*n+j] < INT_MAX && dist[cur] + adjMtx[cur*n+j] < dist[j])
			{
				dist[j] = dist[cur] + adjMtx[cur*n+j];
				path[j] = cur;
			}
		}
	}
}


int main(int argc, char* argv[])
{
	int n;
	int *adjMtx;

#if RUN_TEST
	if (run_test() == 0)
		printf("test failed\n");
	else
		printf("test passed\n");
#else
	if (read_input(&n, &adjMtx) == 1)
	{
		solve(n, adjMtx);
		free(adjMtx); //free it anyway
	}
#endif
	system("pause");

	return 0;
}