# AMRFID

> Camada de abstração sobre a biblioteca MFRC522 que simplifica a leitura e validação de cartões RFID no Arduino.

[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue?logo=arduino)](https://www.arduino.cc/reference/en/libraries/)
[![Version](https://img.shields.io/badge/version-1.0.0-green)]()

---

## ✨ Funcionalidades

- Leitura de UID simplificada — uma linha de código
- Lista de cartões autorizados com limite configurável (máx. `MAX_UIDS = 15`)
- Callbacks personalizados para acesso permitido e negado
- Normalização automática de UIDs (maiúsculas, sem espaços)
- Sem duplicatas na lista de cartões
- API limpa e bem documentada

---

## 📦 Instalação

### Via Arduino IDE (recomendado)
1. Acede a **Sketch → Include Library → Manage Libraries...**
2. Pesquisa por `AMRFID`
3. Clica em **Install**

### Via ficheiro ZIP
1. Faz download do [último release](https://github.com/guicaiala/AMRFID/releases)
2. No Arduino IDE: **Sketch → Include Library → Add .ZIP Library...**
3. Seleciona o ficheiro `AMRFID.zip`

### Dependência obrigatória
A biblioteca requer a **MFRC522** — instala também pelo Library Manager:
> Pesquisa por `MFRC522` de *miguelbalboa*

---

## 🔌 Ligações de Hardware

| MFRC522 | Arduino Uno/Nano |
|---------|-----------------|
| SDA     | Pino 10 (SS)    |
| SCK     | Pino 13         |
| MOSI    | Pino 11         |
| MISO    | Pino 12         |
| RST     | Pino 9          |
| 3.3V    | 3.3V            |
| GND     | GND             |

> ⚠️ **Atenção:** O módulo MFRC522 opera a **3.3V**. Não ligue os pinos de dados diretamente a 5V sem um divisor de tensão ou conversor de nível lógico.

---

## 🚀 Início Rápido

```cpp
#include <AMRFID.h>

AMRFID rfid; // Pinos padrão: SS=10, RST=9, limite=10 cartões

void permitido(String uid) {
    Serial.println("Acesso permitido: " + uid);
}

void negado(String uid) {
    Serial.println("Acesso negado: " + uid);
}

void setup() {
    Serial.begin(9600);
    rfid.begin();

    rfid.adicionarUID("A1B2C3D4");
    rfid.setOnAutorizado(permitido);
    rfid.setOnNegado(negado);
}

void loop() {
    String uid = rfid.lerUID();
    if (uid != "") {
        rfid.verificar(uid) ? permitido(uid) : negado(uid);
        delay(1500);
    }
}
```

---

## 📖 Referência da API

### Construtor

```cpp
AMRFID rfid;                        // Padrão: SS=10, RST=9, limite=10
AMRFID rfid(10, 9);                 // Pinos explícitos, limite=10
AMRFID rfid(10, 9, 5);             // Pinos explícitos, limite=5 cartões
AMRFID rfid(10, 9, MAX_UIDS);      // Limite máximo (15 cartões)
```

---

### Métodos

#### `void begin()`
Inicializa o SPI e o módulo RFID. Deve ser chamado no `setup()`.

```cpp
rfid.begin();
```

---

#### `String lerUID()`
Tenta ler o UID do cartão presente no leitor.  
Retorna o UID em hexadecimal maiúsculo (ex: `"A1B2C3D4"`) ou `""` se nenhum cartão for detectado.

```cpp
String uid = rfid.lerUID();
if (uid != "") {
    Serial.println("Cartão lido: " + uid);
}
```

---

#### `bool adicionarUID(String uid)`
Adiciona um UID à lista de autorizados.  
Retorna `true` se adicionado com sucesso, `false` se a lista estiver cheia ou o UID já existir.

```cpp
rfid.adicionarUID("A1B2C3D4");
```

---

#### `bool removerUID(String uid)`
Remove um UID da lista de autorizados.  
Retorna `true` se removido, `false` se não encontrado.

```cpp
rfid.removerUID("A1B2C3D4");
```

---

#### `bool verificar(String uid)`
Verifica se um UID está na lista de autorizados.

```cpp
if (rfid.verificar(uid)) {
    // acesso permitido
}
```

---

#### `int totalCartoes()`
Retorna o número de cartões atualmente registados.

```cpp
Serial.println(rfid.totalCartoes()); // ex: 3
```

---

#### `int limiteCartoes()`
Retorna o limite configurado no construtor.

```cpp
Serial.println(rfid.limiteCartoes()); // ex: 10
```

---

#### `void setOnAutorizado(void (*func)(String uid))`
Define a função de callback chamada quando o acesso é **permitido**.

```cpp
void minhaFuncao(String uid) { /* ... */ }
rfid.setOnAutorizado(minhaFuncao);
```

---

#### `void setOnNegado(void (*func)(String uid))`
Define a função de callback chamada quando o acesso é **negado**.

```cpp
void minhaFuncao(String uid) { /* ... */ }
rfid.setOnNegado(minhaFuncao);
```

---

### Constante

| Constante  | Valor | Descrição                        |
|------------|-------|----------------------------------|
| `MAX_UIDS` | `15`  | Limite máximo absoluto de cartões |

---

## 💡 Exemplos incluídos

| Exemplo              | Descrição                                                         |
|----------------------|-------------------------------------------------------------------|
| `ExemploBasico`      | Leitura e validação simples via Serial Monitor                   |
| `ControleAcesso`     | Controle de acesso com LED, buzzer e relé                        |
| `GerenciadorCartoes` | Modo administrador via Serial para registar/remover cartões      |

Acede aos exemplos em: **File → Examples → AMRFID**

---

## 🗂️ Estrutura do Projeto

```
AMRFID/
├── AMRFID.h                              # Interface pública
├── AMRFID.cpp                            # Implementação
├── library.properties                    # Metadados Arduino
├── keywords.txt                          # Destaque de sintaxe
├── README.md                             # Este ficheiro
└── examples/
    ├── ExemploBasico/
    │   └── ExemploBasico.ino
    ├── ControleAcesso/
    │   └── ControleAcesso.ino
    └── GerenciadorCartoes/
        └── GerenciadorCartoes.ino
```

---

## 🤝 Contribuir

Contribuições são bem-vindas!

1. Faz um fork do repositório
2. Cria um branch para a tua funcionalidade (`git checkout -b feature/nova-funcionalidade`)
3. Faz commit das tuas alterações (`git commit -m 'Adiciona nova funcionalidade'`)
4. Faz push para o branch (`git push origin feature/nova-funcionalidade`)
5. Abre um Pull Request



---

## 👤 Autor

**Gui Caiala**  
- GitHub: [@guicaiala](https://github.com/guicaiala)
- Email: guicaiala@gmail.com

---

## ⭐ Gostaste do projeto?

Deixa uma estrela no repositório — ajuda outros makers a encontrar a biblioteca!

## License

This project is licensed under the MIT License.
