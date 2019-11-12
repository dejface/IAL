/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**						Vypracoval : David Oravec (xorave05)
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode(tKey key) {
	int retval = 1;
	int keylen = strlen(key);
	for (int i = 0; i < keylen; i++)
		retval += key[i];
	return (retval % HTSIZE);
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit(tHTable* ptrht) {
	for (unsigned int i = 0; i < HTSIZE; i++)	//inicializacia poloziek
		(*ptrht)[i] = NULL;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch(tHTable* ptrht, tKey key) {
	int index = hashCode(key);	//ziskanie indexu
	tHTItem* item = (*ptrht)[index];	//pristup k polozke v tabulke na indexe

	// prechadzam polozky na danom indexe
	while (item != NULL) {
		if (strcmp(item->key, key) == 0) {	// ak sa zhoduje  s key, vraciam
			return item;
		}
		// posuvam ukazatel na dalsi polozku na indexe
		item = item->ptrnext;
	}

	// prvok nebol najdeny, vraciam NULL
	return NULL;
}
/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert(tHTable* ptrht, tKey key, tData data) {
	// hladam polozku s danym klucom v tabulke
	tHTItem* item = htSearch(ptrht, key);

	// ak tam polozka uz je, zmenim jej data
	if (item) {
		item->data = data;
		return;
	}
	else {	// inak alokujem miesto na novu polozku
		item = (tHTItem*)malloc(sizeof(tHTItem));

		if (!item) {	//alokacia sa nepodarila
			return;
		}
		item->data = data;
		item->key = key;
		item->ptrnext = NULL;

		int index = hashCode(key);	//ziskanie indexu z tabulky
		if ((*ptrht)[index] != NULL) {  //ak na indexe nie je prazdna tabulka
			item->ptrnext = (*ptrht)[index];  //nastavim ptr
		}
		else {
			item->ptrnext = NULL;	//inak je tab prazdna a nikam ptr neukazuje
		}
		(*ptrht)[index] = item; // nakoniec ulozim prvok
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead(tHTable* ptrht, tKey key) {
	tHTItem* item = htSearch(ptrht, key);	//hladam polozku s danym klucom
	if (!item) return NULL;
	return &(item->data);	//vraciam ukazatel na data danej polozky
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete(tHTable* ptrht, tKey key) {
    int index = hashCode(key);	//ziskanie indexu z tabulky
    tHTItem* item = (*ptrht)[index];	//pristupujem k polozke na indexe
    tHTItem* previousItem = NULL, *nextItem = NULL; //pomocna premenna

    while (item) {		//pokial item nie je NULL
        nextItem = item->ptrnext;	//ulozim ukazatel na dalsiu polozku
        if (!strcmp(item->key, key)) {	//ak najdem zhodu v key
            if (previousItem == NULL) {	//a polozka nema pred sebou ziadnu inu
                //nasledujucu polozku ukladam priamo do tabulky na dany index
                (*ptrht)[index] = nextItem;
            } else {
                //predchadzajuca polozka ma ukazatel na nasledujucu
                previousItem->ptrnext = nextItem;
            }
            free(item);
            return;
        }
        previousItem = item;	//ukladam aktualnu polozku ako predchadzajucu
        item = nextItem;	//aktualna polozka bude dalsou polozkou
    }
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll(tHTable* ptrht) {
	tHTItem* item, *nextItem;
	for (unsigned i = 0; i < HTSIZE; i++) {	//prechadzam vsetky indexy tabulky
		item = (*ptrht)[i];		//urcim si index v zozname synonym

		while (item) {	//prechadzam zoznamom 
			nextItem = item->ptrnext;	//ulozim si ukazatel na dalsiu polozku

			free(item);	//uvolnenie polozky
			item = nextItem;
		}
		(*ptrht)[i] = NULL;	//nastavim ukazatel na indexe na NULL
	}
}