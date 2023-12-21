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
  for(int i = 0; i < HT_SIZE; i++){
    (*table)[i] = NULL;
  }
}

/*
 * Vyhledání prvku v tabulce.
 *
 * V případě úspěchu vrací ukazatel na nalezený prvek; v opačném případě vrací
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int index = get_hash(key);
  for(ht_item_t* item = (*table)[index]; item != NULL; item = item->next){
    if(item->key && !strcmp(item->key, key)){
      return item;
    }
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

  ht_item_t* found_element = ht_search(table, key);

  if(found_element){
    found_element->value = value;
    return;
  }

  int index = get_hash(key);

  ht_item_t* item = (*table)[index];

  if(item == NULL){
    (*table)[index] = malloc(sizeof(ht_item_t));
    item = (*table)[index];
    if(!item) return;
    item->key = malloc(sizeof(char) * (strlen(key) + 1));
    if(!item->key) return;
    strcpy(item->key, key);
    item->value = value;
    item->next = NULL;
  } 
  else{
    for(;item->next != NULL; item = item->next);

    item->next = malloc(sizeof(ht_item_t));

    item = item->next;

    if(!item) return;

    item->key = malloc(sizeof(char) * (strlen(key) + 1));
    if(!item->key) return;
    strcpy(item->key, key);
    item->value = value;
    item->next = NULL;
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
  ht_item_t* item = ht_search(table, key);
  return item ? &item->value : NULL;
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

  for(ht_item_t* item = (*table)[index]; item != NULL; item = item->next){
    if(!strcmp(item->key, key)){
      if(item->next){
        ht_item_t* next = item->next;
        item->value = next->value;
        free(item->key);
        item->key = next->key;
        item->next = next->next;
        free(next);
      }
    }
  }
}

/*
 * Smazání všech prvků z tabulky.
 *
 * Funkce korektně uvolní všechny alokované zdroje a uvede tabulku do stavu po 
 * inicializaci.
 */
void ht_delete_all(ht_table_t *table) {
  for(int i = 0; i < HT_SIZE; i++){
    ht_item_t* item = (*table)[i];

    while(item){
      ht_item_t* tmp = item->next;
      free(item->key);
      free(item);
      item = tmp;
    }
    (*table)[i] = NULL;
  }
}
