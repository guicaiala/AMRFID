/**
 * GerenciadorCartoes.ino
 * ──────────────────────
 * Exemplo avançado com modo administrador via Serial Monitor.
 * Permite registar e remover cartões em tempo real sem
 * modificar o código.
 *
 * Comandos via Serial Monitor (9600 baud):
 *   LISTAR         → mostra todos os UIDs cadastrados
 *   ADMIN          → entra no modo administrador
 *   SAIR           → sai do modo administrador
 *
 * No modo administrador:
 *   - Qualquer cartão aproximado é REGISTADO (se não existir)
 *     ou REMOVIDO (se já existir) — funciona como toggle
 *
 * No modo normal:
 *   - Cartões são apenas validados (permitido / negado)
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
 *   LED Azul (modo admin) → Pino 8 (com resistor 220Ω)
 */

#include <AMRFID.h>

#define PINO_LED_ADMIN 8

// ── Instância com limite máximo de 15 cartões ─────────────────────────────────
AMRFID rfid(10, 9, MAX_UIDS);

bool modoAdmin = false;

// ── Callbacks AMRFID ──────────────────────────────────────────────────────────

void permitido(String uid) {
    Serial.println("[✔] Acesso PERMITIDO → " + uid);
}

void negado(String uid) {
    Serial.println("[✘] Acesso NEGADO → " + uid);
}

// ── Funções do modo administrador ─────────────────────────────────────────────

void entrarAdmin() {
    modoAdmin = true;
    digitalWrite(PINO_LED_ADMIN, HIGH);
    Serial.println();
    Serial.println("╔══════════════════════════════╗");
    Serial.println("║   MODO ADMINISTRADOR ATIVO   ║");
    Serial.println("╚══════════════════════════════╝");
    Serial.println("Aproxime um cartão para REGISTAR ou REMOVER.");
    Serial.println("Digite SAIR para voltar ao modo normal.");
}

void sairAdmin() {
    modoAdmin = false;
    digitalWrite(PINO_LED_ADMIN, LOW);
    Serial.println();
    Serial.println("── Modo administrador encerrado ──");
    Serial.println("Voltando ao modo de validação...");
}

void listarCartoes() {
    Serial.println();
    Serial.print("── Cartões registados: ");
    Serial.print(rfid.totalCartoes());
    Serial.print(" / ");
    Serial.print(rfid.limiteCartoes());
    Serial.println(" ──");

    if (rfid.totalCartoes() == 0) {
        Serial.println("  (nenhum cartão registado)");
    }
    // Nota: a listagem individual requer acesso à lista interna.
    // Neste exemplo, o registo é feito externamente para manter
    // o histórico dos UIDs adicionados.
    Serial.println();
}

void processarComandoSerial(String cmd) {
    cmd.trim();
    cmd.toUpperCase();

    if (cmd == "ADMIN") {
        if (!modoAdmin) entrarAdmin();
        else Serial.println("Já está no modo administrador.");
    }
    else if (cmd == "SAIR") {
        if (modoAdmin) sairAdmin();
        else Serial.println("Não está no modo administrador.");
    }
    else if (cmd == "LISTAR") {
        listarCartoes();
    }
    else if (cmd.length() > 0) {
        Serial.println("Comando desconhecido: " + cmd);
        Serial.println("Comandos disponíveis: ADMIN, SAIR, LISTAR");
    }
}

void processarCartaoAdmin(String uid) {
    if (rfid.verificar(uid)) {
        // Cartão já existe → remove
        rfid.removerUID(uid);
        Serial.println("[−] Cartão REMOVIDO → " + uid);
    } else {
        // Cartão novo → adiciona
        bool ok = rfid.adicionarUID(uid);
        if (ok) {
            Serial.println("[+] Cartão REGISTADO → " + uid);
        } else {
            Serial.println("[!] Limite atingido! Não foi possível registar → " + uid);
        }
    }

    Serial.print("    Total actual: ");
    Serial.print(rfid.totalCartoes());
    Serial.print(" / ");
    Serial.println(rfid.limiteCartoes());
}

// ── Setup ─────────────────────────────────────────────────────────────────────

void setup() {
    Serial.begin(9600);
    while (!Serial);

    pinMode(PINO_LED_ADMIN, OUTPUT);

    rfid.begin();
    rfid.setOnAutorizado(permitido);
    rfid.setOnNegado(negado);

    // Cartão pré-registado de fábrica
    rfid.adicionarUID("A1B2C3D4");

    Serial.println("==============================");
    Serial.println("  Gerenciador de Cartões RFID");
    Serial.println("==============================");
    Serial.println("Comandos: ADMIN | SAIR | LISTAR");
    Serial.println("Modo atual: VALIDAÇÃO");
    Serial.println();
}

// ── Loop ──────────────────────────────────────────────────────────────────────

void loop() {
    // Verifica comandos enviados pelo Serial Monitor
    if (Serial.available()) {
        String cmd = Serial.readStringUntil('\n');
        processarComandoSerial(cmd);
    }

    // Lê cartão
    String uid = rfid.lerUID();

    if (uid != "") {
        if (modoAdmin) {
            // No modo admin: regista ou remove o cartão
            processarCartaoAdmin(uid);
        } else {
            // No modo normal: valida o cartão
            if (rfid.verificar(uid)) {
                permitido(uid);
            } else {
                negado(uid);
            }
        }

        delay(1500);
    }
}
