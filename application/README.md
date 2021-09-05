# Application
Camada voltada ao desenvolvimento do software que será responsável pelo processamento e exibição dos dados processados provenientes da leitura dos sensores

## Sensor Data API
API responsável pelo *storage* dos dados no banco de dados e disponibilização dos mesmos via *endpoints* que serão utilizados pelo *front-end*.

### Deploy e Release de nova versão
No momento a API está rodando na plataforma em nuvem do *Heroku*. Para a construção da imagem e execução do container na nuvem, sugere-se que se siga o passo a passo abaixo com os comandos necessários para o deploy da API.

*Pré requisitos*:
- Heroku CLI;
- Docker.
