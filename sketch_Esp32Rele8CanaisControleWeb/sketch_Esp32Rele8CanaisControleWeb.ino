/*
  Controle de Módulo 8 Relés v3.0 - MQTT Edition
  Desenvolvedor: [Relton Lima/PlugaShop Tecnologia]
  Versão: 3.0

  Controla os relés via MQTT, recebendo comandos em formato JSON.
*/

// --- BIBLIOTECAS ---
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// --- CONFIGURAÇÕES DE REDE ---
const char* ssid = "nome-da-rede-wifi-aqui";       // <-- COLOQUE AQUI O NOME DO SEU WIFI
const char* password = "senha-da-rede-wifi-aqui"; // <-- COLOQUE AQUI A SENHA

// --- CONFIGURAÇÕES DO MQTT ---
const char* mqtt_server = "broker.hivemq.com"; // Broker MQTT público para testes
const int mqtt_port = 1883;

// Tópicos MQTT: Usar uma base única ajuda a organizar e evitar colisões
#define TOPICO_BASE "sua-empresa-de-aiot/kit/esp32-01" // Altere "esp32-01" se tiver vários dispositivos
#define TOPICO_COMANDO TOPICO_BASE "/comando"
#define TOPICO_ESTADO TOPICO_BASE "/estado"

// --- CONFIGURAÇÕES DOS PINOS (Sem alterações) ---
const int pinosRele[8] = {26, 25, 33, 32, 27, 14, 12, 13};
const int NUM_RELES = 8;
#define RELE_LIGADO LOW
#define RELE_DESLIGADO HIGH

// --- OBJETOS GLOBAIS ---
WiFiClient espClient;
PubSubClient client(espClient);

// =====================================================================
// FUNÇÃO SETUP
// =====================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- Iniciando Sistema de Controle v3.0 (MQTT) ---");

  // Configura os pinos dos relés e os desliga
  for (int i = 0; i < NUM_RELES; i++) {
    pinMode(pinosRele[i], OUTPUT);
    digitalWrite(pinosRele[i], RELE_DESLIGADO);
  }

  setup_wifi(); // Conecta ao Wi-Fi
  client.setServer(mqtt_server, mqtt_port); // Configura o servidor MQTT
  client.setCallback(callback); // Define a função que será chamada ao receber mensagens
}

// =====================================================================
// FUNÇÃO DE CALLBACK: O Coração da Lógica MQTT
// Esta função é chamada AUTOMATICAMENTE pela biblioteca quando uma mensagem
// chega em um tópico que o ESP32 está inscrito.
// =====================================================================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico [");
  Serial.print(topic);
  Serial.print("]: ");
  
  // Converte o payload (bytes) para uma String
  String mensagem;
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.println(mensagem);

  // --- Processamento do JSON ---
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, mensagem);

  if (error) {
    Serial.print("Falha ao ler JSON: ");
    Serial.println(error.c_str());
    return;
  }

  // Extrai os valores do JSON
  // Ex: {"rele": 1, "estado": "on"} ou {"comando": "on"}
  if (doc.containsKey("comando")) {
    String comandoGeral = doc["comando"]; // "on" ou "off"
    if (comandoGeral == "on") ligarTodosReles();
    if (comandoGeral == "off") desligarTodosReles();
  } else {
    int releId = doc["rele"];       // ex: 1
    String estado = doc["estado"]; // ex: "on"

    // Valida o ID do relé (1-8)
    if (releId >= 1 && releId <= NUM_RELES) {
      if (estado == "on") {
        digitalWrite(pinosRele[releId - 1], RELE_LIGADO);
      } else if (estado == "off") {
        digitalWrite(pinosRele[releId - 1], RELE_DESLIGADO);
      }
    }
  }

  // Após qualquer alteração, publica o novo estado de todos os relés
  publicarEstado();
}

// =====================================================================
// FUNÇÃO LOOP: Apenas mantém as conexões ativas
// =====================================================================
void loop() {
  if (!client.connected()) {
    reconnect(); // Se perder a conexão, tenta reconectar
  }
  client.loop(); // ESSENCIAL: Processa as mensagens MQTT e mantém a conexão
}

// =====================================================================
// FUNÇÕES AUXILIARES
// =====================================================================

// Conecta ao Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.print("\nConectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Reconecta ao Broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao Broker MQTT...");
    String clientId = "ESP32Client-"; // Cria um ID de cliente único
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      // Se conecta e se inscreve no tópico de comando
      client.subscribe(TOPICO_COMANDO);
      Serial.print("Inscrito no tópico: ");
      Serial.println(TOPICO_COMANDO);
      publicarEstado(); // Publica o estado inicial ao conectar
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// Publica o estado atual de todos os relés no tópico de estado
void publicarEstado() {
  StaticJsonDocument<256> doc;
  for(int i=0; i < NUM_RELES; i++){
    // Armazena o estado como "on" ou "off"
    doc[String(i+1)] = (digitalRead(pinosRele[i]) == RELE_LIGADO) ? "on" : "off";
  }
  
  char buffer[256];
  size_t n = serializeJson(doc, buffer); // Converte o JSON para String

  client.publish(TOPICO_ESTADO, buffer, n); // Publica
  Serial.print("Estado publicado em ");
  Serial.println(TOPICO_ESTADO);
}

// Funções para ligar/desligar todos (chamadas pelo callback)
void ligarTodosReles(){
  for (int i = 0; i < NUM_RELES; i++) {
    digitalWrite(pinosRele[i], RELE_LIGADO);
  }
}
void desligarTodosReles(){
  for (int i = 0; i < NUM_RELES; i++) {
    digitalWrite(pinosRele[i], RELE_DESLIGADO);
  }
}
