# Projeto 1 - PWM de Software

**Objetivo:** Controlar a saída GPIO conectada ao LED da placa NUCLEO através de uma implementação de PWM (Modulação por Largura de Pulso) gerada por software[cite: 7]. O projeto permite ajustar os parâmetros de frequência e *duty cycle* para visualizar os efeitos práticos de luminosidade e oscilação diretamente na placa[cite: 7].

## Hardware e Software Utilizados
* **Placa:** NUCLEO-L476RG
* **Pino de Saída (LED):** PA5 (LED Verde de Usuário)
* **Ambiente de Desenvolvimento:** STM32CubeIDE
* **Bibliotecas:** HAL (Hardware Abstraction Layer)

## Como Funciona a Lógica do Código
A geração do PWM ocorre sem o uso de periféricos dedicados de hardware (Timers), utilizando funções de atraso (`HAL_Delay`) dentro do loop principal (`while (1)`)[cite: 7].

A função principal, `software_pwm(uint16_t frequency, uint8_t duty_cycle)`, opera em três etapas:
1. **Cálculo do Período:** O período total do ciclo em milissegundos é calculado dividindo 1000 pela frequência desejada (`1000 / frequency`)[cite: 7].
2. **Cálculo dos Tempos (ON/OFF):** O tempo em que o LED permanece aceso (`on_time`) é definido aplicando a porcentagem do *duty cycle* sobre o período total[cite: 7]. O tempo apagado (`off_time`) é o restante do período[cite: 7].
3. **Chaveamento do Pino (GPIO Ajustado):** A função utiliza `HAL_GPIO_WritePin` para ligar e desligar o pino físico da placa (ajustado especificamente para a L476RG), intercalado por `HAL_Delay` com os tempos calculados[cite: 7].

## Como Executar
1. Clone este repositório.
2. Abra o projeto no **STM32CubeIDE**.
3. No arquivo `main.c`, localize as macros de configuração no início do código:
   ```c
   #define PWM_FREQUENCY_HZ 100
   #define DUTY_CYCLE_PERCENT 50
