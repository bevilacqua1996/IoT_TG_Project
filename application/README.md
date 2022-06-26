# Application
Camada voltada ao desenvolvimento do software que será responsável pelo processamento e exibição dos dados processados provenientes da leitura dos sensores

## Sensor Data API
API responsável pelo *storage* dos dados no banco de dados e disponibilização dos mesmos via *endpoints* que serão utilizados pelo *front-end*.

**OBS**: Não se esqueça de preencher as credenciais do banco de dados no arquivo *application.properties*. As credenciais podem ser encontradas na plataforma do Heroku na aplicação *sensordataapi*.

### Deploy e Release de nova versão

#### Deploy Manual
No momento a API está rodando na plataforma em nuvem do *Heroku*. Para a construção da imagem de forma local e execução do container na nuvem, sugere-se que se siga o passo a passo abaixo com os comandos necessários para o deploy da API.

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

#### Deploy automático
Visando a automatização do processo de build e deploy também temos uma esteira automatizada de CI e CD (Continuous Integration e Continuous Deploy) integrada com o sistema do Heroku que fará uso de dois arquivos de configuração principais: *heroku.yml* e *Dockerfile*. O Arquivo de configuração do Heroku se encarregará de referenciar o diretório onde o *Dockerfile* de configuração está armazendao. O *Dockerfile*, por sua vez, é o arquivo que guarda as configurações do ambiente 'containerizado' que fará o build e execução da API na nuvem.

Para execução da esteira, rodar o deploy a partir da plataforma do Heroku apontando para a branch na qual se deseja construir a API. Abaixo temos um diagrama de como funciona a esteira atualmente.

![deploy_strategy](https://user-images.githubusercontent.com/18063196/175832026-5a401e55-5912-498d-a219-a4d22effd2dc.png)

#### Comandos úteis do Heroku Client
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
