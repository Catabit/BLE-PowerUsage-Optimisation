# BLE-PowerUsage-Optimisation

Proiect inscris in **Sesiunea de comunicari stiintifice studentesti** Mai 2018.

Scopul proiectului este optimizarea autonomiei unui presupus device medical ce colecteaza periodic date despre pacient si le sincronizeaza cu telefonul acestuia la anumite intervale. In cazul de fata, datele colectate proveneau de la un accelerometru ADXL345.

Pentru această clasă de dispozitive, pe primul loc ca importantă constructivă se află autonomia: cu cât un device poate fi folosit mai mult cu o singură încărcare, cu atât se pot colecta mai multe date folositoare iar utilizatorul nu este deranjat zilnic de incarcarea acestuia.
Pentru optimizarea consumului de curent, am ales să construiesc un algoritm care, pe baza indicelui de semnal radio RSSI, decide dacă un set de date colectate anterior ar trebui transmise către client (telefon) pentru procesare sau dacă ar trebui amânata transmisia până când condițiile radio sunt mai bune (distanța intre cele două radio-uri este mai mică – când telefonul pacientului este aproape de device). Astfel, device-ul se poate afla in două stări de funcționare, odată conectat la client:
1.	Colectare locală de date, într-un buffer (in limita memoriei interne), cât timp clientul este la o distanță ce ar face transmiterea setului de date ineficient și costisitor. Se transmit doar pachetele periodice de conexiune.
2.	Transmiterea setului de date către client prin notificări succesive până la golirea buffer-ului de date stocate, în cazul în care condițiile de conectivitate radio sunt optime. Se transmit succesiv toate datele stocate.

Pentru a transmite date la o distanță mai mare, intuitiv, se va consuma mai mult curent. La puterea minimă de transmisie, cu raza minimă de acțiune, modulul radio consumă **5.5mA**. Pentru a transmite la putere maximă, pentru a maximiza distanța efectivă, acesta consumă **16mA**.Cât timp se recepționează date, modulul radio consumă un curent constant de aproximativ **13mA**.

Având in vedere numărul mare de seturi de date stocate în buffer, este clar că se dorește minimizarea curentului consumat în perioada de transmitere a datelor prin radio. Pentru un interval de conexiune de 7,5ms cu transmisie la putere maxima (figura 1), se consumă aproximativ 8.53mA. Pentru aceeași conexiune, la putere minimă se va consuma aproximativ 7.75mA. Într-o singură transmisie se pot transmite maxim 6 pachete, deci consumul va fi maxim 51.18mA si minim 46.5mA.
Pentru a transmite date colectate cu o rata de 10Hz, e nevoie ca o transmisie să se producă la fiecare 0.6 secunde. Consumul mediu ar fi, astfel: _{[(600-7.5)ms x sleepCurrent]+[7.5ms x connectionCurrent]} / 600_

În cazul de economie maximă a energiei, consumul mediu ar fi **0.582mA**, iar in cazul transmisiei la putere maximă ar fi **0.64mA**. Pentru o baterie tipica Litiu CR2032 de 220mAh, diferența ca autonomie a device-ului ar fi de **aproximativ 35 ore**. (378 ore vs 343 ore). Această optimizare aduce, deci, un beneficiu de minim **10% timp de funcționare în plus**, în aplicația studiată.
