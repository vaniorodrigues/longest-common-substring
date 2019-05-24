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

vector<int> sort_cyclic_shifts(string const& s) {
    int n = s.size();
    const int alphabet = 256;
    vector<int> p(n), c(n), cnt(max(alphabet, n), 0);
    for (int i = 0; i < n; i++)
        cnt[s[i]]++;
        for (int i = 1; i < alphabet; i++)
            cnt[i] += cnt[i-1];
            for (int i = 0; i < n; i++)
                p[--cnt[s[i]]] = i;
                c[p[0]] = 0;
    int classes = 1;
    for (int i = 1; i < n; i++) {
        if (s[p[i]] != s[p[i-1]])
            classes++;
        c[p[i]] = classes - 1;
    }
    vector<int> pn(n), cn(n);
    for (int h = 0; (1 << h) < n; ++h) {
        for (int i = 0; i < n; i++) {
            pn[i] = p[i] - (1 << h);
            if (pn[i] < 0)
                pn[i] += n;
            }
        fill(cnt.begin(), cnt.begin() + classes, 0);
        for (int i = 0; i < n; i++)
            cnt[c[pn[i]]]++;
        for (int i = 1; i < classes; i++)
            cnt[i] += cnt[i-1];
        for (int i = n-1; i >= 0; i--)
            p[--cnt[c[pn[i]]]] = pn[i];
            cn[p[0]] = 0;
            classes = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> cur (c[p[i]], c[(p[i] + (1 << h)) % n]);
            pair<int, int> prev (c[p[i-1]], c[(p[i-1] + (1 << h)) % n]);
            if (cur != prev)
                ++classes;
            cn[p[i]] = classes - 1;
        }
        c.swap(cn);
    }
    return p;
}

vector<int> suffix_array_construction(string s) {
    s += "$";
    vector<int> sorted_shifts = sort_cyclic_shifts(s);
    sorted_shifts.erase(sorted_shifts.begin());
    return sorted_shifts;
}

vector<int> lcp_construction(string const& s, vector<int> const& p) {
    int n = s.size();
    vector<int> rank(n, 0);
    for (int i = 0; i < n; i++)
        rank[p[i]] = i;

    int k = 0;
    vector<int> lcp(n-1, 0);
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) {
            k = 0;
            continue;
        }
        int j = p[rank[i] + 1];
        while (i + k < n && j + k < n && s[i+k] == s[j+k])
            k++;
        lcp[rank[i]] = k;
        if (k)
            k--;
    }
    return lcp;
}

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

    for (int l = in0.size()-1; l > 0; l--) {
        for (int in = 0; in <= in0.size() - l; in++) {
            sub_in0 = in0.substr(in,l);
            notSubtring = false;
            for (int j = 1; j < k; j++) {
                n = 0;
                for(int i=0;i<sub_in0.size();i++) {
                    if(vecSA.at(j).edges[n].find(sub_in0[i]) == vecSA.at(j).edges[n].end()) {
                        //std::cout << "in['%d']  "<< i <<'\t'<< sub_in0 << '\n';
                        notSubtring = true;
                        break;
                    }
                n = vecSA.at(j).edges[n][sub_in0[i]];
                }
                if (notSubtring) {
                    break;
                }
            }
            if (!notSubtring) {
                return sub_in0.size();
            }
        }
    }
    return 0;
}

vector<string> stringInput;
vector<SuffixAutomaton> vecSA;
char a[10000];

int main ()
{
    int out = 0;
    int t;
    int k;
    scanf("%d", &t);
    //vector<string> stringInput; //  Local onde é armazenado as strings de entrada.
    for (int i = 0; i < t; i++) {
        scanf("%d", &k);
        stringInput.clear();
        vecSA.clear();
        if (k == 1) {
            scanf("%s", a);
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
