/**
 * ControleAcesso.ino
 * ──────────────────
 * Exemplo de controle de acesso físico com:
 *   - LED verde  → acesso permitido
 *   - LED vermelho → acesso negado
 *   - Buzzer     → feedback sonoro
 *   - Relé       → aciona trava/porta
 *
 * Ligações (Arduino Uno / Nano):
 *   MFRC522  →  Arduino
 *   SDA      →  Pino 10
 *   SCK      →  Pino 13
 *   MOSI     →  Pino 11
 *   MISO     →  Pino 12
 *   RST      →  Pino 9
 *   3.3V     →  3.3V
 *   GND      →  GND
 *
 *   LED Verde    →  Pino 4 (com resistor 220Ω)
 *   LED Vermelho →  Pino 5 (com resistor 220Ω)
 *   Buzzer       →  Pino 6
 *   Relé (Porta) →  Pino 7
 */

#include <AMRFID.h>

// ── Pinos dos atuadores ───────────────────────────────────────────────────────
#define PINO_LED_VERDE    4
#define PINO_LED_VERMELHO 5
#define PINO_BUZZER       6
#define PINO_RELE         7

// ── Instância AMRFID com limite de 5 cartões ──────────────────────────────────
AMRFID rfid(10, 9, 5);

// ── Funções auxiliares ────────────────────────────────────────────────────────

void beepCurto() {
    tone(PINO_BUZZER, 1000, 100);
}

void beepLongo() {
    tone(PINO_BUZZER, 400, 500);
}

void abrirPorta() {
    digitalWrite(PINO_RELE, HIGH);
    delay(3000); // Porta aberta por 3 segundos
    digitalWrite(PINO_RELE, LOW);
}

// ── Callbacks AMRFID ──────────────────────────────────────────────────────────

void permitido(String uid) {
    Serial.println("✔ ACESSO PERMITIDO → " + uid);

    // LED verde aceso por 3 segundos
    digitalWrite(PINO_LED_VERDE, HIGH);
    beepCurto();

    abrirPorta();

    digitalWrite(PINO_LED_VERDE, LOW);
}

void negado(String uid) {
    Serial.println("✘ ACESSO NEGADO → " + uid);

    // LED vermelho pisca 3 vezes com beep longo
    for (int i = 0; i < 3; i++) {
        digitalWrite(PINO_LED_VERMELHO, HIGH);
        delay(200);
        digitalWrite(PINO_LED_VERMELHO, LOW);
        delay(200);
    }
    beepLongo();
}

// ── Setup ─────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(9600);

    // Configura pinos dos atuadores
    pinMode(PINO_LED_VERDE,    OUTPUT);
    pinMode(PINO_LED_VERMELHO, OUTPUT);
    pinMode(PINO_BUZZER,       OUTPUT);
    pinMode(PINO_RELE,         OUTPUT);

    // Garante que relé começa desligado
    digitalWrite(PINO_RELE, LOW);

    // Inicializa RFID
    rfid.begin();

    // Cartões autorizados
    rfid.adicionarUID("A1B2C3D4"); // Cartão do funcionário 1
    rfid.adicionarUID("B2C3D4E5"); // Cartão do funcionário 2
    rfid.adicionarUID("C3D4E5F6"); // Cartão do gestor

    // Callbacks
    rfid.setOnAutorizado(permitido);
    rfid.setOnNegado(negado);

    Serial.println("=== Sistema de Controle de Acesso ===");
    Serial.print("Cartões autorizados: ");
    Serial.println(rfid.totalCartoes());
    Serial.println("Aguardando cartão...");

    // Pisca LEDs para indicar sistema pronto
    digitalWrite(PINO_LED_VERDE, HIGH);
    digitalWrite(PINO_LED_VERMELHO, HIGH);
    delay(500);
    digitalWrite(PINO_LED_VERDE, LOW);
    digitalWrite(PINO_LED_VERMELHO, LOW);
}

// ── Loop ──────────────────────────────────────────────────────────────────────

void loop() {
    String uid = rfid.lerUID();

    if (uid != "") {
        if (rfid.verificar(uid)) {
            permitido(uid);
        } else {
            negado(uid);
        }

        delay(2000); // Pausa antes de aceitar nova leitura
    }
}
