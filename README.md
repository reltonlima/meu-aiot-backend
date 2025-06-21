# AIoT Control Deck 🚀

### Um projeto Full-Stack de Automação Residencial (AIoT) para controlar um módulo de 8 relés via web, do ESP32 à nuvem.

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)

![Status do Projeto](https://img.shields.io/badge/status-concluído-brightgreen)

<br>

> **Nota do Desenvolvedor:** Este projeto é uma demonstração completa do ciclo de vida de um produto de Internet das Coisas (IoT), desde a programação de baixo nível de um microcontrolador até a criação de uma API na nuvem e uma interface de usuário moderna e responsiva.

<br>

## 📖 Sobre o Projeto

O **AIoT Control Deck** nasceu como um exercício para controlar um simples módulo de relés via porta serial. Ao longo do seu desenvolvimento, evoluiu para uma aplicação full-stack robusta, incorporando as melhores práticas da indústria para projetos de IoT.

O sistema permite o controle total sobre 8 canais de relé através de uma interface web elegante, acessível por qualquer navegador em um desktop ou smartphone. A comunicação entre o hardware e a nuvem é feita em tempo real utilizando o protocolo MQTT, garantindo respostas instantâneas e baixo consumo de dados.

## ✨ Funcionalidades

* **Controle Remoto via Web:** Interface de usuário moderna e responsiva para controlar dispositivos de qualquer lugar.
* **Comunicação em Tempo Real:** Utilização do protocolo MQTT para latência ultrabaixa entre o comando e a ação.
* **Controle Individual e Global:** Acione relés individualmente com toggles intuitivos ou todos de uma vez com os botões "Ligar Todos" / "Desligar Todos".
* **Arquitetura Desacoplada:** O hardware (ESP32), o backend (Node.js) e o frontend (HTML/CSS/JS) são totalmente independentes, comunicando-se através de um broker central.
* **Backend como Ponte Segura:** Uma API Node.js atua como um intermediário seguro, recebendo requisições web e traduzindo-as para o protocolo MQTT.
* **Firmware Robusto:** O código do ESP32 inclui reconexão automática ao Wi-Fi e ao broker MQTT, garantindo que o dispositivo permaneça online.

## 🛠️ Tecnologias Utilizadas

Este projeto é uma verdadeira demonstração de habilidades full-stack, combinando hardware, nuvem e desenvolvimento web.

#### **Hardware**
* ![ESP32](https://img.shields.io/badge/ESP32-E6007E?style=for-the-badge&logo=espressif&logoColor=white)
* Módulo Relé de 8 Canais

#### **Firmware (Código do ESP32)**
* ![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
* **Bibliotecas:** `PubSubClient` (para MQTT) e `ArduinoJson`.

#### **Backend**
* ![Node.js](https://img.shields.io/badge/Node.js-339933?style=for-the-badge&logo=nodedotjs&logoColor=white)
* ![Express.js](https://img.shields.io/badge/Express.js-000000?style=for-the-badge&logo=express&logoColor=white)
* **Protocolo:** MQTT
* **Servidor:** DigitalOcean Droplet (ou qualquer VPS)
* **Gerenciador de Processos:** PM2

#### **Frontend**
* ![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
* ![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3&logoColor=white)
* ![JavaScript](https://img.shields.io/badge/JavaScript-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)

## 🚀 Configuração e Instalação

Para replicar este projeto, siga os três estágios de configuração:

### 1. Firmware (ESP32)

1.  **Clone o repositório:**
    ```bash
    git clone [https://github.com/reltonlima/meu-aiot-backend.git](https://github.com/reltonlima/meu-aiot-backend.git)
    ```
2.  **Abra o Sketch:** Navegue até a pasta do firmware e abra o arquivo `.ino` na Arduino IDE.
3.  **Instale as Bibliotecas:** Pelo Gerenciador de Bibliotecas, instale `PubSubClient` e `ArduinoJson`.
4.  **Configure suas Credenciais:** No topo do arquivo do sketch, insira o nome (`ssid`) e a senha (`password`) da sua rede Wi-Fi.
5.  **Faça o Upload:** Conecte o ESP32 ao computador e faça o upload do código. Abra o Monitor Serial para verificar os logs de conexão.

### 2. Backend (Servidor Node.js)

1.  **Acesse seu servidor** (ex: via SSH para a DigitalOcean).
2.  **Navegue até a pasta do backend** no repositório clonado.
3.  **Instale as dependências:**
    ```bash
    npm install
    ```
4.  **Inicie o servidor em modo de produção com PM2:**
    ```bash
    pm2 start server.js --name "aiot-backend"
    ```

### 3. Frontend

1.  **Abra o arquivo `index.html`** em um editor de código.
2.  **Configure a URL da API:** Encontre a constante `API_URL` no bloco `<script>` e substitua o valor `'http://IP_DO_SEU_SERVIDOR:3000'` pelo endereço IP público do seu servidor.
3.  **Hospede o arquivo:** Para acessar de qualquer lugar (especialmente do celular), hospede o arquivo `index.html` em um servidor web (pode ser no mesmo Droplet com Nginx/Apache) ou use um serviço como Netlify, Vercel ou GitHub Pages.

##  license Licença

Distribuído sob a Licença MIT. Veja o arquivo `LICENSE` para mais informações.

## 👤 Contato

**[Relton Lima]**

* **LinkedIn:** [https://linkedin.com/in/reltonlima](https://linkedin.com/in/reltonfidelis)
* **GitHub:** [@reltonlima](https://github.com/reltonlima)
* **Portfólio:** [pluha.shop](https://pluha.shop)

<br>
<p align="center">
  Desenvolvido com ❤️ e muito café.
</p>
