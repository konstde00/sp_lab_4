# Лабораторна робота 4 з системного програмування

## Cинтаксичний аналізатор

Розробити LL(1)-синтаксичний аналізатор для заданої
граматики, який будує AST або визначає та локалізує синтаксичну помилку:

1) запрограмувати всі необхідні функції: Firsk(k), Follow(k), побудова таблиці управління, власне аналізатор по
таблиці

2) запрограмувати допоміжні функції: пошук епсилон-нетерміналів, читання і
розбір введеної граматики, тощо

3) 

## Граматика задана у файлі input.txt

S->AB
A->a
A->e
B->b
B->e

## Приклад 1:

Вхідний рядок: ab

Вихідні дані:

Grammar parsed from grammar file:
0.  S -> AB
1.  A -> a
2.  A -> e
3.  B -> b
4.  B -> e

The non terminals in the grammar are: A B S
The terminals in the grammar are: $ a b

Firsts list:
A : a e
B : b e
S : a b e

Follows list:
A : $ b
B : $
S : $

Parsing Table:
   $ a b
A  2 1 2
B  4 - 3
S  0 0 0

Input string is accepted

## Граматика задана у файлі input.txt

S->A
A->a
A->b
B->a
B->b
C->c
D->d

## Приклад 2:

Вхідний рядок: a

Grammar read from file:
0.  S -> A
1.  A -> a
2.  A -> b
3.  B -> a
4.  B -> b
5.  C -> c
6.  D -> d

The terminals in the grammar are: a b c d

The non terminals in the grammar are: A B C D S
The epsilon non-terminals in the grammar are:

First_1():
A : a b
B : a b
C : c
D : d
S : a b

Follow_1():
A : e
B :
C :
D :
S : e

Parsing Table:
$ a b c d
A  - 1 2 - -
B  - 3 4 - -
C  - - - 5 -
D  - - - - 6
S  - 0 0 - -

Starting operations:
0, 1,

Input string is accepted

AST:
   |_S
      |_A
         |_a