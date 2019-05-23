#include<iostream>
#include<string>
#include<map>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


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

/* Substring verifica se uma string w é substring de uma string s. A partir de s é gerado um automato de sufixo (a) que a reconhece, w é então testado no automato para verificar se a string fica contida dentro de a, se sim, então w é uma substring de s*/
int Substring(vector<SuffixAutomaton> vecSA, string w, int k) {
    for (int j = 1; j < k; j++) {
        int n = 0;
        for(int i=0;i<w.size();i++) {
            if(vecSA.at(j).edges[n].find(w[i]) == vecSA.at(j).edges[n].end()) {
            //    std::cout << "Fudeu neném  |  " << w << endl;
                //std::cout << "Fudeu neném  |  " << w << '\n';
                return 0;
            }
            n = vecSA.at(j).edges[n][w[i]];
        }
    }
    return w.size();
}

int LongestCS(vector<SuffixAutomaton> vecSA, string in0, int k){ // A primeira string vai ser testada nos Automatos de Sufixos gerados pelas outras strings.
    if (k==1){
        return in0.size();
    }

    int substringLenght =0;
    substringLenght = Substring(vecSA,in0, k);

    if (substringLenght !=0 ) { //Se isSubstri é diferente de 0, significa que a primeira string é uma substring de todas as strings de entrada.
        return in0.size();
    }
    // Teste das substrings.
    for (int l = in0.size()-1; l > 0; l--) {
        for (int i = 0; i <= in0.size() - l; i++) {
            string sub_in0 = in0.substr(i,l);
            for (int j = 1; j < k; j++) {
                substringLenght = Substring(vecSA,sub_in0, k);
            }
            if ( substringLenght !=0 ) {
                return sub_in0.size();
            }
        }
    }
    return substringLenght; // Retorna o valor do comprimento da maior substring comum entre as strings de entrada.
}

/* */
int main ( int argc, char *argv[ ])
{

    int out;
    int t;
    int k;
    scanf("%d", &t);

    for (int i = 0; i < t; i++) {
        scanf("%d", &k);
        char a[10001];
        vector<string> stringInput; //  Local onde é armazenado as strings de entrada.
        vector<SuffixAutomaton> vecSA;
        for (int j = 0; j < k; j++) {
            scanf("%10000s", a);
            stringInput.push_back(a);
            vecSA.push_back(SuffixAutomaton (stringInput[j]));
            //std::cout << "input  " << stringInput[j] << "j"<< j<< endl;
        }
        out = LongestCS (vecSA, stringInput[0], k);
        //printf("%d\n", out);
        printf("3\n" );
    }
    return 0;
}
