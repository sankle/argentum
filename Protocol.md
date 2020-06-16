# Protocolo de comunicación

Hacer una descripción...

## Estructura

Cada mensaje que  .. .. . .

*Cuando se utilice ... significará que dependerá de cada caso particular.*

`OPCODE (1 byte) | ...`

Identificas el mensaje con el primer byte recibido (opcode), y para cada caso se prosigue distinto.

---

## Casos generales

| OPCODE | TIPO | DIRECCIÓN | ESTRUCTURA (B) |
|--------|------|-----------|----------------|
| 0 | Respuesta | S -> C | `OPCODE_REPLY (1) + LENGTH (4) + REPLY (LENGTH)` |
| 1 | Mensaje privado | S -> C | `SENDER_LENGTH (4) + SENDER (SENDER_LENGTH) + MSG_LENGTH (4) + MSG (MSG_LENGTH)` |
| 2 | Broadcast | S -> C | `LENGTH (4) + BROADCAST (LENGTH)` |
| 128 | Comando | C -> S | `OPCODE_CMD (1) + ...` |
| 129 | Sign-in | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH)` |
| 130 | Sign-up | C -> S | `USER_LENGTH (4) + USERNAME (USER_LENGTH) + PASS_LENGTH (4) + PASSWORD (PASS_LENGTH) + RACE (1) + KIND (1)` |

---

### Comandos (OP = 0)

| OPCODE | TIPO | ESTRUCTURA (B) |
|--------|------|----------------|
| 0 | Empezar a mover arriba | `-` | 
| 1 | Empezar a mover abajo | `-` | 
| 2 | Empezar a mover a la izquierda | `-` | 
| 3 | Empezar a mover a la derecha | `-` | 
| 4 | Dejar de mover | `-` |
| 5 | Usar arma principal | `POS` |
| 6 | Equipar objeto | `POSICION INVENTARIO (SLOT) (1)` |
| 7 | Meditar | `-` |
| 8 | Resucitar | `-` |
| 9 | Resucitar | `POS` |
| 10 | Curar | `POS` |
| 11 | Listar | `POS` |
| 12 | Depositar | `POS + POSICION INVENTARIO (SLOT) (1)` |
| 13 | Depositar | `POS + POSICION INVENTARIO (SLOT) (1) + CANTIDAD (4)` |
| 14 | Depositar oro | `POS + CANTIDAD (4)` |
| 15 | Retirar | `POS + ID (4)` |
| 16 | Retirar | `POS + ID (4) + CANTIDAD (4)` |
| 17 | Retirar oro | `POS + CANTIDAD (4)` |
| 18 | Comprar | `POS + ID (4)` |
| 19 | Comprar | `POS + ID (4) + CANTIDAD (4)` |
| 20 | Vender | `POS + ID (4)` |
| 21 | Vender | `POS + ID (4) + CANTIDAD (4)` |
| 22 | Tomar | `-` |
| 23 | Tirar | `POSICION INVENTARIO (SLOT) (1)` |
| 24 | Tirar | `POSICION INVENTARIO (SLOT) (1) + CANTIDAD (4)` |
| 25 | Listar jugadores conectados | `` |
| 26 | Mensaje | `NICK_LENGTH (4) + NICKNAME (NICK_LENGTH)` |

* `POS` = `X (4) + Y (4)`

---

## Respuestas (OP = 1)

| OPCODE | TIPO |
|--------|------|
| 0 | Error (rojo) |
| 1 | Informacion (blanco) |
| 2 | Información (verde) |
| 3 | Lista (amarillo) |

---