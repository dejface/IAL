
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    //inicializacia poloziek na NULL
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;

}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    // rusim aktivnu a poslednu polozku zoznamu, budem uvolnovat od prvej
    L->Act = NULL;
    L->Last = NULL;

    //uvolnujem zoznam od prvej polozky
    while (L->First != NULL){
        // ukladam pointer na prvu polozku
        tDLElemPtr last = L->First;
        // posuvam sa na dalsiu polozku
        L->First = L->First->rptr;
        // uvolnujem prvu polozku zoznamu
        free(last);
    }
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokacia noveho elementu (polozky)
    tDLElemPtr newItem = malloc(sizeof(struct tDLElem));
    if (newItem == NULL){
        DLError();      //ak sa nepodari alokacia, volame DLError
    }

     newItem->data = val;       //priradenie dat
     newItem->lptr = NULL;    //kedze je to prva polozka,neukazuje na predoslu
     newItem->rptr = L->First;  //nasledujucou polozkou sa stava prva polozka

     if (L->First != NULL){     //ak zoznam nie je prazdny
         //prva polozka sa stava druhou a ukazuje na novu
         L->First->lptr = newItem;
         L->First = newItem;        //prvou polozkou sa stava nova polozka
     } else {     //v opacnom pripade sa nova polozka stava prvou aj poslednou
         L->First = newItem;
         L->Last = newItem;
     }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

    //alokacia noveho elementu (polozky)
    tDLElemPtr newItem = malloc(sizeof(struct tDLElem));
    if (newItem == NULL){
        DLError();      //ak sa nepodari alokacia, volame DLError
    }

    newItem->data = val;        //priradenie dat
    //kedze je to posledna polozka,neukazuje na dalsiu
    newItem->rptr = NULL;
    //predchadzajucou polozkou sa stava posledna polozka
    newItem->lptr = L->Last;


    if (L->First != NULL){      //ak zoznam nie je prazdny
        L->Last->rptr = newItem;    //posledna polozka sa stava predoslou
        L->Last = newItem;          //poslednou polozkou sa stava nova polozka
    } else {      //v opacnom pripade sa nova polozka stava prvou aj poslednou
        L->First = newItem;
        L->Last = newItem;
    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    //pointer na aktivnu polozku bude ukazovat na prvu polozku zoznamu
    L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	//pointer na aktivnu polozku bude ukazovat na poslednu polozku zoznamu
    L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if (L->First == NULL)
        DLError();          //volam DLError ak je zoznam prazdny
    else
        //inak pointer na val bude ukazovat na data prveho prvku
        *val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if (L->First == NULL)
        DLError();          //volam DLError ak je zoznam prazdny
    else
        //inak pointer na val bude ukazovat na data posledneho prvku
        *val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	
    if (L->First != NULL) {     //ak je zoznam prazdny, nedeje sa nic
        //ak je prva polozka aktivna,zrusime aktivitu
        if (L->Act == L->First) {
            L->Act = NULL;
        }
        tDLElemPtr copyFirst = L->First;    //ukladam pointer na prvu polozku

        if (L->First != L->Last) {      //ak je v zozname viac poloziek
            //zrusim pointer druhej polozke na predchadzajucu
            L->First->rptr->lptr = NULL;
            //druha polozka bude teraz prva
            L->First = L->First->rptr;
        } else {
            //ak je polozka v zozname samotna, tak ostane zoznam prazdny
            L->First = NULL;
            L->Last = NULL;
        }
        //uvolnujem povodnu polozku
        free(copyFirst);
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

    if (L->First != NULL){      //ak je zoznam prazdny, nedeje sa nic
        //ak je posledna polozka aktivna,zrusime aktivitu
        if (L->Act == L->Last){
            L->Act = NULL;
        }
        tDLElemPtr copyLast = L->Last;    //ukladam pointer na poslednu polozku

        if (L->First != L->Last){       //ak je v zozname viac poloziek
            //zrusim pointer predposlednej polozke na nasledujucu
            L->Last->lptr->rptr = NULL;
            //predposledna polozka je teraz posledna
            L->Last = L->Last->lptr;
        } else {
            //ak je polozka v zozname samotna, tak ostane zoznam prazdny
            L->First = NULL;
            L->Last = NULL;
        }
        //uvolnujem povodnu polozku
        free(copyLast);
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	//ak je aktivna posledna polozka alebo zoznam nie je aktivny, nic sa nedeje
    if (L->Last != L->Act && DLActive(L)){
        tDLElemPtr copyNext = L->Act->rptr; //ukladam pointer na dalsiu polozku

        if(L->Last != copyNext){    //dalsia polozka zoznamu nie je posledna
            /*aktivna polozka ukazuje na polozku za nasledujucou ako na dalsiu
              polozku*/
            L->Act->rptr = copyNext->rptr;
            //polozka za nasledujucou ukazuje na aktivnu polozku ako na predoslu
            copyNext->rptr->lptr = L->Act;
        } else {        //dalsia polozka zoznamu je posledna
            //aktivna polozka sa stane zaroven poslednou,rusim pointer na dalsiu
            L->Act->rptr = NULL;
            L->Last = L->Act;
        }
        //uvolnujem dalsiu polozku
        free(copyNext);
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    //ak je aktivna prva polozka alebo zoznam nie je aktivny, nic sa nedeje
    if (L->First != L->Act && DLActive(L)){
        //ukladam pointer na predoslu polozku
        tDLElemPtr copyPrev = L->Act->lptr;

        if(L->First != copyPrev){   //predosla polozka nie je v zozname posledna
            /*aktivna polozka ukazuje na polozku pred predoslou ako na predoslu
              polozku*/
            L->Act->lptr = copyPrev->lptr;
            copyPrev->lptr->rptr = L->Act;
        } else {    //predosla polozka zoznamu je prva v zozname
            //aktivna polozka sa stane prvou,rusim pointer na predoslu
            L->Act->lptr = NULL;
            L->First = L->Act;
        }
        //uvolnujem predoslu polozku
        free(copyPrev);
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	//ak je zoznam aktivny, pokracujeme, ak nie, nic sa nedeje
    if (DLActive(L)){
        //alokujem miesto pre novu polozku
        tDLElemPtr newItem = malloc(sizeof(struct tDLElem));
        if (newItem == NULL){
            DLError();      //ak sa alokacia nepodari volam DLError
        }

        newItem->data = val;    //priradim data
        newItem->rptr = L->Act->rptr;
        /*nova polozka bude za aktivnou takze pointer na predoslu ukazuje na
          aktivnu*/
        newItem->lptr = L->Act;
        L->Act->rptr = newItem;

        if (L->Act == L->Last){
            L->Last = newItem;
        } else { //naviazanie pravej polozky na vlozenu polozku
            newItem->rptr->lptr = newItem;
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    //ak je zoznam aktivny, pokracujeme, ak nie, nic sa nedeje
    if (DLActive(L)){
        //alokujem miesto pre novu polozku
        tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
        if (newElem == NULL){
            DLError();      //ak sa alokacia nepodari volam DLError
        }

        newElem->data = val;    //priradim data
        newElem->lptr = L->Act->lptr;
        /*nova polozka bude pred aktivnou, pointer na dalsiu polozku ukazuje na
          aktivnu*/
        newElem->rptr = L->Act;
        L->Act->lptr = newElem;

        if (L->Act == L->First){
            L->First = newElem;
        } else {    //naviazanie lavej polzoky na vlozenu polozku
            newElem->lptr->rptr = newElem;
        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

    if (!(DLActive(L)))     //ak zoznam nie je aktivny, volam DLError
        DLError();
    else
        *val = L->Act->data;    //pointer val smeruje na data aktivnej polozky
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	
    if (DLActive(L))    //ak je zoznam aktivny
        L->Act->data = val;     //menim data aktivnej polozky
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	
    if (DLActive(L))        //ak je zoznam aktivny
        L->Act = L->Act->rptr;      //posuvam aktivnu polozku na nasledujucu
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

    if (DLActive(L))        //ak je zoznam aktivny
        L->Act = L->Act->lptr;      //posuvam aktivnu polozku na predoslu
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	//test ci je zoznam aktivny
    return (L->Act != NULL) ? 1 : 0;
}

/* Konec c206.c*/
