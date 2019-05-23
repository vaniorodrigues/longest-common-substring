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
int Substring(string s, string w) {// S é a string que o automato de suffixo reconhece, e w é a string que será testada.
    SuffixAutomaton a(s);
    bool fail = false;
    int n = 0;
    //int len_w = 0;
    for(int i=0;i<w.size();i++) {
        if(a.edges[n].find(w[i]) == a.edges[n].end()) {
            fail = true;
            return 0;
            //break;
        }
        n = a.edges[n][w[i]];
    }
    return w.size();
}

int LongestCS(int k,vector<string> stringInput){ // A primeira string vai ser testada nos Automatos de Sufixos gerados pelas outras strings.
    int substringLenght = 0;
    string in0 = stringInput[0]; // Usado só pra facilitar a nomenclatura
    if (k==1){
        return stringInput[0].size();
    }

    vector<SuffixAutomaton> vecSA;
    for (int i = 0; i < k; i++) { //  Cria um AS para a segunda string em diante.
       vecSA.push_back(SuffixAutomaton (stringInput[i]));
    }


    int isSubstri =1;
    for (int j = 1; j < k; j++) { // Teste se a primeira string é um substring das outras strigs.
        //--- Substring Function
        int n = 0;
        for(int i = 0; i < stringInput[0].size(); i++) {
            if(vecSA[j].edges[n].find(stringInput[0][i]) == vecSA[j].edges[n].end()) {
                std::cout << "/* laço 1 */" << '\n';
                isSubstri = 0;
            }
            n = vecSA[j].edges[n][stringInput[0][i]];
        }
    }

    if ( isSubstri !=0 ) { //Se isSubstri é diferente de 0, significa que a primeira string é uma substring de todas as strings de entrada.
        return in0.size();
    }

     //isSubstri =1;
    for (int j = 1; j < k; j++) { // Teste se a primeira string é um substring das outras strigs.
        //--- Substring Function
        int n = 0;
        for(int i = 0; i < stringInput[0].size(); i++) {
            if(vecSA[j].edges[n].find(stringInput[0][i]) == vecSA[j].edges[n].end()) {
                std::cout << "/* laço 2 */" << '\n';
                isSubstri = 0;
            }
            n = vecSA[j].edges[n][stringInput[0][i]];
        }
    }

    if ( isSubstri !=0 ) { //Se isSubstri é diferente de 0, significa que a primeira string é uma substring de todas as strings de entrada.
        return in0.size();
    }

    // //vector<string> sub_in0;
    // for (int l = in0.size()-1; l > 0; l--) { // Comprimento do substring vai de w.size -1, até 1. Sendo que as possibilidades de i vai depender do comrpimento l.;
    //     for (int i = 0; i <= in0.size() - l; i++) { // i vai de 0 até a diferença entre o tamanho e o comprimento da string
    //         string sub_in0 = in0.substr(i,l);
    //         //int isSubstri =1;
    //         int isSubstri =1;
    //         for (int j = 1; j < k; j++) { // Teste se alguma  dada substring de também é uma substring das outras strings. Começa em 1 já que k = 0 é a primeira string, sendo desnecessário este testa-lá.
    //             std::cout << "j"<< j << '\n';
    //              // Teste se a primeira string é um substring das outras strigs.
    //                 //--- Substring Function
    //                 int n = 0;
    //                 for(int g = 0; g < in0.size(); g++) {
    //                     std::cout << "/* message */" << '\n';
    //                     if(vecSA[j].edges[n].find(in0[g]) == vecSA[j].edges[n].end()) {
    //
    //                         isSubstri = 0;
    //                         //break;
    //                     }
    //                     n = vecSA[j].edges[n][in0[g]];
    //                 }
    //
    //             // Se um dos valores de substring for zero, nao vai pra saida.
    //         }
    //         if ( isSubstri !=0 ) { //Se isSubstri é diferente de 0, significa que a última substring testado é uma substring de todas as strings de entrada.
    //             return sub_in0.size();
    //              // Hora de sair da função.
    //         }
    //     }
    // }
    end:
    return 0; // Retorna o valor do comprimento da maior substring comum entre as strings de entrada.
}
/* */
int main ( int argc, char *argv[ ])
{

    int out;
    int t;
    int k;
    char a[10][10001];
    scanf("%d", &t);
    for (int i = 0; i < t; i++) {
        scanf("%d", &k);
        vector<string> stringInput;
        for (int j = 0; j < k; j++) {
            scanf("%10000s", a[i]);
            stringInput.push_back(a[i]);
            //std::cout << "input  " << stringInput[0]<< endl;
        }
        out = LongestCS (k, stringInput);
        printf("%d\n", out);

    }

    return 0;
}

// Resposta errada
