#include <bits/stdc++.h>
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

int LongestCS(vector<SuffixAutomaton> vecSA, string in0, int k){ // A primeira string vai ser testada nos Automatos de Sufixos gerados pelas outras strings.

    bool notSubtring = false;
    int n;
    for (int j = 1; j < k; j++) {
    	n = 0;
    	for(int i=0;i<in0.size();i++) {
        	if(vecSA.at(j).edges[n].find(in0[i]) == vecSA.at(j).edges[n].end()) {

            	notSubtring = true;
            	break;
            }
            n = vecSA.at(j).edges[n][in0[i]];
        }
    }
    if (!notSubtring) {
        return in0.size();
    }

    string sub_in0;
    int lenghtTest = 1;
    int lcs = 0;
    for (int z = 0; z <= in0.size(); z++) {
        sub_in0 = in0.substr(z,lenghtTest);
        notSubtring = false;

        n = 0;
        for (int j = 1; j < k; j++) {
            n = 0;
           for(int i=0;i<sub_in0.size();i++) {
                if(vecSA.at(j).edges[n].find(sub_in0[i]) == vecSA.at(j).edges[n].end()) {
                    notSubtring = true;
                    break; // Para de testar esse AS.
                }
            n = vecSA.at(j).edges[n][sub_in0[i]];
            }
            if (notSubtring) {
                break; // Nao testa mas nenhuma AS
            }
        }
        if (!notSubtring && lcs < sub_in0.size()) {
            lenghtTest = sub_in0.size() +1;
            if (z !=0){
            	z = z-1;
            }
            lcs = sub_in0.size();
        }
        if (lenghtTest+z >= in0.size()) {  // Nao vai ser possivel achar nenhuma substring maior do já encontrado.
            return lcs;
        }
    }
    return lcs;
}

vector<string> stringInput;
vector<SuffixAutomaton> vecSA;
char a[10001];

int main ()
{
    int out = 0;
    int t;
    int k;
    scanf("%d", &t);
    for (int i = 0; i < t; i++) {
        scanf("%d", &k);
        stringInput.clear();
        vecSA.clear();
        if (k == 1) {
            scanf("%10000s", a);
            string b = a;
            out = b.size();
            printf("%d\n", out);
        } else {
            for (int j = 0; j < k; j++) {
                scanf("%s", a);
                stringInput.push_back(a);
                vecSA.push_back(SuffixAutomaton (stringInput[j]));
            }
            out = LongestCS (vecSA, stringInput[0], k);
            printf("%d\n", out);
        }
    }
    return 0;
}
