/**
 * @file AMRFID.h
 * @brief Biblioteca de abstração para leitura RFID com MFRC522
 *
 * Camada de abstração sobre a biblioteca MFRC522 que simplifica
 * a leitura de cartões RFID, separando leitura, validação e ação.
 *
 * Dependências:
 *   - MFRC522 (instale via Gerenciador de Bibliotecas do Arduino IDE)
 *   - SPI (incluída no Arduino core)
 *
 * @author   Seu Nome
 * @version  1.0.0
 * @date     2025
 */

#ifndef AMRFID_H
#define AMRFID_H

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

// ─── Limite máximo absoluto de cartões ───────────────────────────────────────
#define MAX_UIDS 15

/**
 * @class AMRFID
 * @brief Encapsula todo o sistema de leitura e validação RFID.
 *
 * Uso básico:
 * @code
 *   AMRFID rfid;              // pinos padrão: SS=10, RST=9, limite=10
 *   rfid.begin();
 *   rfid.adicionarUID("A1B2C3D4");
 *   rfid.setOnAutorizado(minhaFuncaoPermitido);
 *   rfid.setOnNegado(minhaFuncaoNegado);
 *
 *   // No loop():
 *   String uid = rfid.lerUID();
 *   if (uid != "") {
 *       rfid.verificar(uid) ? permitido(uid) : negado(uid);
 *   }
 * @endcode
 */
class AMRFID {
  public:

    /**
     * @brief Construtor principal.
     *
     * @param ssPin    Pino Slave Select (SS/SDA) do módulo RFID. Padrão: 10.
     * @param rstPin   Pino Reset (RST) do módulo RFID.         Padrão:  9.
     * @param quantidade Número máximo de cartões armazenados.
     *                   Valor mínimo: 1. Valor máximo: MAX_UIDS (15).
     *                   Padrão: 10.
     */
    AMRFID(byte ssPin = 10, byte rstPin = 9, int quantidade = 10);

    // ─── Ciclo de vida ────────────────────────────────────────────────────────

    /**
     * @brief Inicializa SPI e o módulo RFID.
     *        Deve ser chamado dentro de setup().
     */
    void begin();

    // ─── Leitura ──────────────────────────────────────────────────────────────

    /**
     * @brief Tenta ler o UID do cartão presente no leitor.
     *
     * @return String com o UID em hexadecimal maiúsculo (ex: "A1B2C3D4"),
     *         ou "" se nenhum cartão foi detectado.
     */
    String lerUID();

    // ─── Gestão de cartões ────────────────────────────────────────────────────

    /**
     * @brief Adiciona um UID à lista de cartões autorizados.
     *
     * @param uid  UID a registar (case-insensitive).
     * @return true  se foi adicionado com sucesso,
     *         false se a lista já está cheia ou o UID já existe.
     */
    bool adicionarUID(String uid);

    /**
     * @brief Remove um UID da lista de cartões autorizados.
     *
     * @param uid  UID a remover.
     * @return true  se foi removido, false se não foi encontrado.
     */
    bool removerUID(String uid);

    /**
     * @brief Verifica se um UID está na lista de autorizados.
     *
     * @param uid  UID a verificar.
     * @return true se autorizado, false caso contrário.
     */
    bool verificar(String uid);

    /**
     * @brief Retorna quantos cartões estão registados.
     */
    int totalCartoes();

    /**
     * @brief Retorna o limite configurado no construtor.
     */
    int limiteCartoes();

    // ─── Callbacks ────────────────────────────────────────────────────────────

    /**
     * @brief Define a função chamada quando o acesso é permitido.
     *
     * @param func  Ponteiro para função com assinatura: void minha_func(String uid)
     *
     * @code
     *   void permitido(String uid) { Serial.println("Bem-vindo: " + uid); }
     *   rfid.setOnAutorizado(permitido);
     * @endcode
     */
    void setOnAutorizado(void (*func)(String uid));

    /**
     * @brief Define a função chamada quando o acesso é negado.
     *
     * @param func  Ponteiro para função com assinatura: void minha_func(String uid)
     */
    void setOnNegado(void (*func)(String uid));

  private:

    // ─── Hardware ─────────────────────────────────────────────────────────────
    MFRC522 _mfrc522;

    // ─── Lista de UIDs ────────────────────────────────────────────────────────
    String  _listaUIDs[MAX_UIDS];
    int     _totalUIDs;
    int     _limite;

    // ─── Callbacks ────────────────────────────────────────────────────────────
    void (*_onAutorizadoCallback)(String uid);
    void (*_onNegadoCallback)(String uid);

    // ─── Utilitários internos ─────────────────────────────────────────────────

    /** Converte UID para maiúsculas e remove espaços. */
    String _normalizar(String uid);
};

#endif // AMRFID_H
