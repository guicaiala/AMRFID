#include <AMRFID.h>


// ===============================
// CRIAÇÃO DO OBJETO RFID
// ===============================
// ── Instância com pinos e limite padrão (SS=10, RST=9, max=10 cartões) ────────
AMRFID rfid;

// ===============================
// FUNÇÃO SETUP (executa 1 vez)
// ===============================
void setup() {
    // Inicializa comunicação serial
    // Usada para mostrar o UID no monitor serial
    Serial.begin(9600);

    // Inicializa o módulo RFID
    rfid.begin();

    // Mensagem inicial para o usuário
    Serial.println("=== AMRFID: Leitura de UID ===");
    Serial.println("Aproxime um cartão RFID...");
}

// ===============================
// FUNÇÃO LOOP (executa continuamente)
// ===============================
void loop() {

    // ===============================
    // LEITURA DO UID
    // ===============================
    // A função lerUID():
    // - Verifica se há um cartão presente
    // - Lê o UID se existir
    // - Retorna uma String com o UID
    // - Retorna "" (vazio) se não houver cartão
    String uid = rfid.lerUID();

    // ===============================
    // VERIFICA SE UM CARTÃO FOI LIDO
    // ===============================
    if (uid != "") {

        // Mostra o UID no monitor serial
        Serial.print("UID detectado: ");
        Serial.println(uid);

        // Pequena pausa para evitar múltiplas leituras do mesmo cartão
        delay(1000);

        Serial.println("Aproxime outro cartão...");
    }

    // Se não houver cartão, o loop continua rodando
}
