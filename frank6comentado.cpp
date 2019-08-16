#include <bits/stdc++.h>
using namespace std;


struct SuffixAutomaton {
  vector<map<char,int> > edges; // Lista de transições de saída
  vector<int> link;            // Lista de transições de entrada
  vector<int> length;          // Comprimento da maior substring endpos-equivalente
  int last;                    // the index of the equivalence class of the whole string

  SuffixAutomaton(string s) {
    // Adiciona o estado inicial
    edges.push_back(map<char,int>()); 
    link.push_back(-1);      // Link de entrada é um estado imaginário         
    length.push_back(0);     // Como é o estado inicial, não apresenta comprimento         
    last = 0;                 // Último estado antes da adição de uma novo caractere.                   

    for(int i=0;i<s.size();i++) {
      // Cria um novo estado para cada letra
      edges.push_back(map<char,int>());
      length.push_back(i+1); // Comprimento(estadoAtual) vai comprimento(estadoAnterior) +1
      link.push_back(0);    // Nao se sabe ainda qual é o link
      int r = edges.size() - 1;

      // O laço while procura a existência de transições que utilizam o caractere sendo adicionado.
      int p = last; // Estado do caracter adicionado
      while(p >= 0 && edges[p].find(s[i]) == edges[p].end()) {
        edges[p][s[i]] = r;
        p = link[p]; 
      }
      if(p != -1) {
        int q = edges[p][s[i]]; // q é o maior substring dentre as endpos-equivalente do estado.
        if(length[p] + 1 == length[q]) { // Se essa comparação for verdadeira estamos na maior string das
        //endpos-equivalentes, basta copiar as transições e ir para o próximo caracter.
          link[r] = q;
        } else { // Indica que os estados p e q não apresentam o mesmo caminho. 
        // É necessário criar um novo estado qq e copiar todas as informações de q para qq, menos o comprimento.
        // Já que o comprimento de qq vai ser o comprimento(p)+1
          edges.push_back(edges[q]); // copy edges of q
          length.push_back(length[p] + 1);
          link.push_back(link[q]); // copy parent of q
          int qq = edges.size()-1;
          link[q] = qq;
          link[r] = qq;
          
          while(p >= 0 && edges[p][s[i]] == q) { // Verifica se a transições do caractere adicinado que leve
          // do estado p para q, e as direcionada para o estado qq.
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
        if (!notSubtring && lcs < sub_in0.size()) { // Se a substring testado é uma substring de todas as outras substrings então atualiza o valor de lcs e aumenta o valor do lenght de teste. 
            lenghtTest = sub_in0.size() +1;
            if (z !=0){
            	z = z-1;
            }
            lcs = sub_in0.size(); // Novo valor da Longest Common Substring
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
        stringInput.clear(); // Limpa os valores to teste case anterior
        vecSA.clear();
        if (k == 1) { // Caso só tenha uma string imprimi o tamanho da própria
            scanf("%10000s", a);
            string b = a;
            out = b.size();
            printf("%d\n", out);
        } else {
            for (int j = 0; j < k; j++) {
                scanf("%s", a);
                stringInput.push_back(a);
                vecSA.push_back(SuffixAutomaton (stringInput[j])); // Vetor que indexa os AS.
            }
            out = LongestCS (vecSA, stringInput[0], k); // Função retorna o lcs.
            printf("%d\n", out);
        }
    }
    return 0;
}
