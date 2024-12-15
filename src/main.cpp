#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;

#define BUTTON_ACCELERATOR 2
#define BUTTON_BRAKE 3

bool buttonAcceleratorState;         // Estado atual do micro switch
bool buttonBrakeState;               // Estado atual do micro switch
unsigned long ultimoTempo = 0;       // Variável para controle de tempo
const int intervalo = 500;           // Intervalo de meio segundo (500 ms)

struct Command {
  uint8_t button;  // ID do botão
  uint8_t action;  // Ação associada (1 para pressionado, 0 para solto)
};

void sendSignal(Command cmd) {
  rf_driver.send((uint8_t *)&cmd, sizeof(cmd));
  rf_driver.waitPacketSent();
  delay(200);
}

void setup() {
  Serial.begin(9600);
  
  rf_driver.init();
  Serial.println("Transmissor iniciado...");

  // Configura o pino do micro switch como entrada
  pinMode(BUTTON_ACCELERATOR, INPUT_PULLUP); // INPUT_PULLUP para evitar resistores externos
  pinMode(BUTTON_BRAKE, INPUT_PULLUP);
}

void loop() {
  // Obtém o estado atual do micro switch
  buttonAcceleratorState = digitalRead(BUTTON_ACCELERATOR);
  buttonBrakeState = digitalRead(BUTTON_BRAKE);

  // Estrutura para enviar comandos
  Command cmd;

  if (millis() - ultimoTempo >= intervalo) {
    ultimoTempo = millis(); // Atualiza o tempo

    // Verifica o estado do micro switch
    if (buttonAcceleratorState == LOW) {
      cmd = {1, 1};
      sendSignal(cmd);
      Serial.println("Acelera");
    } else {
      cmd = {1, 0};
      sendSignal(cmd);
    }
    if (buttonBrakeState == LOW) {
      cmd = {2, 1};
      sendSignal(cmd);
      Serial.println("Freia");
    }
  }
}
