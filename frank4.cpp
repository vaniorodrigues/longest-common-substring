//#include <bits/stdc++.h>
#include<iostream>
#include<string>
#include<map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

string marker[9] = {"$","|", "!", "@", "#","&","*","?",">"};
struct SuffixAutomaton {
  vector<map<char,int> > edges; // edges[i]  : the labeled edges from node i
  vector<int> link;            // link[i]   : the parent of i
  vector<int> length;          // length[i] : the length of the longest string in the ith class
  int last;                    // the index of the equivalence class of the whole string

  SuffixAutomaton(string s) {
    // add the initial node
    edges.push_back(map<char,int>());
    link.push_back(-1);
    length.push_back(0);
    last = 0;

    for(int i=0;i<s.size();i++) {
      // construct r
      edges.push_back(map<char,int>());
      length.push_back(i+1);
      link.push_back(0);
      int r = edges.size() - 1;

      // add edges to r and find p with link to q
      int p = last;
      while(p >= 0 && edges[p].find(s[i]) == edges[p].end()) {
        edges[p][s[i]] = r;
        p = link[p];
      }
      if(p != -1) {
        int q = edges[p][s[i]];
        if(length[p] + 1 == length[q]) {
          // we do not have to split q, just set the correct suffix link
          link[r] = q;
        } else {
          // we have to split, add q'
          edges.push_back(edges[q]); // copy edges of q
          length.push_back(length[p] + 1);
          link.push_back(link[q]); // copy parent of q
          int qq = edges.size()-1;
          // add qq as the new parent of q and r
          link[q] = qq;
          link[r] = qq;
          // move short classes pointing to q to point to q'
          while(p >= 0 && edges[p][s[i]] == q) {
            edges[p][s[i]] = qq;
            p = link[p];
          }
        }
      }
      last = r;
    }
  }
};

//int LongestCS(vector<SuffixAutomaton> vecSA, string in0, int k){ // A primeira string vai ser testada nos Automatos de Sufixos gerados pelas outras strings.
int LongestCS(string in0, string  V, int k){

    SuffixAutomaton sa(V);

    bool notSubtring = false;
    int n = 0;
    //for (int j = 1; j < k; j++) {
    	//n = 0;
    	for(int i=0;i<in0.size();i++) {
        	if(sa.edges[n].find(in0[i]) == sa.edges[n].end()) {

            	notSubtring = true;
            	break;
            }
            n = sa.edges[n][in0[i]];
        }
    //}
    if (!notSubtring) {
        return in0.size();
    }

    string sub_in0;
    int lenghtTest = 1;
    int lcs = 0;

    for (int in = 0; in <= in0.size(); in++) {
        //for (int l = 1; l < in0.size() -1 ; l++){
        //int sub1 = in0.substr(0,2);
        sub_in0 = in0.substr(in,lenghtTest);
        notSubtring = false;
        //for (int j= 0;j<k-1; j++){
        //    sub_in0 = marker[j] + sub_in0;
        //    std::cout << "subin0dentro   " << sub_in0 << '\n';

        //}
        sub_in0 = marker[0] + sub_in0 + marker[1] + sub_in0;

        std::cout << "subin0fora   " << sub_in0 << '\n';
        //for (int j = 1; j < k; j++) {// testa uma string por vez
        n = 0;
        for(int i=0;i<sub_in0.size();i++) {
            if(sa.edges[n].find(sub_in0[i]) == sa.edges[n].end()) {
            //std::cout << "in['%d']  "<< i <<'\t'<< sub_in0 << '\n';
            notSubtring = true;
            break; // Sai do laço de testar esse AS.
            }
            n = sa.edges[n][sub_in0[i]];
            //}
            //    if (notSubtring) {
            //    break; // Nao testa mas nenhuma AS
            //    }
            }
        if (!notSubtring && lcs < sub_in0.size()) {
            lenghtTest = sub_in0.size() +1;
            lcs = sub_in0.size();
            std::cout << "lcs " << lcs << '\t'<< "lenthTest  "<< lenghtTest<< endl;
        }
        //}
    }
    return lcs;
}

//vector<string> stringInput;
//vector<SuffixAutomaton> vecSA;
char a[10000];
string V;

int main ()
{
    int out = 0;
    int t;
    int k;
    scanf("%d", &t);
    //vector<string> stringInput; //  Local onde é armazenado as strings de entrada.
    for (int i = 0; i < t; i++) {
        scanf("%d", &k);
        V.clear();
        //stringInput.clear();
        //vecSA.clear();
        if (k == 1) {
            scanf("%s", a);
            V = a;
            out = V.size();
            printf("%d\n", out);
        } else {
            scanf("%s", a);// Primeira string inserida, será utilizada para gerar substrings.
            string in0 = a;
            for (int j = 0; j < k-1; j++) { // k-1 já que a primeira string nao precisa ser testada.
                scanf("%s", a);
                V += marker[j] + a;
            }
            std::cout << "V   " << V << '\n';
            out = LongestCS (in0, V, k);
            printf("%d\n", out);
        }
    }
    return 0;
}
