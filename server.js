/*
  Backend AIoT v1.0 - Node.js + Express + MQTT
  Desenvolvedor: [Seu Nome/Sua Empresa de AIoT]

  Este servidor atua como uma ponte entre a API REST (acessada pelo frontend)
  e a rede MQTT (onde o ESP32 está ouvindo).
*/

// --- BIBLIOTECAS ---
const express = require('express');
const mqtt = require('mqtt');
const cors = require('cors');

// --- CONFIGURAÇÕES ---
const PORT = process.env.PORT || 3000; // Porta onde o servidor web vai rodar
const MQTT_BROKER = 'mqtt://broker.hivemq.com'; // Mesmo broker usado no ESP32

// Tópicos MQTT (DEVEM SER IGUAIS AOS DO ESP32)
const TOPICO_BASE = 'aiot/kit/esp32-01';
const TOPICO_COMANDO = `${TOPICO_BASE}/comando`;
const TOPICO_ESTADO = `${TOPICO_BASE}/estado`;

// --- INICIALIZAÇÃO ---
const app = express();
app.use(cors()); // Habilita o CORS para todas as rotas
app.use(express.json()); // Middleware para entender requisições com corpo em JSON

// Conecta ao Broker MQTT
console.log('Conectando ao Broker MQTT...');
const client = mqtt.connect(MQTT_BROKER);

client.on('connect', () => {
  console.log('Conectado ao Broker MQTT com sucesso!');
  // Aqui poderíamos nos inscrever no tópico de estado para logar ou reagir a mudanças
  // client.subscribe(TOPICO_ESTADO, (err) => {
  //   if (!err) console.log(`Inscrito no tópico de estado: ${TOPICO_ESTADO}`);
  // });
});

client.on('error', (error) => {
  console.error('Erro na conexão MQTT:', error);
});

// --- ROTAS DA API ---

// Rota de teste para verificar se o servidor está no ar
app.get('/', (req, res) => {
  res.send('Servidor AIoT Backend está funcionando!');
});

// Rota principal para controlar os relés
// Ex: /api/rele/1/on ou /api/rele/4/off
app.post('/api/rele/:id/:estado', (req, res) => {
  const { id, estado } = req.params;
  const releId = parseInt(id);

  // Validação simples
  if (isNaN(releId) || releId < 1 || releId > 8 || (estado !== 'on' && estado !== 'off')) {
    return res.status(400).json({ message: 'Parâmetros inválidos.' });
  }

  // Monta a mensagem para o ESP32
  const payload = JSON.stringify({
    rele: releId,
    estado: estado
  });

  // Publica a mensagem no tópico MQTT
  client.publish(TOPICO_COMANDO, payload, (err) => {
    if (err) {
      console.error('Erro ao publicar mensagem:', err);
      return res.status(500).json({ message: 'Erro ao enviar comando para o dispositivo.' });
    }
    console.log(`Comando enviado: ${payload}`);
    res.status(200).json({ message: 'Comando enviado com sucesso!', data: JSON.parse(payload) });
  });
});

// Rota para comandos gerais (ligar/desligar todos)
// Ex: /api/comando/on
app.post('/api/comando/:acao', (req, res) => {
    const { acao } = req.params;

    if (acao !== 'on' && acao !== 'off') {
        return res.status(400).json({ message: 'Ação inválida. Use "on" ou "off".' });
    }

    const payload = JSON.stringify({ comando: acao });

    client.publish(TOPICO_COMANDO, payload, (err) => {
        if (err) {
            console.error('Erro ao publicar comando geral:', err);
            return res.status(500).json({ message: 'Erro ao enviar comando geral.' });
        }
        console.log(`Comando geral enviado: ${payload}`);
        res.status(200).json({ message: 'Comando geral enviado com sucesso!', data: JSON.parse(payload) });
    });
});


// --- INICIA O SERVIDOR ---
app.listen(PORT, () => {
  console.log(`Servidor rodando na porta ${PORT}`);
});
