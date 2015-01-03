duplist([],[]).
duplist([HX|TX],[H1 | [H2 | T2]]) :- H1 = H2, HX = H1, duplist(TX, T2).

put(K,V,[], [[K,V]]).
put(K,V,[[K,_]|T], D2) :-  append([[K,V]], T, D2).
put(K,V,[[K1, V1]|T], [[K1,V1]|Rest]) :-  put(K,V,T, Rest).

get(K,[[K,V]|T],V).
get(K,[[K1,V1]|T],V) :- get(K,T,V).


eval(intconst(I), E, V) :- V = intval(I).
eval(boolconst(B), E, V) :- V = boolval(B).
eval(var(X), [[X,V1]|T], V) :- V = V1.
eval(var(X), [[K,V1]|T], V) :- eval(var(X), T, V).
eval(geq(E1,E2), E, V) :- eval(E1, E, intval(R1)), eval(E2, E, intval(R2)), R1 > R2, V = boolval(true).
eval(geq(E1,E2), E, V) :- eval(E1, E, intval(R1)), eval(E2, E, intval(R2)), R1 =< R2, V = boolval(false).
eval(if(E1, E2, E3), E, V) :- eval(E1, E, boolval(V1)), V1, eval(E2, E, V).
eval(if(E1, E2, E3), E, V) :- eval(E1, E, boolval(V1)), V1 = false, eval(E3, E, V).
eval(function(X,E1), E, V) :- V = funval(X,E1,E).
eval(funcall(E1, E2), E, V) :- eval(E1,E,funval(X,E3, E)), eval(E2, E, R), put(X, R, E, D2), eval(E3, D2, V).

move(world([H|T],S2,S3,none), pickup(stack1), world(T,S2,S3,H)).
move(world(S1,[H|T],S3,none), pickup(stack2), world(S1,T,S3,H)). 
move(world(S1,S2,[H|T],none), pickup(stack3), world(S1,S2,T,H)). 

move(world(S1,S2,S3,H), putdown(stack1), world([H|S1],S2,S3,none)) :- H\= none.
move(world(S1,S2,S3,H), putdown(stack2), world(S1,[H|S2],S3,none)) :- H\= none. 
move(world(S1,S2,S3,H), putdown(stack3), world(S1,S2,[H|S3],none)) :- H\= none. 

blocksworld(Start, [], Start).
blocksworld(Start, [A|As], End) :- move(Start, A, Mid), blocksworld(Mid, As, End).