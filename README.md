## Sobre o Jogo

### Nome
*(a definir)*

### Integrantes do grupo
- Luísa Feitosa Magalhães
- Laura Vitória da Silva Alves
- Pedro José Oliveira e Silva

### Ambientação
O jogo se passa no planeta **Erid**, um mundo altamente tecnológico que está sendo invadido por forças alienígenas. O jogador assume o papel de defensor do planeta, posicionando estruturas estratégicas para impedir que os invasores alcancem a base.

---

## Sistema de Batalha

### Campo de batalha
- Grid **5 linhas × 9 colunas**
- Os aliens surgem na **coluna 8** (direita) e avançam para a **coluna 0** (esquerda)
- O jogador posiciona defesas nas colunas da esquerda para criar linhas de resistência
- Se um alien atingir a coluna 0, o planeta perde uma vida

### Recursos do jogador
| Recurso | Valor inicial | Descrição |
|---|---|---|
| Energia | 50 | Usada para construir defesas |
| Vidas | 5 | Resistência da base; zera = derrota |
| Score | 0 | Aumenta ao eliminar aliens |
| Onda | 1 | Dificuldade cresce a cada onda |

---

## Defesas

| Defesa | Vida | Custo | Dano | Cadência | Especial |
|---|---|---|---|---|---|
| Gerador | 100 | 10 | — | — | +25 energia/turno |
| Torreta | 100 | 100 | 10 | a cada 2 turnos | Ataca o alien mais próximo à frente |
| Muro | 300 | 10 | — | — | Bloqueia e absorve dano |
| Bomba | 1 | 100 | 200 | — | Explode em área (raio 1) ao ser atingida |

---

## Aliens

| Tipo | Vida | Velocidade | Dano | Característica |
|---|---|---|---|---|
| Invasor | 10 | 1 col/turno | 10 | Padrão |
| Blindado | 100 | 1 col/turno | 20 | Alta resistência |
| Kamikaze | 10 | 2 col/turno | 100 | Rápido e altamente destrutivo |

---

## Loop do Jogo
1. O jogador posiciona ou mantém defesas (gasta energia)
2. Geradores produzem energia
3. Torretas atacam os aliens à sua frente
4. Aliens avançam (de acordo com sua velocidade)
5. Aliens que encontram uma defesa a atacam
6. Aliens que chegam à coluna 0 causam dano ao planeta

---

## Condições de Fim de Jogo

**Vitória** — sobreviver a todas as ondas de invasão

**Derrota** — o planeta perder todas as 5 vidas

---

## Estrutura do Código

```
tipos.h     — constantes e macros globais (dimensões, tipos de alien/defesa)
alien.h/c   — estrutura Alien, criação, remoção e destruição da lista encadeada
defesa.h/c  — estrutura Defesa, criação e liberação de memória
jogo.h/c    — estado global (grid, aliens por linha, energia, vidas, score, cursor)
```

### Estado atual de implementação
- [x] Estruturas de dados (Alien, Defesa, Celula, Jogo)
- [x] Criação de aliens com atributos por tipo
- [x] Lista encadeada de aliens por linha com inserção e remoção
- [x] Criação de defesas com atributos por tipo
- [x] Inicialização e destruição segura do estado do jogo
- [ ] Loop principal de turnos
- [ ] Movimentação dos aliens
- [ ] Combate (torreta × alien, alien × defesa)
- [ ] Spawn de ondas
- [ ] Explosão em área da bomba
- [ ] Renderização do grid no terminal
- [ ] Input do jogador (posicionamento com cursor)