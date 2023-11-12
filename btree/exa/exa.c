/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
  int i = 0;
  int count = 0;
  char ch;
  
  while (input[i] != '\0') {
    ch = input[i];
    count = 1; // pokud se klic nenajde count bude nastaveny na 1 
    
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ') {
      if ((ch >= 'A' && ch <= 'Z')) {
        ch += 32; // prevede uppercase na lowercase
      }

    } else { // jiny znak
      ch = '_';
    }

    bool found = bst_search(*tree, ch, &count);

    if (found) { //pokud byl uzel s klicem nalezen - prida se 1 k aktualnimu poctu
      count++;
    }

    bst_insert(tree, ch, count);
      
    i++;
  }
}

 /*
  bst_node_t *balance_tree_with_inorder(bst_items_t *items, int from, int to) {
    if (to < from)
      return NULL;

    int mid = (from + to) / 2;

    bst_node_t *rootNode = items->nodes[mid];

    rootNode->left = balance_tree_with_inorder(items, from, --mid);
    rootNode->right = balance_tree_with_inorder(items, ++mid, to);

    return rootNode;
  }*/


/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
  /*
  // serazeny seznam hodnot
  bst_items_t items = {
    .size = 0,
    .capacity = 0,
    .nodes = malloc(sizeof(bst_node_t *))
  };

  if (items.nodes == NULL)
    return;
  
  bst_inorder(*tree, &items);


  int from = 0;
  int to = items.size - 1;

  *tree = balance_tree_with_inorder(&items, from, to);*

  free(items.nodes);*/
}
