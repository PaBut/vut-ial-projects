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
  bool search = false;
  bool finish = tree == NULL;

  while(!finish){
    if(tree->key == key){
      *value = tree->value;
      finish = true;
      search = true;
    }
    else if(key < tree->key){
      tree = tree->left;
    }
    else{
      tree = tree->right;
    }
    if(tree == NULL){
      finish = true;
    }
  }

  return search;
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
  bst_node_t** node = tree;
  if(!*node){
    if(!((*node) = malloc(sizeof(bst_node_t)))) return;
    (*node)->key = key;
    (*node)->value = value;
    (*node)->left = NULL;
    (*node)->right = NULL;
    return;
  }

  while(true){
    if(key < (*node)->key){
      if((*node)->left == NULL){
        bst_node_t* temp = malloc(sizeof(bst_node_t));
        if(!temp) return;
        temp->key = key;
        temp->value = value;
        temp->left = NULL;
        temp->right = NULL;
        (*node)->left = temp;
        return;
      }
      else{
        node = &(*node)->left;
      }
    }
    else if(key > (*node)->key){
      if((*node)->right == NULL){
        bst_node_t* temp = malloc(sizeof(bst_node_t));
        if(!temp) return;
        temp->key = key;
        temp->value = value;
        temp->left = NULL;
        temp->right = NULL;
        (*node)->right = temp;
        return;
      }
      else{
        node = &(*node)->right;
      }
    }
    else{
      (*node)->key = key;
      (*node)->value = value;
      return;
    }
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
  bst_node_t** node = tree;
  bst_node_t** parent = NULL;
  while((*node)->right != NULL){
    parent = node;
    node = &(*node)->right;
  }
  target->key = (*node)->key;
  target->value = (*node)->value;

  bst_node_t* lnode = (*node)->left;

  if((*node)->left){
    (*node)->key = lnode->key;
    (*node)->value = lnode->value;
    (*node)->left = lnode->left;
    (*node)->right = lnode->right;
    free(lnode);
  }

  if(*parent && (*parent)->right->key == target->key){
    free((*parent)->right);
    (*parent)->right = NULL;
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
  bst_node_t** node = tree;
  bool found = false;
  while(!found && *node){
    if(key < (*node)->key){
      node = &(*node)->left;
    }   
    else if(key > (*node)->key){
      node = &(*node)->right;
    }
    else{
      if((*node)->left && (*node)->right){
        bst_replace_by_rightmost(*node, &(*node)->left);
      }
      else if(!(*node)->left && !(*node)->right){
        free(*node);
        *node = NULL;
      }
      else{
        bst_node_t* temp = (*node)->left ? (*node)->left : (*node)->right;
        (*node)->key = temp->key;
        (*node)->value = temp->value;
        (*node)->left = temp->left;
        (*node)->right = temp->right;
        free(temp);
      }
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
  stack_bst_t* stack = malloc(sizeof(stack_bst_t));
  if(!stack) return;
  stack_bst_init(stack);
  bst_node_t* node = *tree;
  while(node){
    stack_bst_push(stack, node);
    node = node->left;
  }

  while(!stack_bst_empty(stack)){
    node = stack_bst_pop(stack);
    if(node->right){
      bst_node_t* temp = node->right;
      while(temp){
        stack_bst_push(stack, temp);
        temp = temp->left;
      }
    }
    free(node);
  }
  free(stack);
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
  while(tree){
    stack_bst_push(to_visit, tree);
    bst_add_node_to_items(tree, items);
    tree = tree->left;
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
  stack_bst_t* stack = malloc(sizeof(stack_bst_t));
  if(!stack) return;
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack, items);
  while(!stack_bst_empty(stack)){
    tree = stack_bst_pop(stack);
    bst_leftmost_preorder(tree->right, stack, items);
  }
  free(stack);
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
  while(tree){
    stack_bst_push(to_visit, tree);
    tree = tree->left;
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
  stack_bst_t* stack = malloc(sizeof(stack_bst_t));
  if(!stack) return;
  stack_bst_init(stack);
  bst_leftmost_inorder(tree, stack);
  while(!stack_bst_empty(stack)){
    tree = stack_bst_pop(stack);
    bst_add_node_to_items(tree, items);
    bst_leftmost_inorder(tree->right, stack);
  }
  free(stack);
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
  while(tree){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit, true);
    tree = tree->left;
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
  stack_bst_t* n_stack = malloc(sizeof(stack_bst_t));
  if(!n_stack) return;
  stack_bst_init(n_stack);
  stack_bool_t* b_stack = malloc(sizeof(stack_bool_t));
  if(!b_stack) return;
  stack_bool_init(b_stack);
  bool fromLeft = false;
  bst_leftmost_postorder(tree, n_stack, b_stack);
  while(!stack_bst_empty(n_stack)){
    tree = stack_bst_top(n_stack);
    fromLeft = stack_bool_pop(b_stack);
    if(fromLeft){
      stack_bool_push(b_stack, false);
      bst_leftmost_postorder(tree->right, n_stack, b_stack);
    }
    else{
      stack_bst_pop(n_stack);
      bst_add_node_to_items(tree, items);
    }
  }
  free(n_stack);
  free(b_stack);
}
