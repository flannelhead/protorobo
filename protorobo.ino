#include <NewPing.h>

struct moottori {
    const int pinA, pinD;  // A = analoginen pinni, D = digitaalinen pinni
};

NewPing anturi(A4, A5, 50);  // ultraäänianturi pinneissä A4 ja A5,
                             // maksimi tunnistettava etäisyys 50 cm

const moottori vasen = { 5, 6 },   // vasemman moottorin pinnit
               oikea = { 9, 10 };  // oikean moottorin pinnit

const int ledi = 4;  // sinisen ledin pinni
const int maxNopeusV = 85, maxNopeusO = 70,
          kaantymisAika = 350, kaantymisEtaisyys = 30;

// nopeus välillä -255 (täysillä taaksepäin) ... 255 (täysillä eteenpäin)
void asetaMoottorinNopeus(const moottori m, const int nopeus) {
    if (nopeus < 0) {
        digitalWrite(m.pinD, HIGH);
        analogWrite(m.pinA, 255 + nopeus);
    } else {
        digitalWrite(m.pinD, LOW);
        analogWrite(m.pinA, nopeus);
    }
}

void eteenpain() {
    asetaMoottorinNopeus(vasen, maxNopeusV);
    asetaMoottorinNopeus(oikea, maxNopeusO);
}

// suunta: 0 vasen, 1 oikea
void kaanny(const int suunta) {
    asetaMoottorinNopeus(vasen, suunta ? maxNopeusV : -maxNopeusV);
    asetaMoottorinNopeus(oikea, suunta ? -maxNopeusO : maxNopeusO);
}

void alustaMoottori(const moottori m) {
    pinMode(m.pinA, OUTPUT);
    pinMode(m.pinD, OUTPUT);
    asetaMoottorinNopeus(m, 0);
}

void setup() {
    alustaMoottori(vasen);
    alustaMoottori(oikea);
    pinMode(ledi, OUTPUT);

    Serial.begin(9600);

    eteenpain();
}

void loop() {
    int etaisyys = anturi.convert_cm(anturi.ping_median());
    Serial.println(etaisyys);

    if (etaisyys != 0 && etaisyys < kaantymisEtaisyys) {
        digitalWrite(ledi, HIGH);
        kaanny(random(0, 2));
        delay(kaantymisAika);
        digitalWrite(ledi, LOW);
        eteenpain();
    }
}
