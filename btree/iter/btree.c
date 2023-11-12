/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
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
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t *item = tree;

  while (item != NULL) {
    if (item->key == key) {
      *value = item->value;
      return true;

    } else if (item->key > key) { // hledany klic je mensi => hledame vlevo
      item = item->left;

    } else { //hledany klic je vetsi => hledame vpravo
      item = item->right;
    }
  }

  return false;
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
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t *item;

  bst_node_t *newNode = (bst_node_t *)malloc(sizeof(bst_node_t));
  if (newNode == NULL)
    return;

  newNode->key = key;
  newNode->value = value;
  newNode->right = NULL;
  newNode->left = NULL;

  // vyhledavani
  if (*tree != NULL) { // strom neni prazdny
    item = *tree;
    
    while (item != NULL) {
      if (item->key == key) { // uzel byl nalezen
        free(newNode); // nebyl potreba
        item->value = value;
        return;

      } else if (item->key > key) { // vkladany klic je mensi
        if (!item->left) { // nema leveho syna
          item->left = newNode;
          return;
        }
        item = item->left; // pokracuje se doleva

      } else { // vkladany klic je vetsi
        if (!item->right) { // nema praveho syna
          item->right = newNode;
          return;
        }
        item = item->right; // pokracuje se doprava
      }
    }
  } else { // strom je prazdny
    *tree = newNode;
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *item = *tree;
  bst_node_t *previous = NULL;

  while (item != NULL) {
    if (item->right == NULL) { // je nejpravejsi
      target->value = item->value;
      target->key = item->key;

      if (previous != NULL) { 
        previous->right = item->left; // pripojeni leveho uzlu nejpravejsiho uzlu k predchozimu
      } else {
        *tree = item->left; 
      }
      
      free(item);
      item = NULL;
      break;
    }

    previous = item;
    item = item->right;
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
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *previous;
  bst_node_t *item = *tree; // aktualni uzel

  while (item) {
    if (item->key > key) { // hledany uzel je vlevo
      previous = item;
      item = item->left;
    } else if (item->key < key) { //hledany uzel je vpravo
      previous = item;
      item = item->right;
    } else { // uzel byl nalezen
      if (!item->left && !item->right) { //uzel nema ani jeden podstrom
        if (previous == NULL) { // je to korenovy uzel
          *tree = NULL;
        } else {
          if (previous->right == item) { // aktualni uzel jde zprava
            previous->right = NULL;
          } else { // aktualni uzel jde zleva
            previous->left = NULL;
          }
        }

        free(item);
        item = NULL;
        
      } else if (item->left && item->right) { // uzel ma oba podstromy
        bst_replace_by_rightmost(item, &item->left); // item bude nahrazen nejpravejsim uzlem leveho podstromu (item->left)
      
      } else if (item->left && !item->right) { // uzel ma levy podstrom
        if (previous == NULL) { // je to korenovy uzel
          *tree = NULL;
        } else {
          if (previous->right == item) { // aktualni uzel jde zprava
            previous->right = item->left;
          } else { // aktualni uzel jde zleva
            previous->left = item->left;
          }
        }

        free(item);
        item = NULL;

      } else { // uzel ma pravy podstrom
        if (previous == NULL) { // je to korenovy uzel
          *tree = NULL;
        } else {
          if (previous->right == item) { // aktualni uzel jde zprava
            previous->right = item->right;
          } else { // aktualni uzel jde zleva
            previous->left = item->right;
          }
        }

        free(item);
        item = NULL;
      }
      
      break;
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
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {
  // prochazi stromem a pridava levy a pravy ukazatel (pokud existuji) do stacku
  // ze stacku se potom vybiraji ukazatele dokud neni prazdny a rusi se

  if (tree == NULL || *tree == NULL)
    return;

  stack_bst_t stack;
  stack_bst_init(&stack);

  stack_bst_push(&stack, *tree);

  bst_node_t *top;

  while (stack_bst_empty(&stack) == false) {

    top = stack_bst_pop(&stack);

    if (top->right != NULL) {
      stack_bst_push(&stack, top->right);
    }

    if (top->left != NULL) {
      stack_bst_push(&stack, top->left);
    }

    free(top);
  }

  *tree = NULL;
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  if (tree == NULL)
    return;
  
  bst_node_t *item = tree;

  while (item != NULL) {
    stack_bst_push(to_visit, item);

    bst_add_node_to_items(item, items);

    item = item->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL)
    return;
  
  stack_bst_t toVisit;
  stack_bst_init(&toVisit);

  bst_leftmost_preorder(tree, &toVisit, items);

  bst_node_t *top;

  while (stack_bst_empty(&toVisit) == false) {
    top = stack_bst_pop(&toVisit);
    bst_leftmost_preorder(top->right, &toVisit, items);
  }

}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  if (tree == NULL)
    return;
  
  bst_node_t *item = tree;

  while (item != NULL) {
    stack_bst_push(to_visit, item);
    item = item->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  if (tree == NULL)
    return;
  
  stack_bst_t toVisit;
  stack_bst_init(&toVisit);

  bst_leftmost_inorder(tree, &toVisit);

  bst_node_t *top;

  while (stack_bst_empty(&toVisit) == false) {
    top = stack_bst_pop(&toVisit);
    bst_add_node_to_items(top, items);
    bst_leftmost_inorder(top->right, &toVisit);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  if (tree == NULL)
    return;

  bst_node_t *item = tree;

  while (item != NULL) {
    stack_bst_push(to_visit, item);
    stack_bool_push(first_visit, true);
    item = item->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t toVisit; // zasobnik ukazatelu, ke kterym se bude vracet
  stack_bst_init(&toVisit);

  stack_bool_t firstVisit; // zasobnik boolu, true = navrat zleva
  stack_bool_init(&firstVisit);

  bst_node_t *item = tree;

  bool fromLeft;

  bst_leftmost_postorder(item, &toVisit, &firstVisit);

  while (stack_bst_empty(&toVisit) == false) {
    item = stack_bst_top(&toVisit);
    fromLeft = stack_bool_top(&firstVisit);
    stack_bool_pop(&firstVisit);

    if (fromLeft) { // jeste nebyl vpravo
      stack_bool_push(&firstVisit, false);
      bst_leftmost_postorder(item->right, &toVisit, &firstVisit);
    } else { // uz byl vlevo i vpravo
      stack_bst_pop(&toVisit);
      bst_add_node_to_items(item, items);
    }
  }
}
