PhilosopherProblem

-Ten program spróbuję rozwiązać problem ucztujących filozofów na 3 sposoby:
1 - Za pomocą kelnera, który daje naczynia przy umowie, że oba naczynia są wolne.
2 - Za pomocą hierarchii zasobów: najpierw jest pobierany lewy widelec, później prawy. Zwrócenie naczyń zachodzi w odwrotnej kolejności ( prawy -> lewy). Dla ostatniego filozofa pobieranie i zwolnienie naczyń zachodzi w odwrotnej kolejności w stosunku do innych ( pobieranie: prawy -> lewy, zwolnienie: lewy -> prawy).
3 - podobny do sposobu nr.2, ale przy "długim" czasie zajętości spotrzebowanego widelca już zajmowany zezwala się i próba otrzymać naczynia powtarza.

KOMPILACJA:
/makefile

-URUCHOMIENIE:
./PhilosopherProblem [tup rozwiązania] [liczba filozofów]

parametry nie obowiązkowe

typy rozwiązań:
1 - kelner
2 - zwykła hierarchia
3 - hierarchia z "zezwalaniem"
domyślnie - kelner

liczba filozofów:
domyślnie - 5

-Przykład uruchomienia 8 filozofów z hierarchią z "zezwalaniem":
./PhilosopherProblem 3 8

-Przyciski wewnątrz programu:
ESC - wyjście z porządnym zamykaniem wątków
Shift+Q - wymusza zajęcie wątków i awaryjne wyjście z programu

!UWAGA: podczas działania interfejsu graficznego działają tylko przyciski ESC i Shift+Q, w inne momenty Ctrl+C wywoła zwykłe zamknięcie programu!