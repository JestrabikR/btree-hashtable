/*
 * Tabulka s rozptýlenými položkami
 *
 * S využitím datových typů ze souboru hashtable.h a připravených koster
 * funkcí implementujte tabulku s rozptýlenými položkami s explicitně
 * zretězenými synonymy.
 *
 * Při implementaci uvažujte velikost tabulky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptylovací funkce která přidělí zadanému klíči index z intervalu
 * <0,HT_SIZE-1>. Ideální rozptylovací funkce by měla rozprostírat klíče
 * rovnoměrně po všech indexech. Zamyslete sa nad kvalitou zvolené funkce.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializace tabulky — zavolá sa před prvním použitím tabulky.
 */
void ht_init(ht_table_t *table) {
  for (int i = 0; i < MAX_HT_SIZE; i++)
    (*table)[i] = NULL; //nastavi vsechny polozky tabulky na NULL
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int index = get_hash(key);

  ht_item_t *item = (*table)[index];

  while (item != NULL) {
    if (strcmp(item->key, key) == 0) { // klice jsou stejne
      return item;
    }

    item = item->next; // dalsi item
  }

  return NULL;
}

/*
 * Vložení nového prvku do tabulky.
 *
 * Pokud prvek s daným klíčem už v tabulce existuje, nahraďte jeho hodnotu.
 *
 * Při implementaci využijte funkci ht_search. Pri vkládání prvku do seznamu
 * synonym zvolte nejefektivnější možnost a vložte prvek na začátek seznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  ht_item_t *item = ht_search(table, key);
  
  if (item) { // pokud byl nalezen
    item->value = value;
  } else { // pridani noveho prvku
    int index = get_hash(key);
    item = (*table)[index];

    ht_item_t* new_item = (ht_item_t *)malloc(sizeof(ht_item_t));
    if (new_item == NULL)
      return;

    new_item->key = key;
    
    new_item->value = value;

    new_item->next = item;

    (*table)[index] = new_item;

  }
}

/*
 * Získání hodnoty z tabulky.
 *
 * V případě úspěchu vrací funkce ukazatel na hodnotu prvku, v opačném
 * případě hodnotu NULL.
 *
 * Při implementaci využijte funkci ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item = ht_search(table, key);

  if (item == NULL)
    return NULL;

  return &item->value;
}

/*
 * Smazání prvku z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje přiřazené k danému prvku.
 * Pokud prvek neexistuje, funkce nedělá nic.
 *
 * Při implementaci NEPOUŽÍVEJTE funkci ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  int index = get_hash(key);
  ht_item_t *item = (*table)[index];

  if (item == NULL)
    return;

  ht_item_t *previous_item;
  
  while (item != NULL) { // pokud to neni prvni prvek
    if (strcmp(item->key, key) == 0) {
      if (previous_item == NULL) { // je to prvni prvek
        (*table)[index] = item->next; // nastavime na prvni prvek item->next
        
        free(item);
        item = NULL;
        return;
      }

      previous_item->next = item->next; // spojime predchazejici prvek s prvkem nasledujicim po vymazanem
      
      free(item);
      item = NULL;

      return;
    }

    previous_item = item;
    item = item->next;
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  for (int i = 0; i < MAX_HT_SIZE; i++) {
    ht_item_t *item = (*table)[i];

    while (item != NULL) {
      ht_item_t *current_item = item;
      item = item->next; // pred smazanim posuneme na dalsi

      free(current_item);
    }

    (*table)[i] = NULL;
  }
}
