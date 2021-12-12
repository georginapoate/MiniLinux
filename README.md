# Minilinux

## main:

In functia main am creat directorul home, care va fi echivalentul root-ului. I-am setat numele "home" si am mai creat un director "curr" careia ii atribuim "home" la inceput. Curr o sa isi schimbe valoarea pe parcursul codului.

Am folosit functia strtok pentru a imparti linie (line) in doua stringuri. Dupa ce se citeste de la tastatura linia, se verifica numele primului string pentru comanda, apoi se atribuie stringului-parametru al doilea string cu strtok.

## freeFiles

functia face free la toate fisierile din directorul primit ca parametru (daca acestea exista)

## freeDrts

functia apeleaza freeFiles ca sa stearga fisierele din cadrul directorului. se verifica daca directorul are in cadrul sau alte directoare - ca sa se apeleze recursiv si sa se stearga si acestea. se sterge numele si directorul in sine.

## contains 1 si contains 2:

Am creat functiile ajutatoare contains1 (pentru files) si contains2 (pentru directoare), ca sa verifice existenta fisierelor/directoarele cu numele date ca parametru. Functiile date sunt ajutoatoare pentru: touch, mkdir, rm, rmdir, cd si mv. Se returneaza fie NULL in cazul in care nu exista file/director cu numele dat, fie file-ul/directorul cu numele din parametru.


## touch:

Se verifica functia contains: daca fisierul exista: se printeaza "file already exists"; daca fisierul nu exista, se verifica daca este primul din cadrul directorului si se creaza acolo. Daca fisierul nu e head_children_files, se face un loop pana se ajunge la ultimul fisier si se creaza la capatul listei.

## mkdir:

Functia mkdir are acelasi principiu ca functia touch - sunt modificate doar numele specifice structurii si functia contains2.

## ls:

In functia ls o sa facem mai intai un loop pentru parcurgerea fiecarui director si afisarea acestuia, apoi un alt loop pentru parcurgerea si afisarea fisierelor.

## rm:

Functia rm trebuie sa stearga un fisier din cadrul directorului curent. Se apeleaza functia contains ca sa se verifice daca fisierul exista. Daca fisierul este inexistent, se afiseaza "Could not find file". Daca fisierul sters e primul din lista, se schimba capul listei cu urmatorul fisier, iar primul se sterge. In ultimul caz, se face un loop pana cand se gaseste fisierul, se fac legaturile intre fisierul anterior si nextul fisierului care trebuie sters, apoi se elibereaza fisierul care trebuie sters.

## rmdir:

Functia rmdir este la fel ca si rm, diferenta fiind stergerea elementelor din structura Dir: nume, primul fisier, primul director, structura in sine. Sunt aceleasi cazuri: directorul nu exista, directorul este primul din lista, directorul nu e primul din lista.

## cd:

Functia cd schimba directorul curent. Daca parametrul de la tastatura este "..", se verifica daca ne aflam in root (parintele e null) si se face return. Daca exista directorul parent, directorul curent devine directorul parinte. Daca de la tastatura se introduce ca parametru un alt director, verificam cu contains daca directorul in care vrem sa ne mutam exista. Daca nu exista, se printeaza "No directories found". In caz contrar, directorul curent devine cel gasit in contains.

## pwd:

In pwd folosim recursivitatea pentru reapela functia si a printa parintele directorului curent. Daca suntem in root, se returneaza numele /home al root-ului din main. Am creat buffer-ul path unde se adauga calea dupa recursivitatea functiei.

## stop:

stop apeleaza functia fileDrts ca sa stearga toate directoarele si fisierele din cadrul directorului home. Dupa ce face acest lucru, in main se face break si se inchide aplicatia.

## tree:

Tree are aceeasi functionalitate ca si ls, doar ca acesta printeaza patru spatii * nivelul directoarelor/fisierelor inaintea afisarii lor. Se parcurge recursiv.

## mv:

mv verifica daca exista fisierul/directorul cu numele vechi. Daca acesta nu exista, se afiseaza: "File/Director not found\n". In caz contrar, se verifica daca exista un fisier/director cu numele nou: daca exista, se printeaza "File/Director already exists\n". Ultima verificare este daca se modifica un fisier sau un director, prin functiile contains(nume vechi). Se sterg fisierele/directoarele noi si se adauga cele cu nume nou la capatul listei.