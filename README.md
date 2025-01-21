# BitDogLabSimulator

Bibliotecas do SSD1306 e WS2818B retiradas do repositório do BitDogLab: https://github.com/BitDogLab/BitDogLab-C

Bem-vindo ao repositório BitDogLabSimulator! Este projeto contém exemplos de uso de componentes e o arquivo principal para simulação e controle com Raspberry Pi Pico.

Pré-requisitos

Visual Studio Code (VS Code) com as extensões de suporte para Raspberry Pi Pico instaladas.

Raspberry Pi Pico SDK configurado no seu ambiente de desenvolvimento.

Passo a Passo para Configurar o Projeto

1. Criar um Novo Projeto no VS Code

Abra o VS Code e crie um novo projeto com o nome BitDogLabSimulator.Nota: Usar exatamente este nome para o projeto facilita as configurações automáticas do CMake.

Siga as instruções da extensão Raspberry Pi Pico para configurar a estrutura básica do projeto.

2. Clonar o Repositório

No diretório onde o projeto foi criado, execute o comando:

$ git clone <(https://github.com/IanPerigoUERN/BitDogSimulator.git)>

3. Preparar os Arquivos do Projeto

Copie todos os arquivos de exemplo e o arquivo BitDogLabSimulator.c do repositório para a pasta raiz do seu projeto.

Substitua o arquivo CMakeLists.txt criado automaticamente pelo CMakeLists.txt fornecido no repositório.

Você pode alterar o conteúdo do BitDogLabSimulator.c com seu código e compilar, apenas lembre-se de adicionar as bibliotecas necessárias no código e no Cmakelists.txt em target_link_libraries

4. Configurar os Componentes Adicionais

Para usar a matriz de LED e o display OLED:

Copie as pastas LedMatrix e Display1306 do repositório para a pasta raiz do seu projeto.

Certifique-se de que essas pastas estão no mesmo nível do arquivo principal e do CMakeLists.txt.

5. Compilar o Projeto

Após copiar os arquivos e configurar o CMake, use os comandos ou ferramentas fornecidas pelo VS Code para compilar o projeto.

6. Executar os Exemplos

Este repositório contém quatro exemplos(DIsplay, JoyStick, Microfone, RGB com botões e buzzer e Matriz de led) que demonstram o uso de componentes específicos. Consulte os comentários nos arquivos para detalhes de uso e implementações práticas.

Estrutura do Repositório

BitDogLabSimulator.c: Arquivo principal a ser compilado.

LedMatrix/: Biblioteca para controle de matriz de LED.

Display1306/: Biblioteca para controle de displays OLED.

Exemplos adicionais para referência e teste de componentes.

Siga os passos cuidadosamente para garantir que o projeto seja configurado e compilado com sucesso. Para dúvidas ou problemas, consulte a documentação oficial do Raspberry Pi Pico ou abra uma issue no repositório.

Vídeo com passo a passo : https://youtu.be/0YVhHWwAnI4

