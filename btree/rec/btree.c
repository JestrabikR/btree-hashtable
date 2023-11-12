/*
 * Binární vyhledávací strom — rekurzivní varianta
 *
 * S využitím datových typů ze souboru btree.h a připravených koster funkcí
 * implementujte binární vyhledávací strom pomocí rekurze.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  *tree = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte rekurzivně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL)
    return false;

  if (tree->key == key) {
    *value = tree->value;
    return true;
  } else if (tree->key > key) {
    return bst_search(tree->left, key, value);
  } else {
    return bst_search(tree->right, key, value);
  }    
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (*tree == NULL) { // vlozime listovy uzel
    *tree = (bst_node_t *)malloc(sizeof(bst_node_t));
    if (*tree == NULL)
      return;
    
    (*tree)->value = value;
    (*tree)->key = key;
    (*tree)->right = NULL;
    (*tree)->left = NULL;
    return;
  }

  if ((*tree)->key > key) { // vkladany uzel je mensi -> jdeme doleva
    bst_insert(&(*tree)->left, key, value);
  } else if ((*tree)->key < key) { // vkladany uzel je vetsi -> jdeme doprava
    bst_insert(&(*tree)->right, key, value);
  } else {
    (*tree)->value = value;
  }

}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazeny klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využitá při implementaci funkce bst_delete.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (target == NULL)
    return;
  
  if ((*tree)->right == NULL) { // je nejpravejsi
    target->value = (*tree)->value;
    target->key = (*tree)->key;

    free((*tree));
    *tree = NULL;
  } else {
    bst_replace_by_rightmost(target, &(*tree)->right);
  }
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte rekurzivně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if ((*tree) == NULL)
    return; 
  
  if ((*tree)->key > key) { // hledany uzel je vlevo
    bst_delete(&(*tree)->left, key);
  } else if ((*tree)->key < key) { //hledany uzel je vpravo
    bst_delete(&(*tree)->right, key);
  } else { // uzly se rovnaji

    if ((*tree)->left == NULL && (*tree)->right == NULL) { //uzel nema ani jeden podstrom
      free(*tree);
      *tree = NULL;
      
    } else if ((*tree)->left != NULL && (*tree)->right != NULL) { // uzel ma oba podstromy
      bst_replace_by_rightmost(*tree, &(*tree)->left); // *tree bude nahrazen nejpravejsim uzlem leveho podstromu ((*tree)->left)
    
    } else if ((*tree)->left != NULL && (*tree)->right == NULL) { // uzel ma levy podstrom
      bst_node_t *deleted_node = *tree;
      *tree = (*tree)->left;

      free(deleted_node);
      deleted_node = NULL;

    } else { // uzel ma pravy podstrom
      bst_node_t *deleted_node = *tree;
      *tree = (*tree)->right;

      free(deleted_node);
      deleted_node = NULL;
    }
  }
}

/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  if (tree == NULL || (*tree) == NULL)
    return;

  bst_dispose(&(*tree)->left);
  bst_dispose(&(*tree)->right);
  
  free(*tree);
  *tree = NULL;
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (items == NULL)
    return;

  if (tree != NULL) {
    bst_add_node_to_items (tree, items);
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (items == NULL)
    return;

  if (tree != NULL) {
    bst_preorder(tree->left, items);
    bst_add_node_to_items (tree, items);
    bst_preorder(tree->right, items);
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  if (items == NULL)
    return;

  if (tree != NULL) {
    bst_preorder(tree->left, items);
    bst_preorder(tree->right, items);
    bst_add_node_to_items (tree, items);
  }
}
