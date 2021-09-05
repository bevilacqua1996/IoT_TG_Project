# Application
Camada voltada ao desenvolvimento do software que será responsável pelo processamento e exibição dos dados processados provenientes da leitura dos sensores

## Sensor Data API
API responsável pelo *storage* dos dados no banco de dados e disponibilização dos mesmos via *endpoints* que serão utilizados pelo *front-end*.

### Deploy e Release de nova versão
No momento a API está rodando na plataforma em nuvem do *Heroku*. Para a construção da imagem e execução do container na nuvem, sugere-se que se siga o passo a passo abaixo com os comandos necessários para o deploy da API.

*Pré requisitos*:
- Heroku CLI [Download](https://devcenter.heroku.com/articles/heroku-cli#download-and-install);
- Docker [Download](https://www.docker.com/products/docker-desktop);.

```
"Login"
heroku login
heroku container:login

"Executar na pasta com o Dockerfile"
heroku container:push web --app sensordataapi
heroku container:release web --app sensordataapi
```

Outros comandos interessantes para gerenciamento do container na nuvem são os seguintes:

```
"criação manual da imagem"
docker build -t sensor_data_api .

"logs"
heroku logs --tail -a sensordataapi

"Stop do container"
heroku ps:stop web -a sensordataapi

"Restart do container"
heroku ps:restart web -a sensordataapi

"Remoção do container"
heroku container:rm web -a sensordataapi

"Listagem de containers"
heroku ps
```

### Endpoints

#### Get All Data

*Endpoint* que faz *retrieve* de todos os dados persistidos no banco de dados até então

- *Endereço*: /sensordata
- *Método*: GET
- *Retorno*: JSON com todos os objetos da entidade Sensor Data armazenados até então.

#### Add new Data

*Endpoint* que adiciona novo objeto da entidade Sensor Data.

- *Endereço*: /sensordata
- *Método*: POST
- *Parâmetros*: 
  - *Body*: JSON com atributos da entidade Sensor Data.
