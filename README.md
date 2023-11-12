# Druhý úkol do předmětu Algorimy (IAL)

## Zadání
**Cílem je vytvořit funkční implementaci:**
* **hashtable**: tabulka s rozptýlenými položkami (hashovací tabulka s explicitně zřetězenými synonymy) (technika chaining)
* **btree/rec**: binární vyhledávací strom a operace nad ním implementované rekurzivně
* **btree/iter**: binární vyhledávací strom a operace nad ním implementované iterativně
* **btree/exa**: jednoduché použití a vybalancování binárního vyhledávacího stromu

**Doplnit těla funkcí těchto souborů:**
* _hashtable/hashtable.c_
* _btree/rec/btree.c_
* _btree/iter/btree.c_
* btree/exa/exa.c

## Spuštění
Ověření funkionality probíhalo pomocí testů:
* hashtable: &nbsp;&nbsp; ```make test && ./test```
* btree/rec: &nbsp;&nbsp;&nbsp; ```make test && ./test```
* btree/iter: &nbsp;&nbsp; ```make test && ./test```
* btree/exa: &nbsp;&nbsp; ```make test && ./test_iter```&nbsp; nebo &nbsp; ```make test && ./test_rec```