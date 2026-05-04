/**
 * @file AMRFID.cpp
 * @brief Implementação da biblioteca AMRFID
 */

#include "AMRFID.h"

// ─────────────────────────────────────────────────────────────────────────────
// Construtor
// ─────────────────────────────────────────────────────────────────────────────

AMRFID::AMRFID(byte ssPin, byte rstPin, int quantidade)
    : _mfrc522(ssPin, rstPin),
      _totalUIDs(0),
      _onAutorizadoCallback(nullptr),
      _onNegadoCallback(nullptr)
{
    // Garante que o limite fique entre 1 e MAX_UIDS
    if (quantidade < 1)        quantidade = 1;
    if (quantidade > MAX_UIDS) quantidade = MAX_UIDS;
    _limite = quantidade;
}

// ─────────────────────────────────────────────────────────────────────────────
// Ciclo de vida
// ─────────────────────────────────────────────────────────────────────────────

void AMRFID::begin() {
    SPI.begin();
    _mfrc522.PCD_Init();
}

// ─────────────────────────────────────────────────────────────────────────────
// Leitura
// ─────────────────────────────────────────────────────────────────────────────

String AMRFID::lerUID() {
    // Nenhum cartão novo presente → retorna string vazia
    if (!_mfrc522.PICC_IsNewCardPresent()) {
        return "";
    }

    // Não conseguiu ler o serial → retorna string vazia
    if (!_mfrc522.PICC_ReadCardSerial()) {
        return "";
    }

    // Converte os bytes do UID para string hexadecimal
    String uid = "";
    for (byte i = 0; i < _mfrc522.uid.size; i++) {
        if (_mfrc522.uid.uidByte[i] < 0x10) {
            uid += "0"; // Zero à esquerda para bytes menores que 0x10
        }
        uid += String(_mfrc522.uid.uidByte[i], HEX);
    }

    // Encerra comunicação com o cartão para permitir nova leitura
    _mfrc522.PICC_HaltA();

    return _normalizar(uid);
}

// ─────────────────────────────────────────────────────────────────────────────
// Gestão de cartões
// ─────────────────────────────────────────────────────────────────────────────

bool AMRFID::adicionarUID(String uid) {
    uid = _normalizar(uid);

    // Lista cheia
    if (_totalUIDs >= _limite) {
        return false;
    }

    // UID já existe na lista (evita duplicatas)
    for (int i = 0; i < _totalUIDs; i++) {
        if (_listaUIDs[i] == uid) {
            return false;
        }
    }

    // Adiciona e incrementa o contador
    _listaUIDs[_totalUIDs] = uid;
    _totalUIDs++;
    return true;
}

bool AMRFID::removerUID(String uid) {
    uid = _normalizar(uid);

    for (int i = 0; i < _totalUIDs; i++) {
        if (_listaUIDs[i] == uid) {
            // Preenche o espaço vazio deslocando os elementos seguintes
            for (int j = i; j < _totalUIDs - 1; j++) {
                _listaUIDs[j] = _listaUIDs[j + 1];
            }
            _listaUIDs[_totalUIDs - 1] = ""; // Limpa última posição
            _totalUIDs--;
            return true;
        }
    }

    return false; // Não encontrado
}

bool AMRFID::verificar(String uid) {
    uid = _normalizar(uid);

    for (int i = 0; i < _totalUIDs; i++) {
        if (_listaUIDs[i] == uid) {
            return true;
        }
    }

    return false;
}

int AMRFID::totalCartoes() {
    return _totalUIDs;
}

int AMRFID::limiteCartoes() {
    return _limite;
}

// ─────────────────────────────────────────────────────────────────────────────
// Callbacks
// ─────────────────────────────────────────────────────────────────────────────

void AMRFID::setOnAutorizado(void (*func)(String uid)) {
    _onAutorizadoCallback = func;
}

void AMRFID::setOnNegado(void (*func)(String uid)) {
    _onNegadoCallback = func;
}

// ─────────────────────────────────────────────────────────────────────────────
// Utilitários internos
// ─────────────────────────────────────────────────────────────────────────────

String AMRFID::_normalizar(String uid) {
    uid.trim();
    uid.toUpperCase();
    return uid;
}
