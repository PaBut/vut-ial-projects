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
  if(tree == NULL){
    return false;
  }

  if(tree->key == key) {
    *value = tree->value;
    return true;
  }
  else if(key < tree->key){
    return bst_search(tree->left, key, value);
  }
  else{
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
  if(!*tree){
    if(!((*tree) = malloc(sizeof(bst_node_t)))) return;
      (*tree)->key = key;
      (*tree)->value = value;
      (*tree)->left = NULL;
      (*tree)->right = NULL;
  }
  if((*tree)->key == key){
    if((*tree)->key == 0) (*tree)->key = key;
    (*tree)->value = value;
  }
  else if(key < (*tree)->key){
    if((*tree)->left != NULL){
      bst_insert(&(*tree)->left, key, value);
    }
    else{
      if(!((*tree)->left = malloc(sizeof(bst_node_t)))) return;
      (*tree)->left->key = key;
      (*tree)->left->value = value;
      (*tree)->left->left = NULL;
      (*tree)->left->right = NULL;
    }
  }
  else if(key > (*tree)->key){
    if((*tree)->right != NULL){
      bst_insert(&(*tree)->right, key, value);
    }
    else{
      if(!((*tree)->right = malloc(sizeof(bst_node_t)))) return;
      (*tree)->right->key = key;
      (*tree)->right->value = value;
      (*tree)->right->left = NULL;
      (*tree)->right->right = NULL;
    }
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
  if((*tree)->right){
    bst_replace_by_rightmost(target, &(*tree)->right);
    if((*tree)->right->key == target->key){
      free((*tree)->right);
      (*tree)->right = NULL;
    }
  } 
  else{
    target->key = (*tree)->key;
    target->value = (*tree)->value;
    if((*tree)->left){
      bst_node_t* temp = (*tree)->left;
      (*tree)->key = temp->key;
      (*tree)->value = temp->value;
      (*tree)->right = temp->right;
      (*tree)->left = temp->left;
      free(temp);
    }else{
      (*tree)->key = target->key;
    }
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
  if(!*tree){
      printf("///////////////////////\n");
    return;
  }
  printf("%c\n", (*tree)->key);
  if((*tree)->key == key){
    if(!(*tree)->left && !(*tree)->right){
      return;
    }
    else if((*tree)->left && (*tree)->right){
      bst_replace_by_rightmost(*tree, &(*tree)->left);
    }
    else if((*tree)->left){
      bst_node_t* temp = (*tree)->left;
      (*tree)->key = temp->key;
      (*tree)->value = temp->value;
      (*tree)->right = temp->right;
      (*tree)->left = temp->left;
      free(temp);
    }else{
      bst_node_t* temp = (*tree)->right;
      (*tree)->key = temp->key;
      (*tree)->value = temp->value;
      (*tree)->right = temp->right;
      (*tree)->left = temp->left;
      free(temp);
    }
    
  }
  else if(key < (*tree)->key){
    bst_delete(&(*tree)->left, key);
    if((*tree)->left && (*tree)->left->key == key){
      free((*tree)->left);
      (*tree)->left = NULL;
    }
  } 
  else{
    bst_delete(&(*tree)->right, key);
    if((*tree)->right && (*tree)->right->key == key){
      free((*tree)->right);
      (*tree)->right = NULL;
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
  if(*tree){
    bst_dispose(&(*tree)->left);
    bst_dispose(&(*tree)->right);
    free(*tree);
    *tree = NULL;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte rekurzivně bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  if(tree){
    bst_add_node_to_items(tree, items);
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
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
  if(tree){
    bst_inorder(tree->left, items);
    bst_add_node_to_items(tree, items);
    bst_inorder(tree->right, items);
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
  if(tree){
    bst_postorder(tree->left, items);
    bst_postorder(tree->right, items);
    bst_add_node_to_items(tree, items);
  }
}
