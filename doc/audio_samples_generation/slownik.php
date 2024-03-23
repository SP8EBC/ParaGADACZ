<?php
# URUCHOMIENIE GENEROWANIA W KONSOLI:
#
# php index.php

# TYLKO Z TEJ TABLICY GENEROWANE SĄ SAMPLE
$slownik = array(
	array('wszystkie stacje wszystkie stacje wszystkie stacje tu','intro'),
	array('skrzyczne skrzyczne radio', 'intro2'),
    array('beskidy radio', 'intro3'),
	array('sierra. november. nine. november. sierra. kilo', 'intro_nsk'),
    array('sierra. papa. november. two. ziro. ziro.', 'intro_spn200'),

    array('komunikat dla paralotniarzy, na temat obecnie aktywnych stref', 'intro_pg_restrictions'),

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
    array('jedenasta', 'jedenasta'),
	array('dwunasta', 'dwunasta'),
    array('trzynasta', 'trynasta'),
    array('czternasta', 'czternasta'),
    array('pietnasta', 'pietnasta'),
    array('szesnasta', 'szesnasta'),
    array('siedemnasta', 'siedemnasta'),
    array('osiemnasta', 'osiemnasta'),
    array('dziewietnasta', 'dziewietnasta'),
    array('dwudziesta', 'dwudziesta'),

    array('poniedziałek', 'poniedzialek'),
    array('wtorek', 'wtorek'),
    array('środa', 'sroda'),
    array('czwartek', 'czwartek'),
    array('piątek', 'piatek'),
    array('sobota', 'sobota'),
    array('niedziela', 'niedziela'),

    array('styczeń', 'styczen'),
    array('luty', 'luty'),
    array('marzec', 'marzec'),
    array('kwiecień', 'kwiecien'),
    array('maj', 'maj'),
    array('czerwiec', 'czerwiec'),
    array('lipiec', 'lipiec'),
    array('sierpień', 'sierpien'),
    array('wrzesień', 'wrzesien'),
    array('październik', 'pazdziernik'),
    array('listopad', 'listopad'),
    array('grudzień', 'grudzien'),

    array('godzina', 'godzina'),
    array('czasu uniwersalnego', 'czasu_uniwersalnego'),
    array('czasu lokalnego', 'czasu_lokalnego'),

    array('aktualne warunki pogodowe', 'aktualne_warunki'),
    array('prognoza pogody', 'prognoza_pogody'),
    array('na następne', 'na_nastepne'),

    array('brak zmian', 'brak_zmian'),
    array('przez ostatnie', 'przez'),
    array('spadek o', 'spadek'),
    array('wzrost o', 'wzrpst'),

    array('prędkość wiatru', 'predkosc_wiatru'),
    array('porywy', 'porywy'),
    array('wiatr', 'wiatr'),
    array('temperatura', 'temperatura'),
    array('ciśnienie', 'cisnienie'),
    array('ciśnienie w regionie', 'cisnienie_w_regionie'),
    array('wilgotność', 'wilgotnosc'),
    array('opady', 'opady'),
    array('brak opadów', 'brak_opadow'),
    array('przelotne', 'przelotne'),
    array('deszczu', 'deszczu'),
    array('śniegu', 'sniegu'),
    array('od', 'od'),   
    array('do', 'do'),
    array('powyżej', 'powyzej'),
    array('intensywne', 'intensywne'),
    array('lokalne', 'lokalne'),
    array('możliwe', 'mozliwe'),
    array('burze', 'burze'),

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

    array('metr', 'metr'),
    array('metry', 'metry'),
    array('metrów', 'metrow'),

    array('kilometr', 'kilometr'),
    array('kilometry', 'kilometry'),
    array('kilometrów', 'kilometrow'),

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
	array('dwanaście','dwanascie'),
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
    array('jezioro międzybrodzkie góra żar', '_jezioro_miedzybrodzkie'),
    array('międzybrodzie żywieckie', '_jezioro_miedzybrodzkie'),
    array('góra żar', '_zar'),
    array('pilsko', '_pilsko'),
    array('markowe szczawiny', '_markowe_szczawiny'),
    array('babia góra', '_babia_gora'),

    #ARISPACE
    array('alfa', 'ph_alpha'),
    array('brawo', 'ph_bravo'),
    array('czarli', 'ph_charlie'),
    array('delta', 'ph_delta'),
    array('eko', 'ph_echo'),
    array('fokstrot', 'ph_foxtrot'),
    array('golf', 'ph_golf'),
    array('hotel', 'ph_hotel'),
    array('india', 'ph_india'),
    array('dżuliet', 'ph_juliet'),
    array('kilo', 'ph_kilo'),
    array('lima', 'ph_lima'),
    array('majk', 'ph_mike'),
    array('nowember', 'ph_november'),
    array('oskar', 'ph_oscar'),
    array('papa', 'ph_papa'),
    array('kłebek', 'ph_quebec'),
    array('romijo', 'ph_romeo'),
    array('sierra', 'ph_sierra'),
    array('tango', 'ph_tango'),
    array('uniform', 'ph_uniform'),
    array('wiktor', 'ph_vitor'),
    array('łiski', 'ph_whiskey'),
    array('iksrej', 'ph_xray'),
    array('janki', 'ph_yankee'),
    array('zulu', 'ph_zulu'),

    array('łan', 'one'),
    array('tu', 'two'),
    array('tri', 'three'),
    array('for', 'four'),
    array('fajf', 'five'),
    array('siks', 'six'),
    array('sewen', 'seven'),
    array('ejt', 'eight'),
    array('najner', 'nine'),
    array('ziro', 'ziro'),


    array('strefa rezerwowana', 'strefa_rezerwowana'),
    array('strefa zamknięta', 'strefa_zamknieta'),
    array('strefa ruchu nadlotniskowego', 'strefa_ruchu_nadlotniskowego'),
    array('strefa niebezpieczna', 'strefa_niebezpieczna'),
    array('strefa ograniczeń', 'strefa_ograniczen'),
    array('strefa zakazana', 'strefa_zakazana'),

    array('strefa', 'strefa'),
    array('sektor', 'sektor'),

    array('mila morska', 'mila_morska'),
    array('mile morskie', 'mile_morskie'),
    array('mil morskich', 'mil_morskich'),

    array('obszar', 'obszar'), 
    array('promień', 'promien'), 
    array('ograniczenia lotów', 'ograniczenia_lotow'), 
    array('w promieniu', 'w_promieniu'), 
    array('wokół', 'wokol'), 
    array('od lokalizacji', 'od_lokalizacji'), 
    array('wysokość', 'wysokosc'), 
    array('poziom gruntu', 'poziomu_gruntu'), 
    array('aktywna', 'aktywna'), 

    array('a te zet nowy targ', 'atz_epnt'), 
    array('a te zet bielsko biała', 'atz_epba'), 
    array('a te zet żar', 'atz_epzr'), 
    array('a te zet rybnik', 'atz_eprg'), 

    array('wojskowe skoki spadochronowe', 'wojskowe_skoki'), 
    array('cywilne skoki spadochronowe', 'cywilne_skoki'), 
    array('akrobacja szybkowcowa', 'akrobacja_szybowcowa'), 
    array('zawody szybowcowe', 'zawody_szybowcowe'), 
    array('zawody paralotniowe', 'zawody_para'), 

    array('stacja podaje wyłącznie planowane użycie przestrzeni powietrznej, które może się różnić od faktycznego. ', 'outro_disclarimer'), 
    array('najnowsze informację dostępne w Kraków Informacja', 'outro_disclarimer_2'), 
    array('aktualne warunki i prognoza pogody, proszę słuchać Skrzyczne Radio, u ka ef kanał 7, częstotliwość, sto sześćdziesiąt kropka dziewięć pięć zero megaherców, ef em', 'outro_listen_to_spn')



);







# TA TABLICA TO TYLKO PRZECHOWALNIA
#$slownik_wszystkie = array(
#
#    # rozne
#    array('tu eksperymentalna automatyczna stacja pogodowa'),
#    array('test', 'test')
#);
