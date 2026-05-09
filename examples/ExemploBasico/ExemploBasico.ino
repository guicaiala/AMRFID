/**
 * ExemploBasico.ino
 * ─────────────────
 * Demonstra o fluxo completo da biblioteca AMRFID:
 *   1. Inicializar o leitor
 *   2. Registar cartões autorizados
 *   3. Definir callbacks de acesso
 *   4. Ler e validar cartões no loop
 *
 * Ligações (Arduino Uno / Nano):
 *   MFRC522  →  Arduino
 *   SDA      →  Pino 10 (SS padrão)
 *   SCK      →  Pino 13
 *   MOSI     →  Pino 11
 *   MISO     →  Pino 12
 *   RST      →  Pino 9  (RST padrão)
 *   3.3V     →  3.3V
 *   GND      →  GND
 *
 * Dependências:
 *   - AMRFID  (esta biblioteca)
 *   - MFRC522 (instale via Gerenciador de Bibliotecas)
 */

#include <AMRFID.h>

// ── Instância com pinos e limite padrão (SS=10, RST=9, max=10 cartões) ────────
AMRFID rfid;

// ── Callbacks ─────────────────────────────────────────────────────────────────

void permitido(String uid) {
    Serial.print("[OK] Acesso permitido → UID: ");
    Serial.println(uid);
    // Aqui podes acionar relé, LED verde, buzzer, etc.
}

void negado(String uid) {
    Serial.print("[X]  Acesso negado   → UID: ");
    Serial.println(uid);
    // Aqui podes acionar LED vermelho, alarme, etc.
}

// ── Setup ─────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(9600);
    while (!Serial); // Aguarda Serial (necessário em algumas placas)

    rfid.begin();

    // Registar cartões autorizados
    rfid.adicionarUID("A1B2C3D4");
    rfid.adicionarUID("1A2B3C4D");

    // Associar callbacks
    rfid.setOnAutorizado(permitido);
    rfid.setOnNegado(negado);

    Serial.println("── Sistema RFID pronto ──");
    Serial.print("Cartões registados: ");
    Serial.print(rfid.totalCartoes());
    Serial.print(" / ");
    Serial.println(rfid.limiteCartoes());
    Serial.println("Aproxime um cartão...");
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

        delay(1500); // Pequena pausa para evitar leituras repetidas
    }
}
