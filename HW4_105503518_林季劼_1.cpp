#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h> 
#include<stdlib.h>
#include<iostream>
#include<queue>

#define maxn 1001
#define maxc 1001

using namespace std;

typedef struct g {
	int va;
	bool s;
	struct g* next;
}graph;

int	edmond_karp(graph *g, int v);
int BFS(graph *g, int v);
void DFS(graph *g, int from);
void find_min_cut(graph *g, int v);

int parent[maxn] = { 0 };
int allcapa[maxn][maxn] = { 0 };
bool visit[maxn] = { 0 };

int main() {
	FILE *f;
	char name[20];

	gets(name);
	if (!(f = fopen(name, "rb")))
	{
		printf("file open failed.\n");
	}
	else
	{
		int times = 0, ve = 0, eg = 0, from = 0, to = 0, capa = 0;

		fscanf(f, "%d", &times);
	//	printf("times %d\n", times);
		for (int k=1; k<=times; k++) {

			for (int i = 0; i < maxn; i++) {
				for (int k = 0; k < maxn; k++) {
					allcapa[i][k] = 0;
				}
				parent[i] = 0;
				visit[i] = 0;
			}

			fscanf(f, "%d%d", &ve, &eg);
		//	printf("ve %d eg %d\n", ve, eg);
			graph *g=NULL;
			g = (graph*)(malloc((ve+1) * sizeof(graph)));//生出對應數量的VER
			for (int i = 0; i<=ve; i++) {				//Initial all vertex
				g[i].va = 0;
				g[i].s = 0;
				g[i].next = NULL;
			}

			for (; eg>0; eg--) {
				fscanf(f, "%d%d%d", &from, &to, &capa);		//取得所有的EDGE AND CAPACITY 

				graph *pt = NULL;
				bool create = 1;

				pt = (g + from);		//指到底 生成EDGE
				while(pt->next!=NULL){
					pt = pt->next;
					if (pt->va == to) {
						create=0; //已經有這個EG了
						break;
					}
				}
				if (create == 1) {
					pt->next = (graph*)(malloc(sizeof(graph)));//生出一個EDGE 
					pt = pt->next;
					pt->va = to;	pt->s = 1;	pt->next = NULL;
					allcapa[from][to] = capa;		//放上capacity
										//	cout << from << " " << to << " " << capa << endl;
				pt = (g + to);
				while (pt->next != NULL) {//指到底 生成EDGE
					pt = pt->next;

				}
				pt->next = (graph*)(malloc(sizeof(graph)));//生出一個EDGE 
				pt = pt->next;
				pt->va = from;	pt->s = 0;	pt->next = NULL;
				}
				else {
					allcapa[from][to] = capa;
					pt->s = 1;
										//	cout << from << " " << to << " " << capa<<" back" << endl;
				}
			}//把所有的EDGE放完 並初始化

			 //找MAX FLOW 
			 cout<<"case "<<k<<endl;
			cout<<"maximun flow: "<<edmond_karp(g, ve)<<endl;
			find_min_cut(g,ve);


		}

		fclose(f);
	}
	system("pause");
	return 0;

}

int BFS(graph *g, int v) {		//s=1 t=v
	queue<int> sQ;
	queue<int> mQ;
	int resul = 0;
	for (int i = 0; i <= v; i++) { parent[i] = 0; }
	parent[1] = -1;
	sQ.push(1);
	mQ.push(maxc);

	while (!sQ.empty()) {
		int deal=0,minc=maxc;
		deal = sQ.front(); //取得sQ的頭
		minc = mQ.front();

		sQ.pop();
		mQ.pop();
	//	cout <<"deal: "<< deal << " minc: "<<minc << endl;
			for (graph *pt = g[deal].next; pt != NULL;) {//把正在處理的這個點的邊全部放進Q裡面
				//cout << "pt->va"<<pt->va << endl;
				if (allcapa[deal][pt->va] > 0 && parent[pt->va] == 0) {//如果還有CAPACITY可以走的話，也沒被放過就把他的邊放進來
					sQ.push(pt->va);		
					mQ.push(min(minc, allcapa[deal][pt->va]));
					parent[pt->va] = deal;
					if (pt->va == v) {
						resul = min(minc, allcapa[deal][pt->va]);	//找到一條路了

						break;
					}
				}
				pt = pt->next;
			}
	}
	if (resul > 0) {
		int nowver = v;
		while (nowver != 1)
		{
			allcapa[parent[nowver]][nowver] -= resul;
			allcapa[nowver][parent[nowver]] += resul;
			nowver = parent[nowver];
		}

	}
	return resul;

}
int edmond_karp(graph *g, int v) {
	//BFS 開始找AUGMENT PATH 
	int totflow = 0;
	while (true)
	{
		int nowFlow = BFS(g,v);
		if (nowFlow == 0) break;
		else totflow += nowFlow;
	}
	return totflow;

}


void find_min_cut(graph *g, int v) {
	DFS(g, 1);		//DFS完 VISIT到的點是可以觸及的點

	for (int i = 1; i <= v; i++) {
		if (visit[i] == 1) { //走過那個點
			graph *pt = g + i;
			for (pt = pt->next; pt != NULL;) {	//走過每個邊
				if (allcapa[i][pt->va] == 0 && !visit[pt->va] && pt->s == 1) {
					cout << i << "->" << pt->va << endl;
				}
				pt = pt->next;
			}
	}
	
	
	}

}



void DFS(graph *g, int from) {  //用DFS找出MIN CUT
	visit[from] = 1;
	graph *pt = g + from;

	for (pt = pt->next; pt != NULL;) {	//走過每個邊
		if (allcapa[from][pt->va] > 0 && !visit[pt->va] &&pt->s==1) {  //大於零表示可以走  而且要沒走過  pt->s是1的話表示是原來就有的邊不是RESIIDUAL
			DFS(g, pt->va);
		}
		pt = pt->next;
	}

}
