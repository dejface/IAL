/* ******************************* c202.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c202 - Zásobník znaků v poli                                        */
/*  Referenční implementace: Petr Přikryl, 1994                               */
/*  Vytvořil: Václav Topinka, září 2005                                       */
/*  Úpravy: Kamil Jeřábek, září 2019                                          */
/* ************************************************************************** */
/*
** Implementujte datový typ zásobník znaků ve statickém poli. ADT zásobník je
** reprezentován záznamem typu tStack. Statické pole 'arr' je indexováno
** do maximální hodnoty STACK_SIZE. Položka záznamu 'top' ukazuje na prvek
** na vrcholu zásobníku. Prázdnému zásobníku odpovídá hodnota top == -1,
** zásobníku s jedním prvkem hodnota 0, atd. Přesnou definici typů neleznete
** v hlavičkovém souboru c202.h.
**
** Implementujte následující funkce:
**
**    stackInit .... inicializace zásobníku
**    stackEmpty ... test na prázdnost zásobníku
**    stackFull .... test na zaplněnost zásobníku
**    stackTop ..... přečte hodnotu z vrcholu zásobníku
**    stackPop ..... odstraní prvek z vrcholu zásobníku
**    stackPush .... vloží prvku do zásobníku
**
** Své řešení účelně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c202.h"

int solved;
int STACK_SIZE = MAX_STACK;

void stackError ( int error_code ){
/*   ----------
** Vytiskne upozornění, že došlo k chybě při práci se zásobníkem.
**
** TUTO FUNKCI, PROSÍME, NEUPRAVUJTE!
*/
	static const char* SERR_STRINGS[MAX_SERR+1] = {"Unknown error","Stack error: INIT","Stack error: PUSH","Stack error: TOP"};
	if ( error_code <= 0 || error_code > MAX_SERR )
		error_code = 0;
	printf ( "%s\n", SERR_STRINGS[error_code] );
	err_flag = 1;
}

void stackInit ( tStack* s ) {
/*   ---------
** Provede inicializaci zásobníku - nastaví vrchol zásobníku.
** Hodnoty ve statickém poli neupravujte - po inicializaci zásobníku
** jsou nedefinované.
**
** V případě, že funkce dostane jako parametr s == NULL,
** volejte funkci stackError(SERR_INIT). U ostatních funkcí pro zjednodušení
** předpokládejte, že tato situace nenastane.
*/
	/* ak dostala funkcia param NULL -> error*/
	if (s == NULL) stackError(SERR_INIT);
	else s->top = -1; //nastavime vrchol zasobniku
}

int stackEmpty ( const tStack* s ) {
/*  ----------
** Vrací nenulovou hodnotu, pokud je zásobník prázdný, jinak vrací hodnotu 0.
** Funkci implementujte jako jediný příkaz. Vyvarujte se zejména konstrukce
** typu "if ( true ) b=true else b=false".
*/
	//ak je stack prazdny vraciame 1, inak 0
	return (s->top != -1) ? 0 : 1 ;
}

int stackFull ( const tStack* s ) {
/*  ---------
** Vrací nenulovou hodnotu, je-li zásobník plný, jinak vrací hodnotu 0.
** Dejte si zde pozor na častou programátorskou chybu "o jedničku"
** a dobře se zamyslete, kdy je zásobník plný, jestliže může obsahovat
** nejvýše STACK_SIZE prkvů a první prvek je vložen na pozici 0.
**
** Funkci implementujte jako jediný příkaz.
*/
	//kontrola ci je zasobnik plny, ak ano vraciame 1 inak 0
	return ((STACK_SIZE -  1) == s->top) ? 1 : 0;
}

void stackTop ( const tStack* s, char* c ) {
/*   --------
** Vrací znak z vrcholu zásobníku prostřednictvím parametru c.
** Tato operace ale prvek z vrcholu zásobníku neodstraňuje.
** Volání operace Top při prázdném zásobníku je nekorektní
** a ošetřete ho voláním funkce stackError(SERR_TOP).
**
** Pro ověření, zda je zásobník prázdný, použijte dříve definovanou
** funkci stackEmpty.
*/
	// ak je zasobnik prazdny vraciame error, else cez param c vratime znak z vrcholu stacku
	if(stackEmpty(s) != 0) stackError(SERR_TOP);
	else *c = s->arr[s->top];
}


void stackPop ( tStack* s ) {
/*   --------
** Odstraní prvek z vrcholu zásobníku. Pro ověření, zda je zásobník prázdný,
** použijte dříve definovanou funkci stackEmpty.
**
** Vyvolání operace Pop při prázdném zásobníku je sice podezřelé a může
** signalizovat chybu v algoritmu, ve kterém je zásobník použit, ale funkci
** pro ošetření chyby zde nevolejte (můžeme zásobník ponechat prázdný).
** Spíše než volání chyby by se zde hodilo vypsat varování, což však pro
** jednoduchost neděláme.
**
*/

	if (stackEmpty(s) == 0){
		s->arr[s->top] = '\0';	//ak nie je stack prazdny odstranime znak na vrchu
		s->top -= 1; // posun o odstraneny prvok na stacku vzad
	}
}


void stackPush ( tStack* s, char c ) {
/*   ---------
** Vloží znak na vrchol zásobníku. Pokus o vložení prvku do plného zásobníku
** je nekorektní a ošetřete ho voláním procedury stackError(SERR_PUSH).
**
** Pro ověření, zda je zásobník plný, použijte dříve definovanou
** funkci stackFull.
*/

	if (stackFull(s) == 1){
		stackError(SERR_PUSH);	// pokial je stack plny hadzeme err
	} else {
		s->top += 1;	// posun stacku dopredu (spravi miesto)
		s->arr[s->top] = c;	//vlozenie znaku na vrch stacku
	}
}

/* Konec c202.c */
