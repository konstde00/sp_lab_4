# Лабораторна робота 4 з системного програмування

## Cинтаксичний аналізатор

Розробити LL(1)-синтаксичний аналізатор для заданої
граматики, який будує AST або визначає та локалізує синтаксичну помилку:

1) запрограмувати всі необхідні функції: Firsk(k), Follow(k), побудова таблиці управління, власне аналізатор по
таблиці

2) запрограмувати допоміжні функції: пошук епсилон-нетерміналів, читання і
розбір введеної граматики, тощо

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

## Приклад 2:

Вхідні дані: aa

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

No production found in parse table
Input string is rejected