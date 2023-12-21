/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
#include <stdio.h>

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {
	char* operator = malloc(sizeof(char));
	while(!Stack_IsEmpty(stack)){
		Stack_Top(stack, operator);
		Stack_Pop(stack);
		if(*operator == '('){
			break;
		}
		postfixExpression[(*postfixExpressionLength)++] = *operator;
	}
	free(operator);
}


bool is_higher_precedence(char a, char b){
	if(b == '(')
		return true;
	if(((a == '+' || a == '-') 
	&& (b == '+' || b == '-')) 
	|| ((a == '*' || a == '/') 
	&& (b == '*' || b == '/'))){
		return false;
	}
	if((a == '*' || a == '/')
	&& (b == '+' || b == '-'))
		return true;
	if((a == '+' || a == '-') &&
	(b == '*' || b == '/'))
		return false;
	return false;
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')){
		postfixExpression[(*postfixExpressionLength)++] = c;
	}
	else if(c == '('){
		Stack_Push(stack, c);
	}
	else if(c == ')'){
		untilLeftPar(stack, postfixExpression, postfixExpressionLength);
	}
	else if(c == '='){
		untilLeftPar(stack, postfixExpression, postfixExpressionLength);
		postfixExpression[(*postfixExpressionLength)++] = c;
	}
	else if(c == '+' || c == '-' || c == '*' || c == '/'){
		char* top = malloc(sizeof(char));

		if(!Stack_IsEmpty(stack)){
			Stack_Top(stack, top);
		}
		
		while(!Stack_IsEmpty(stack) && !is_higher_precedence(c, *top)){
			Stack_Pop(stack);
			postfixExpression[(*postfixExpressionLength)++] = *top;
		}

		Stack_Push(stack, c);

		free(top);
	}
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	char* postfixExpression = malloc(MAX_LEN * sizeof(char));
	if(postfixExpression == NULL){
		return NULL;
	}
	unsigned postfixExpressionLength = 0;
	Stack* stack = malloc(sizeof(Stack));
	if(stack == NULL){
		return NULL;
	}
	Stack_Init(stack);

	int i = 0;

	for(; infixExpression[i] != '\0'; i++){
		doOperation(stack, infixExpression[i], postfixExpression, &postfixExpressionLength);
	}
	postfixExpression[postfixExpressionLength] = '\0';

	Stack_Dispose(stack);
	free(stack);

	return postfixExpression;
}

const int CHAR_BIT = 8;

/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	int mask = 0xFF;
	for(size_t i = 0; i < sizeof(int); i++){
		char numberPart = value & mask << (i * CHAR_BIT);
		Stack_Push(stack, numberPart);
	}
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	*value = 0;
	for(int i = sizeof(int) - 1; i >= 0 ; i--){
		if(!Stack_IsEmpty(stack)){
			char temp = 0;
			Stack_Top(stack, &temp);
			Stack_Pop(stack);
			*value |= temp << CHAR_BIT * i;
		}
		else{
			*value = 0;
			return;
		}
	}
}

bool get_value(char name, VariableValue variableValues[], int variableValueCount, int* value){
	for(int i = 0; i < variableValueCount; i++){
		if(variableValues[i].name == name){
			*value = variableValues[i].value;
			return true;
		}
	}
	return false;
} 

/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
	char* postfixExpression = infix2postfix(infixExpression);
	if(postfixExpression == NULL)
		return false;

	Stack* stack = malloc(sizeof(Stack));
	if(stack == NULL){
		free(postfixExpression);
		return NULL;
	}
	Stack_Init(stack);

	for(int i = 0; postfixExpression[i] != '='; i++){
		if(postfixExpression[i] >= '0' && postfixExpression[i] <= '9'){
			int value = postfixExpression[i] - '0';
			expr_value_push(stack, value);
		}
		else if((postfixExpression[i] >= 'A' && postfixExpression[i] <= 'Z')
		|| (postfixExpression[i] >= 'a' && postfixExpression[i] <= 'z')){
			int value = 0;
			bool found = get_value(postfixExpression[i], variableValues, variableValueCount, &value);
			if(!found){
				Stack_Dispose(stack);
				free(stack);
				free(postfixExpression);
				return false;
			}
			expr_value_push(stack, value);
		}
		else if(postfixExpression[i] == '+' || postfixExpression[i] == '-' 
		|| postfixExpression[i] == '*' || postfixExpression[i] == '/'){
			int value1 = 0;
			int value2 = 0;

			expr_value_pop(stack, &value1);
			expr_value_pop(stack, &value2);

			int result_value;

			switch(postfixExpression[i]){
				case '+':
					result_value = value2 + value1;
					break;
				case '-':
					result_value = value2 - value1;
					break;
				case '*':
					result_value = value2 * value1;
					break;
				case '/':
					if(value1 == 0){
						free(postfixExpression);
						Stack_Dispose(stack);
						free(stack);
						return false;
					}
					result_value = value2 / value1;
					break;
			}

			expr_value_push(stack, result_value);
		}
		else if(postfixExpression[i] == '\0'){
			free(postfixExpression);
			Stack_Dispose(stack);
			free(stack);
			return false;
		}
	}

	int result = 0;
	expr_value_pop(stack, &result);
	*value = result;
	free(postfixExpression);
	Stack_Dispose(stack);
	free(stack);
	return true;
}

/* Konec c204.c */
