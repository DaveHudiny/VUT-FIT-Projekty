Dobrý den,

projekt byl tvořen a testován s pomocí Pythonu 3.8.5 na mém Ubuntu WSL,
funkčnost byla ověřena i standardním voláním python3 na Merlinovi.

Dalším detailem je, že adresová struktura se nevytváří a kontrola existence souborů se nekoná.
Tudíž pokud se stáhne více souborů (a v případě self-testu k tomu také dochází),
dojde k přepsání starého souboru. Pokud je volán příkaz *, je stažen i soubor index.
K tomuto řešení jsem přistoupil z prostého důvodu, že pokud bych se dotazoval na existenci souborů
a vytvářel jejich přejmenované verze, pak by při self-testu vznikala ve složce self bomba.

Děkuji a přeji příjemné opravování.