DROP TABLE Barva_očí CASCADE CONSTRAINTS;
DROP TABLE Rasa_koček CASCADE CONSTRAINTS;
DROP TABLE Kočka CASCADE CONSTRAINTS;
DROP TABLE Hostitel CASCADE CONSTRAINTS;
DROP TABLE Věc CASCADE CONSTRAINTS;
DROP TABLE Život CASCADE CONSTRAINTS;
DROP TABLE Teritorium CASCADE CONSTRAINTS;
DROP TABLE Vnitřní CASCADE CONSTRAINTS;
DROP TABLE Venkovní CASCADE CONSTRAINTS;
DROP TABLE Barva_Rasa CASCADE CONSTRAINTS;
DROP TABLE Rasa_Hostitel CASCADE CONSTRAINTS;
DROP TABLE Kočka_Hostitel CASCADE CONSTRAINTS;
DROP SEQUENCE Hostitel_trigger;
DROP MATERIALIZED VIEW MATERIAL;

CREATE TABLE Barva_očí
    (
        barva CHAR(50),
        CONSTRAINT PK_Barva_očí PRIMARY KEY (barva)
    );
    
CREATE TABLE Teritorium
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        název CHAR(50),
        kapacita INT
    );
    
CREATE TABLE Vnitřní
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        počet_pokojů INT,
        vnitřní_teritorium INT CONSTRAINT vnitřní_teritorium_NN NOT NULL,
        CONSTRAINT Vnitřní_Teritorium FOREIGN KEY (vnitřní_teritorium) REFERENCES Teritorium ON DELETE CASCADE
    );
    
CREATE TABLE Venkovní
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        rozloha INT,
        venkovní_teritorium INT CONSTRAINT venkovní_teritorium_NN NOT NULL,
        CONSTRAINT Venkovní_Teritorium FOREIGN KEY (venkovní_teritorium) REFERENCES Teritorium ON DELETE CASCADE
    );

CREATE TABLE Rasa_koček
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        název CHAR(50),
        původ CHAR(50),
        maximální_délka_tesáku int check (MAXIMÁLNÍ_DÉLKA_TESÁKU >= 0)
    );

CREATE OR REPLACE TRIGGER Rasa_trigger
	BEFORE INSERT ON Rasa_koček
	FOR EACH ROW 
BEGIN
    IF :NEW.maximální_délka_tesáku > 300 THEN
    :NEW.maximální_délka_tesáku := 300;
    END IF;
END;
/

CREATE TABLE Hostitel
    (
        id INT DEFAULT NULL PRIMARY KEY,
        jméno CHAR(50),
        věk INT,
        pohlaví CHAR(1),
        místo_bydlení CHAR(50),
        dostupný number(1)
    );

/**Trigger pro primární klíče hostitele */
CREATE SEQUENCE Hostitel_trigger;

CREATE OR REPLACE TRIGGER Hostitel_trigger
	BEFORE INSERT ON hostitel
	FOR EACH ROW
    WHEN (new.id IS NULL)
BEGIN
    SELECT Hostitel_trigger.NEXTVAL
	into :NEW.id
    FROM DUAL;
END;
/

CREATE TABLE Kočka
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        hlavní_jméno CHAR(50),
        barva_srsti CHAR(50),
        hmotnost_vzorku_kuze INT,
        kočka_rasa INT CONSTRAINT kočka_rasa_NN NOT NULL,
	kočka_teritorium INT CONSTRAINT kočka_teritorium_NN NOT NULL,
        CONSTRAINT FK_Rasa FOREIGN KEY (kočka_rasa) REFERENCES Rasa_koček,
	CONSTRAINT FK_Teritorium FOREIGN KEY (kočka_teritorium) REFERENCES Teritorium
    );


CREATE TABLE Věc
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        typ CHAR(50),
        kvantita INT,
	id_hostitele INT CONSTRAINT id_hostitele_NN NOT NULL,
	kočka_věc INT CONSTRAINT kočka_věc_NN NOT NULL,
	kočka_věc_id INT,
	kočka_věc_počátek_vlastnictví DATE,
	kočka_věc_konec_vlastnictví DATE,
	věc_teritorium INT CONSTRAINT věc_teritorium_NN NOT NULL,
	CONSTRAINT FK_id_hostitele FOREIGN KEY (id_hostitele) REFERENCES Hostitel,
	CONSTRAINT FK_Kočka_věc FOREIGN KEY (kočka_věc) REFERENCES Kočka,
	CONSTRAINT FK_teritorium_věc FOREIGN KEY (věc_teritorium) REFERENCES Teritorium
    );

CREATE TABLE Život
    (
    id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
    délka FLOAT(24),
	kočka_život INT CONSTRAINT kočka_život_NN NOT NULL,
	narodil_se INT CONSTRAINT narodil_se_NN NOT NULL,
	končí INT CONSTRAINT končí_NN NOT NULL,
	končí_id INT,
	způsob_smrti CHAR(255),
	CONSTRAINT FK_kočka_život FOREIGN KEY (kočka_život) REFERENCES Kočka,
	CONSTRAINT FK_Teritorium1 FOREIGN KEY (narodil_se) REFERENCES Teritorium,
	CONSTRAINT FK_Teritorium2 FOREIGN KEY (končí) REFERENCES Teritorium
    );

CREATE TABLE Barva_Rasa
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        id_rasa INT CONSTRAINT id_rasa_NN NOT NULL,
        barva CHAR(50) CONSTRAINT barva_NN NOT NULL,
        CONSTRAINT FK_Barva_rasa FOREIGN KEY (barva) REFERENCES Barva_očí,
        CONSTRAINT FK_Rasa_rasa_kočka FOREIGN KEY (id) REFERENCES Rasa_koček
    );

CREATE TABLE Rasa_Hostitel
    (
        id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
        id_rasy INT CONSTRAINT id_rasy_NN NOT NULL,
        id_hostitel INT CONSTRAINT id_hostitel_NN NOT NULL,
        CONSTRAINT FK_id_rasy FOREIGN KEY (id_rasy) REFERENCES Rasa_koček,
        CONSTRAINT FK_id_hostitel FOREIGN KEY (id_hostitel) REFERENCES Hostitel
    );

CREATE TABLE Kočka_Hostitel
    (
    id INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	id_kočka INT CONSTRAINT id_kočka_NN NOT NULL,
	id_hostitel INT CONSTRAINT hostitel_NN NOT NULL,
	hostitelův_název CHAR(50),
	hodnocení_hostitele FLOAT(24),
	CONSTRAINT FK_id_kočka_kh FOREIGN KEY (id_kočka) REFERENCES Kočka,
	CONSTRAINT FK_id_hostitel_kh FOREIGN KEY (id_hostitel) REFERENCES Hostitel
    );
    
INSERT INTO Barva_očí VALUES ('Modra');
INSERT INTO Barva_očí VALUES ('Cervena');
INSERT INTO Barva_očí VALUES ('Zelena');

INSERT INTO Teritorium (název, kapacita) VALUES ('Pokojik', 1);
INSERT INTO Teritorium (název, kapacita) VALUES ('Obyvak', 10);
INSERT INTO Teritorium (název, kapacita) VALUES ('Pokojicek', 100);
INSERT INTO Teritorium (název, kapacita) VALUES ('Zahrada', 50);
INSERT INTO Teritorium (název, kapacita) VALUES ('Balkón', 2);
INSERT INTO Teritorium (název, kapacita) VALUES ('Pole', 1000);

INSERT INTO Vnitřní (počet_pokojů, vnitřní_teritorium) VALUES (1, 1);
INSERT INTO Vnitřní (počet_pokojů, vnitřní_teritorium) VALUES (1, 2);
INSERT INTO Vnitřní (počet_pokojů, vnitřní_teritorium) VALUES (1, 3);

INSERT INTO Venkovní (rozloha, venkovní_teritorium) VALUES (100, 4);
INSERT INTO Venkovní (rozloha, venkovní_teritorium) VALUES (2, 5);
INSERT INTO Venkovní (rozloha, venkovní_teritorium) VALUES (50000, 6);

INSERT INTO Rasa_koček (název, původ, maximální_délka_tesáku) VALUES ('Britska', 'Britanie', 10);
INSERT INTO Rasa_koček (název, původ, maximální_délka_tesáku) VALUES ('Siamska', 'Sphinx', 5);
INSERT INTO Rasa_koček (název, původ, maximální_délka_tesáku) VALUES ('Ostrovid', 'Czechia', 100);
INSERT INTO Rasa_koček (název, původ, maximální_délka_tesáku) VALUES ('Prosta', 'Czechia', 15);
INSERT INTO Rasa_koček (název, původ, maximální_délka_tesáku) VALUES ('Obrzub', 'Africa', 400);

SELECT * FROM RASA_KOČEK; 
/** Demonstrace triggeru omezující překročení hodnoty 300 **/
SELECT * FROM RASA_KOČEK;

INSERT INTO Hostitel (jméno, věk, pohlaví, místo_bydlení, dostupný) VALUES ('David', 10, 'B', 'Ořechov', 0);
INSERT INTO Hostitel (jméno, věk, pohlaví, místo_bydlení, dostupný) VALUES ('Davidek',21, 'M', 'Ořechov', 1);
INSERT INTO Hostitel (jméno, věk, pohlaví, místo_bydlení, dostupný) VALUES ('Štefan', 12, 'N', 'Nový Lískovec', 1);
INSERT INTO Hostitel (jméno, věk, pohlaví, místo_bydlení, dostupný) VALUES ('Honza', 9, 'F', 'Třebíči', 1);

/** Demonstrace triggeru pro generování id hostitele **/

INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Rudolf', 'Zelena', 10, 3, 3);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Kryšpínek', 'Mourovata', 10, 1, 1);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Quentin', 'Hnědá', 10, 1, 1);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Honzakocka', 'Bezsrstna', 10, 2, 2);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Obrozub', 'Zubata', 10, 5, 2);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Obrozub_ze_zahrady', 'Zubata', 10, 5, 4);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Obrozub_z_balkonu', 'Zubata', 10, 5, 5);
INSERT INTO Kočka (hlavní_jméno, barva_srsti, hmotnost_vzorku_kuze, kočka_rasa, kočka_teritorium) VALUES ('Proste_z_pole', 'Zubata', 10, 4, 6);

INSERT INTO Věc (typ, kvantita, id_hostitele, kočka_věc, kočka_věc_počátek_vlastnictví, kočka_věc_konec_vlastnictví, věc_teritorium) VALUES ('Mič', 1, 1, 1, TO_DATE('2020-08-14','yyyy-mm-dd'),
TO_DATE('2021-01-14','yyyy-mm-dd'), 1);
INSERT INTO Věc (typ, kvantita, id_hostitele, kočka_věc, kočka_věc_počátek_vlastnictví, kočka_věc_konec_vlastnictví, věc_teritorium) VALUES ('Sušenka', 200, 3, 2, TO_DATE('2020-08-11','yyyy-mm-dd'),
TO_DATE('2021-01-14','yyyy-mm-dd'), 1);
INSERT INTO Věc (typ, kvantita, id_hostitele, kočka_věc, kočka_věc_počátek_vlastnictví, kočka_věc_konec_vlastnictví, věc_teritorium) VALUES ('Brouk', 4, 4, 3, TO_DATE('2020-08-13','yyyy-mm-dd'),
TO_DATE('2021-01-14','yyyy-mm-dd'), 1);

INSERT INTO Život(délka, kočka_život, narodil_se, končí, končí_id, způsob_smrti) VALUES (20, 1, 1, 2, 1, 'Prejet vozem znacky Fiat');
INSERT INTO Život(délka, kočka_život, narodil_se, končí, končí_id, způsob_smrti) VALUES (100, 2, 1, 2, 2, 'Spadl z okna a neotocil se');
INSERT INTO Život(délka, kočka_život, narodil_se, končí, končí_id, způsob_smrti) VALUES (230, 3, 1, 2, 3, 'Překrmení granulemi');

INSERT INTO Barva_Rasa(barva, id_rasa) VALUES ('Modra', 1);
INSERT INTO Barva_Rasa(barva, id_rasa) VALUES ('Cervena', 2);
INSERT INTO Barva_Rasa(barva, id_rasa) VALUES ('Modra', 3);

INSERT INTO Rasa_Hostitel(id_hostitel, ID_RASY) VALUES (1, 3);
INSERT INTO Rasa_Hostitel(id_hostitel, ID_RASY) VALUES (2, 2);
INSERT INTO Rasa_Hostitel(id_hostitel, ID_RASY) VALUES (3, 1);
INSERT INTO Rasa_Hostitel(id_hostitel, ID_RASY) VALUES (1, 1);
INSERT INTO Rasa_Hostitel(id_hostitel, ID_RASY) VALUES (2, 1);
INSERT INTO Rasa_Hostitel(id_hostitel, ID_RASY) VALUES (4, 4);


INSERT INTO Kočka_Hostitel(id_kočka, ID_HOSTITEL, hostitelův_název, hodnocení_hostitele) VALUES (1,1, 'Nora', 1);
INSERT INTO Kočka_Hostitel(id_kočka, ID_HOSTITEL, hostitelův_název, hodnocení_hostitele) VALUES (2,2, 'Rudolf', 2);
INSERT INTO Kočka_Hostitel(id_kočka, ID_HOSTITEL, hostitelův_název, hodnocení_hostitele) VALUES (3,3, 'Kocka', 3);

/**  Tiskne spojení tří tabulek obsahující jméno a základní údaje hostitele, hostitelovo jméno kočky a reálné jméno kočky v rámci n:n relace **/
SELECT DISTINCT H.jméno, H.věk, H.pohlaví, HK.hostitelův_název, K.hlavní_jméno
 FROM Hostitel H JOIN KOČKA_HOSTITEL HK on H.id = HK.id_hostitel
 JOIN Kočka K on K.id = HK.id_kočka;

/** Tiskne základní údaje o kočce, která vlastní míč **/
SELECT DISTINCT K.hlavní_jméno, K.barva_srsti, K.hmotnost_vzorku_kuze, V.typ
 FROM Kočka K JOIN Věc V USING (id)
 WHERE typ = 'Mič';

/** Tiskne názvy teritorií a smrtí, které se v ném odehrály. **/
SELECT DISTINCT T.Název, Z.způsob_smrti 
 FROM Teritorium T JOIN Život Z USING(id);

/** Tiskne součet délek všech životů koček v daném teritoriu **/
SELECT T.název, SUM(délka) soucet_zivotu
 FROM TERITORIUM T, Život Z
 WHERE T.id = Z.kočka_život
 GROUP BY T.název;

/** Tiskne maximální součet délek tésáků v kažém teritoriu (aby uživatel mohl zvážit nutnost odolnosti nábytku.) **/
SELECT T.název, sum(MAXIMÁLNÍ_DÉLKA_TESÁKU) max_delka_tesaku
 FROM KOČKA K, RASA_KOČEK R, TERITORIUM T
 WHERE K.kočka_rasa = R.id and K.kočka_teritorium = T.id
 GROUP BY T.název;


/** Tiskne všechny údaje o všech dostupných rasách koček **/
SELECT *
FROM RASA_KOČEK;

/** Tiskne všechny barvy očí **/
SELECT *
FROM BARVA_OČÍ;

/** Tiskne všechny údaje koček + jejich teritoria a rasy **/
SELECT *
FROM KOČKA K, TERITORIUM T, RASA_KOČEK R
WHERE K.kočka_rasa = R.id and K.kočka_teritorium = T.id;

/** Tiskne jenom takové kočky, které jsou prosté (nemají speciální rasu, venkovský mix) **/
SELECT H.*, R.název
 FROM HOSTITEL H, RASA_HOSTITEL RH, RASA_KOČEK R
 WHERE RH.id_hostitel = H.id and RH.id_rasy = R.id and NOT EXISTS(
     SELECT *
     FROM HOSTITEL H, RASA_KOČEK R
     WHERE RH.id_rasy = R.id and R.název <> 'Prosta'
 );

/** Tiskne doplňek k předchozímu příkazu - tiskne všechny kočky, které nejsou prosté **/
 SELECT H.*, R.název
 FROM HOSTITEL H, RASA_HOSTITEL RH, RASA_KOČEK R
 WHERE RH.id_hostitel = H.id and RH.id_rasy = R.id and NOT EXISTS(
     SELECT *
     FROM HOSTITEL H, RASA_KOČEK R
     WHERE RH.id_rasy = R.id and R.název = 'Prosta'
 );

/** Tiskne všechny hostitele, kteří mají kočičku rasy britská a ostrovid (chceme se jich poptat na tipy a triky k chovu daného plemena) **/
SELECT H.*, R.název
 FROM HOSTITEL H, RASA_HOSTITEL RH, RASA_KOČEK R
 WHERE RH.id_hostitel = H.id and RH.id_rasy = R.id and R.název in ('Britska', 'Ostrovid');


/** Tvorba materializovaného pohledu na sjednocení tabulek rasy koček a koček (ukázána jména a jejich rasa) **/
CREATE MATERIALIZED VIEW MATERIAL
NOLOGGING
CACHE
BUILD IMMEDIATE
REFRESH ON COMMIT
ENABLE QUERY REWRITE
AS
  SELECT K.hlavní_jméno, R.název
  FROM KOČKA K NATURAL JOIN RASA_KOČEK R;

EXPLAIN PLAN FOR
SELECT T.název, sum(MAXIMÁLNÍ_DÉLKA_TESÁKU) max_delka_tesaku
 FROM KOČKA K, RASA_KOČEK R, TERITORIUM T
 WHERE K.kočka_rasa = R.id and K.kočka_teritorium = T.id
 GROUP BY T.název;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

/** Snaha o optimalizaci, kočkoindex se ukázal jako neoptimální **/
-- DROP INDEX KOČKOINDEX;
DROP INDEX RASOINDEX;
DROP INDEX TERITORIUMINXED;
--CREATE INDEX KOČKOINDEX ON KOČKA (ID, kočka_rasa, kočka_teritorium);
CREATE INDEX RASOINDEX ON RASA_KOČEK (ID, MAXIMÁLNÍ_DÉLKA_TESÁKU);
CREATE INDEX TERITORIUMINXED ON TERITORIUM(ID, NÁZEV);

EXPLAIN PLAN FOR
SELECT T.název, sum(MAXIMÁLNÍ_DÉLKA_TESÁKU) max_delka_tesaku
 FROM KOČKA K, RASA_KOČEK R, TERITORIUM T
 WHERE K.kočka_rasa = R.id and K.kočka_teritorium = T.id
 GROUP BY T.název;

SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);


/** Udělení privelegií **/
GRANT ALL ON BARVA_OČÍ TO xmracn00;
GRANT ALL ON TERITORIUM TO xmracn00;
GRANT ALL ON VNITŘNÍ TO xmracn00;
GRANT ALL ON VENKOVNÍ TO xmracn00;
GRANT ALL ON RASA_KOČEK TO xmracn00;
GRANT ALL ON HOSTITEL TO xmracn00;
GRANT ALL ON KOČKA TO xmracn00;
GRANT ALL ON VĚC TO xmracn00;
GRANT ALL ON ŽIVOT TO xmracn00;
GRANT ALL ON BARVA_RASA TO xmracn00;
GRANT ALL ON RASA_HOSTITEL TO xmracn00;
GRANT ALL ON KOČKA_HOSTITEL TO xmracn00;

GRANT SELECT ON MATERIAL TO xmracn00;

/** Ukázka materializovaného pohledu **/

SELECT * FROM MATERIAL;

SET SERVEROUTPUT ON;

/** Procedura říkající, zdali je daná věc pro kočku postačující (musí jí být více než jedna na den, aby se kočka nenudila)**/
DECLARE
    CURSOR curs IS 
        SELECT typ, kvantita,  kočka_věc_počátek_vlastnictví, kočka_věc_konec_vlastnictví
        FROM Věc;
    iterator Věc%ROWTYPE;
    vysl FLOAT;
BEGIN
    FOR iterator IN curs LOOP
        vysl:=(iterator.kočka_věc_konec_vlastnictví-iterator.kočka_věc_počátek_vlastnictví)/iterator.kvantita;
        IF vysl<1 THEN
            dbms_output.put_line('Věc ' || iterator.typ || ' je postačující');
        ELSE
            dbms_output.put_line('Věc ' || iterator.typ || ' není postačující');
        END IF;
    END LOOP;
EXCEPTION
    WHEN zero_divide then
        dbms_output.put_line('Jedna z věcí neexistuje');
END;
/

/** Procedura počítající průměrný věk hostitelů **/
DECLARE
    průměr_hostitele hostitel.věk%TYPE;
BEGIN
    SELECT AVG(věk)
    INTO průměr_hostitele
    FROM hostitel
    WHERE dostupný=1;
    dbms_output.put_line('Průměrný věk dostupných hostitelů je ' || průměr_hostitele);
END;
/
