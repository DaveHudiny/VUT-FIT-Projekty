Body: 13,5 z 18

Komentáø uèitele k hodnocení:
Poznámky:
     Black box tests:
        RemoveNode má být DeleteNode.
        Napøíè testy jsem si všiml, že nekontrolujete vrácený uzel.
        Je tøeba použít assert, pro kontrolu ukazatele, jinak mùže docházet k segfault.
        Špatná implementace testù axiomù (sizeof(pole)).
     White Box Testing:
        Nedostateèné otestování solveEquation.
     Test Driven Development:
        ok

Detail k bodùm:
 Black Box Testing: 2.5
 White Box Testing: 5
 Test Driven Development: 6
Orientaèní výsledky z automatického ohodnocení:
 black box operations: 83%
 black box axioms: 0%
 white box coverage: 100%
 white box matrix sizes: 50%
 tdd empty queue: 100%
 tdd non empty queue + implementation: 100%
 tdd validity: 100%