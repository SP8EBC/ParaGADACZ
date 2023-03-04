<?php
# URUCHOMIENIE GENEROWANIA W KONSOLI:
#
# php index.php

# TYLKO Z TEJ TABLICY GENEROWANE SĄ SAMPLE
$slownik = array(
	array('wszystkie stacje wszystkie stacje wszystkie stacje tu','intro'),
	array('skrzyczne skrzyczne radio', 'intro2'),
	array('sierra november nine november sierra kilo', 'intro3'),

	array('kropka', 'kropka'),
	array('minus', 'minus'),
    array('oraz', 'oraz'),
    array('komunikaty specjalne', 'komunikaty_specjalne'),
    array('brak komunikatów specjalnych', 'brak_komunikatow_specjalnych'),

	array('pierwsza', 'pierwsza'),
	array('druga', 'druga'),
	array('trzecia', 'trzecia'),
	array('czwarta', 'czwarta'),
	array('piąta', 'piata'),
	array('szósta', 'szosta'),
	array('siódma', 'siodma'),
	array('ósma', 'osma'),
	array('dziewiąta', 'dziewiata'),
	array('dziesiąta', 'dziesiata'),
	array('jedynasta', 'jedynasta'),
	array('dwunasta', 'dwunasta'),
    array('trzynasta', 'trynasta'),
    array('czternasta', 'czternasta'),
    array('pietnasta', 'pietnasta'),
    array('szesnasta', 'szesnasta'),
    array('siedemnasta', 'siedemnasta'),
    array('osiemnasta', 'osiemnasta'),
    array('dziewietnasta', 'dziewietnasta'),
    array('dwudziesta', 'dwudziesta'),

    array('godzina', 'godzina'),
    array('czasu uniwersalnego', 'czasu_uniwersalnego'),
    array('czasu lokalnego', 'czasu_lokalnego'),

    array('aktualne warunki pogodowe', 'aktualne_warunki'),
    array('prognoza pogoda', 'prognoza_pogody'),
    array('na następne', 'na_nastepne'),

    array('prędkość wiatru', 'predkosc_wiatru'),
    array('porywy', 'porywy'),
    array('wiatr', 'wiatr'),
    array('temperatura', 'temperatura'),
    array('ciśnienie', 'cisnienie'),
    array('ciśnienie w regionie', 'cisnienie_w_regionie'),
    array('wilgotność', 'wilgotnosc'),

    array('północny', 'polnocny'),
    array('północny północno wschodni', 'polnocny_polnocno_wschodni'),
    array('północno wschodni', 'polnocno_wschodni'),
    array('wschodni północno wschodni', 'wschodni_polnocno_wschodni'),
    array('wschodni', 'wschodni'),
    array('wschodni południowo wschodni', 'wschodni_poludniowo_wschodni'),
    array('południowo wschodni', 'poludniowo_wschodni'),
    array('południowy południowo wschodni', 'poludniowy_poludniowo_wschodni'),
    array('południowy', 'poludniowy'),
    array('południowy południowo zachodni', 'poludniowy_poludniowo_zachodni'),
    array('południowo zachodni', 'poludniowo_zachodni'),
    array('zachodni południowo zachodni', 'zachodni_poludniowo_zachodni'),
    array('zachodni', 'zachodni'),
    array('zachodni północno zachodni', 'zachodni_polnocno_zachodni'),
    array('północno zachodni', 'polnocno_zachodni'),
    array('północny północno zachodni', 'polnocny_polnocno_zachodni'),

    array('metr na sekundę', 'metr_na_sekunde'),
    array('metry na sekundę', 'metry_na_sekunde'),
    array('metrów na sekundę', 'metrow_na_sekunde'),

    array('węzeł', 'wezel'),
    array('węzły', 'wezly'),
    array('węzłów', 'wezlow'),

    array('stopień', 'stopien'),
    array('stopnie', 'stopnie'),
    array('stopni', 'stopni'),

    array('godzine', 'godzine'),
    array('godziny', 'godziny'),
    array('godzin', 'godzin'),

    array('hektopaskale', 'hektopaskale'),
    array('procent', 'procent'),
    array('celsjusza', 'celsjusza'),

    array('tysiąc', 'tysiac'),
    array('tysiące', 'tysiace'),
    array('tysięcy', 'tysiecy'),

    array('sto', 'sto'),
    array('dwieście', 'dwiescie'),
    array('trzysta', 'trzysta'),
    array('czterysta', 'czterysta'),
    array('pięćset', 'piecset'),
    array('sześćset', 'szescset'),
    array('siedemset', 'siedemset'),
    array('osiemset', 'osiemset'),
    array('dziewięćset', 'dziewiecset'),

    array('dziesięć', 'dziesiec'),
    array('dwadzieścia', 'dwadziescia'),
    array('trzydzieści', 'trzydziesci'),
    array('czterdzieści', 'czterdziesci'),
    array('pięćdziesiąt', 'piecdziesiat'),
    array('sześćdziesiąt', 'szescdziesiat'),
    array('siedemdziesiąt', 'siedemdziesiat'),
    array('osiemdziesiąt', 'osiemdziesiat'),
    array('dziewięćdziesiąt', 'dziewiecdziesiat'),

	array('jedenaście', 'jedenascie'),
	array('dwanaście','dwanaście'),
	array('trzynaście', 'trzynascie'),
	array('czternaście', 'czternascie'),
	array('piętnaście', 'pietnascie'),
	array('szesnaście', 'szesnascie'),
	array('siedemnaście', 'siedemnascie'),
	array('osiemnaście', 'osiemnascie'),
	array('dziewietnaście', 'dziewietnascie'),

    array('jeden', 'jeden'),
    array('dwa', 'dwa'),
    array('trzy', 'trzy'),
    array('cztery', 'cztery'),
    array('pięć', 'piec'),
    array('sześć', 'szesc'),
    array('siedem', 'siedem'),
    array('osiem', 'osiem'),
    array('dziewięć', 'dziewiec'),
    array('zero', 'zero'),

    array('dwie', 'dwie'),
    array('do usłyszenia', 'outro'),

    array('brak komunikatów specjalnych', 'brak_komunikatow_specjalnych'),

    array('zagrożnie lawinowe', 'zagrozenie_lawinowe'),
    array('brak pokrywy śnieżnej', 'brak_pokrywy_snieznej'),
    array('niskie, pierwszy stopień', 'stopien_pierwszy'),
    array('umiarkowane, drugi stopień', 'stopien_drugi'),
    array('znaczne, trzeci stopień', 'stopien_trzeci'),
    array('wysokie, czwarty stopień', 'stopien_czwarty'),
    array(' Szczególnie niebezpieczna wystawa', 'niebezpieczna_wystawa'),



    array('skrzyczne', '_skrzyczne'),
    array('startowisko', '_startowisko'),
    array('jaworzyna skrzyczneńska', '_jaworzyna'),
    array('szczyrk', '_szczyrk'),
    array('magurka wilkowicka', '_magurka'),
    array('jezioro żywieckie', '_jezioro_zywieckie'),
    array('jezioro międzybrodzkie', '_jezioro_miedzybrodzkie'),
    array('międzybrodzie żywieckie', '_jezioro_miedzybrodzkie'),
    array('góra żar', '_zar'),
    array('pilsko', '_pilsko'),
    array('markowe szczawiny', '_markowe_szczawiny'),
    array('babia góra', '_babia_gora'),


);







# TA TABLICA TO TYLKO PRZECHOWALNIA
#$slownik_wszystkie = array(
#
#    # rozne
#    array('tu eksperymentalna automatyczna stacja pogodowa'),
#    array('test', 'test')
#);
