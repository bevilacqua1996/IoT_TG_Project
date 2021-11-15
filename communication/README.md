# Communication

Camada que tem como objetivo a implementação do firmware responsável pela coleta dos dados dos sensores e envio para API.

## Sensor Data Sender

A camada de envio (*sender*) será utilizada para a leitura de dados dos sensores e envio por protocolo LoRa para um *Gateway* responável pelo roteamento desses dados para o banco de dados.

**OBS**: Não se esqueça de adicionar as bibliotecas relacionadas à extração de dados dos sensores da camada *sensors*. É aqui que elas deverão ser utilizadas.

## Sensor Data Receiver

Esta é a camada de implementação do *Gateway*. O *Gateway* será responsável pelo tratamento do pacote de informações enviado pelo *Sender* e envio para a API via método POST.

**OBS**: Não se esqueça de atualizar as constantes de SSID e PASSWORD no arquivo *SensorDataReceiver.ino* com informações do seu roteador para configuração da conexão WiFi.

### Support

Biblioteca que possui métodos de suporte para o tratamento dos dados recebidos pelo *Sender*.

### Sensor Data JSON

Biblioteca de construção do objeto JSON enviado para a API via método POST.

**Atributos:**

- rpm;
- voltage;
- timestamp.

### Rest Mapping

Biblioteca com os métodos possíveis de comunicação com a API.

**Métodos disponíveis:**

- **PostData**: Método POST de adição de novo conjunto de dados. 
  - **Parâmetros**: JSON *body* com os atributos da entidade Sensor Data.
