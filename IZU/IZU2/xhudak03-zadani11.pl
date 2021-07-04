% Řešení zadání 11
% Autor: David Hudák
% Jazyk: Prolog
% Testováno v: SWI-Prolog pro Windows 10, 64-bit

uloha11([], [], [], _).

uloha11([X|List], SEZNAMM, [X|SEZNAMV], MEZ):-
    X > MEZ,
    uloha11(List, SEZNAMM, SEZNAMV, MEZ).
uloha11([X|List], [X|SEZNAMM], SEZNAMV, MEZ):-
    X =< MEZ,
    uloha11(List, SEZNAMM, SEZNAMV, MEZ).

