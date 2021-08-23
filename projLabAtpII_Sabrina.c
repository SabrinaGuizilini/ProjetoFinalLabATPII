#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>

// Constantes
#define ARQ_USUARIOS "usuarios.dat"
#define ARQ_LIVROS "livros.dat"
#define ARQ_ALOCACOES "alocacoes.dat"

#define TAM_NOME 51
#define TAM_ENDERECO 101
#define TAM_EMAIL 101
#define TAM_TELEFONE 12
#define TAM_TITULO 51

// Declarando estruturas

typedef struct {
    int idusuario;
    int idlivro;
    char dtprevista[11];
    char dtdevolucao[11];
    char situacao[15]; // pendente, devolvido
} alocacao;

typedef struct {
    int id;
    char nome[TAM_NOME];
    char endereco[TAM_ENDERECO];
    char email[TAM_EMAIL];
    char telefone[TAM_TELEFONE];
    alocacao *livros;
} usuario;

typedef struct {
    int id;
    char titulo[TAM_TITULO];
    char autor[TAM_NOME];
    int qtddisponivel;
} livro;

// conta o total de alocações independente da situação por usuário
int contar_alocacoes_por_usuario(int idusuario)
{
    int qtdtotalalocados = 0;
    FILE* arq;
    alocacao a;

    arq = fopen(ARQ_ALOCACOES,"rb");
    if (arq)
    {
        while (fread(&a, sizeof(alocacao), 1, arq))
        {
            if (a.idusuario==idusuario)
                qtdtotalalocados++;
        }
        fclose(arq); // fecha o arquivo
    }

    return qtdtotalalocados;
}

// busca no arquivo de alocações pelo ID do usuário e preenche
// o vetor passado com a lista encontrada
void buscar_alocacoes_por_usuario(int idusuario, alocacao *alocacoes)
{
    int i=0;
    alocacao a;
    FILE* arq;

    arq = fopen(ARQ_ALOCACOES,"rb");
    if (arq)
    {
        while (fread(&a, sizeof(alocacao), 1, arq))
        {
            if (a.idusuario==idusuario)
            {
                alocacoes[i] = a;
                printf("\n");
                i++;
            }
        }
        fclose(arq); // fecha o arquivo
    }

}

// função usada para "limpar" possíveis
// dados acima do limite do tamanho do membro das estruturas
void restringir_dados_usuario(usuario *u)
{
      u->nome[TAM_NOME-1] = '\0';
      u->endereco[TAM_ENDERECO-1] = '\0';
      u->email[TAM_EMAIL-1] = '\0';
      u->telefone[TAM_TELEFONE-1] = '\0';
}

// varre o arquivo de usuários, contando quantos cadastros tem
int contar_usuarios_cadastrados()
{
    int qtdusuarios = 0;
    FILE* arq;
    usuario u;

    arq = fopen(ARQ_USUARIOS,"rb");
    if (arq)
    {
        while (fread(&u, sizeof(usuario), 1, arq))
            if (u.id>0) // condição para ignorar registros removidos
                qtdusuarios++;
        fclose(arq); // fecha o arquivo
    }

    return qtdusuarios;
}

// Pede os dados do usuário via teclado e retorna
// uma variável com estes dados lidos
usuario obter_dados_usuario()
{
    usuario novo;

    printf("Nome: ");
    gets(novo.nome);

    printf("Endereço: ");
    gets(novo.endereco);

    printf("E-mail: ");
    gets(novo.email);

    printf("Telefone: ");
    gets(novo.telefone);

    return novo;
}

// recebe um usuário e apresenta seus dados em tela
void mostrar_dados_usuario(usuario u)
{
    printf("#%3d   Nome: %s | Endereço: %s\n", u.id, u.nome, u.endereco);
    printf("       E-mail: %s | Telefone: %s\n", u.email, u.telefone);
}

// função usada para formatar as datas previstas e de devolução
void formatar_data(char *dt, int forma)
{
    char ndt[11];

    if(strlen(dt)>0) // ignora datas vazias
    {
        strcpy(ndt, dt); // faz uma cópia da data

        if (forma==1)   // dd/mm/aaaa para aaaammdd
        {
            dt[0] = ndt[6]; dt[1] = ndt[7]; dt[2] = ndt[8]; dt[3] = dt[9]; // aaaa
            dt[4] = ndt[3]; dt[5] = ndt[4]; // mm
            dt[6] = ndt[0]; dt[7] = ndt[1]; // dd
            dt[8] = '\0';
        }
        else    // aaaammdd para dd/mm/aaaa
        {
            dt[0] = ndt[6]; dt[1] = ndt[7]; // dd
            dt[2] = '/';
            dt[3] = ndt[4]; dt[4] = ndt[5]; // mm
            dt[5] = '/';
            dt[6] = ndt[0]; dt[7] = ndt[1]; dt[8] = ndt[2]; dt[9] = ndt[3]; // aaaa
            dt[10] = '\0';
        }
    }
}


// apresenta os dados da alocação em tela
void mostrar_dados_alocacao(alocacao a)
{
    formatar_data(a.dtprevista, 2);
    formatar_data(a.dtdevolucao, 2);

    printf("Usuário: #%d\tLivro: #%d\tSituação: %s\n", a.idusuario, a.idlivro, a.situacao);
    printf("Dt. prevista: %s\tDt. devolução: %s\n", a.dtprevista, a.dtdevolucao);
}


// ordenação do vetor de alocacoes utilizando o algoritmo selection sort
void ordernar_alocacoes(alocacao *alocacoes, int qtd, int ordem)
{
    alocacao alocacaotemp;
    int i, j;

    for (i=0;i<qtd-1;i++)
    {
        for (j=i+1;j<qtd;j++)
        {
            if (ordem==1) // crescente
            {
                if (strcmp(alocacoes[i].dtprevista,alocacoes[j].dtprevista)>0)
                {
                    alocacaotemp = alocacoes[i];
                    alocacoes[i] = alocacoes[j];
                    alocacoes[j] = alocacaotemp;
                }
            }
            else    // decrescente
            {
                if (strcmp(alocacoes[i].dtprevista,alocacoes[j].dtprevista)<0)
                {
                    alocacaotemp = alocacoes[i];
                    alocacoes[i] = alocacoes[j];
                    alocacoes[j] = alocacaotemp;
                }
            }
        }
    }
}


// recebe um usuário e, além de apresentar seus dados em tela,
// também consulta os livros já alocados por ele e mostra e tela
void mostrar_dados_completos_usuario(usuario u)
{
    int i, qtd;
    livro l;
    printf("\n\nUsuário selecionado:\n");
    printf("------------------------------------------------------------------\n");

    mostrar_dados_usuario(u); // mostra os dados utilizando função anterior

    // conta as alocações de um determinado usuário
    qtd = contar_alocacoes_por_usuario(u.id);

    if (qtd>0)
    {

        // aloca memória para guardar as alocações
        u.livros = malloc(sizeof(alocacao)*qtd);

        // faz a busca
        buscar_alocacoes_por_usuario(u.id, u.livros);

        printf("Histórico de alocações de livros em ordem decrescente de\n");
        printf("data prevista para devolução, incluindo os devolvidos.\n");
        printf("------------------------------------------------------------------\n");

        if (u.livros!=NULL)
        {
            // ordena os dados antes de apresentar
            ordernar_alocacoes(u.livros, qtd, 2);

            for(i=0; i<qtd; i++)
            {
                // apresenta utlizando a função que mostra os dados da alocação
                mostrar_dados_alocacao(u.livros[i]);
                printf("------------------------------------------------------------------\n");
            }
        }

        // libera a memória usada
        free(u.livros);
    }
}

// varre o arquivo de usuários e retorna
// o maior ID somado de 1
int obter_novo_codigo_usuario()
{
    int ultimo = 0;
    FILE* arq;
    usuario usuariotemp;
    arq = fopen(ARQ_USUARIOS,"rb");

    if (arq)
    {
        while (fread(&usuariotemp, sizeof(usuario), 1, arq))
        {
            if (usuariotemp.id>0) // condição para ignorar registros removidos
                if (usuariotemp.id>ultimo)
                    ultimo = usuariotemp.id; // guarda o maior encontrado
        }
        fclose(arq); // fecha o arquivo
    }
    return ultimo+1;
}

// funcão que busca um usuário e retorna a posição do cursor dentro do arquivo
// se encontrar o usuário, o valor de "usuariopesq" será o encontrado
int buscar_usuario(usuario *usuariopesq)
{
    int tipo, idpesq=-1, posicao=-1;
    char nomepesq[TAM_NOME]="VAZIO", nometrunc[TAM_NOME];
    FILE* arq;

    printf("Pesquisar usuário cadastrado\n\n");

    printf("1) Pesquisar por código/id\n");
    printf("2) Pesquisar por parte do nome\n");
    do
    {
        printf("Informe o tipo de pesquisa desejada: ");
        scanf("%d", &tipo);
        getchar(); // limpa buffer
    } while(tipo!=1 && tipo!=2); // só sai do loop se o tipo for válido

    if (tipo==1)
    {
        do
        {
            printf("Informe o código/id para pesquisa: ");
            scanf("%d", &idpesq);
            getchar(); // limpa buffer
        } while (idpesq<=0);  // só sai do loop se o id for válido
    }
    else
    {
        do
        {
            printf("Informe parte do nome para pesquisa: ");
            gets(nomepesq);
        } while (strcmp(nomepesq,"")==0);  // só sai do loop se informar parte do nome
    }

    arq = fopen(ARQ_USUARIOS,"rb");

    if (arq)
    {
        while (fread(usuariopesq, sizeof(usuario), 1, arq))
        {
            // guarda na variável "nometrunc", a mesma quantidade de caracteres
            // colocada na variável "nomepesq" para comparar só parte do nome
            strncpy(nometrunc, usuariopesq->nome, strlen(nomepesq));
            nometrunc[strlen(nomepesq)] = '\0';  //seta o final da string, pois "strncpy" não faz automático

            // se parte do nome ou o id for igual, entra no IF
            if (strcmp(nometrunc,nomepesq)==0 || usuariopesq->id == idpesq) {

                posicao = ftell(arq); // guarda a posição onde o registro foi encontrado

                break; // sai do loop
            }
        }
        fclose(arq); // fecha o arquivo
    }

    return posicao;
}


// função que faz a inclusão de um novo usuário no sistema
void incluir_usuario()
{
    usuario novo;
    FILE* arq;

    printf("\nIncluir novo usuário\n\n");

    // obtém os dados a partir da digitação
    novo = obter_dados_usuario();

    // busca o novo id no arquivo
    novo.id = obter_novo_codigo_usuario();

    // abre o arquivo para adicionar um registro
    arq = fopen(ARQ_USUARIOS,"ab+");

    if (arq!=NULL)
    {
        // faz a gravação dos dados
        if (!fwrite(&novo, sizeof(usuario), 1, arq))
            printf("\nErro ao adicionar registro! E01");

        printf("\nRegistro incluido com sucesso!");
    }
    else
    {
        printf("\nErro ao adicionar registro! E02");
    }

    fclose(arq); // fecha o arquivo
}


// função que faz a alteração de um usuário no sistema
void alterar_usuario()
{
    int posicao;
    usuario usuariopesq, usuarioalt;
    FILE* arq;

    printf("\nAlterar usuário\n\n");

    // busca o usuário utilizando a função que dá opção de buscar
    // por id ou parte do nome
    posicao = buscar_usuario(&usuariopesq);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o nome ou id informado.\n");
    }
    else
    {
        printf("\nDados cadastrados atualmente\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_usuario(usuariopesq);
        printf("------------------------------------------------------------------\n\n");

        printf("Informe os novos dados\n\n");
        usuarioalt = obter_dados_usuario(); // pede para o usuário digitar os dados novamente
        usuarioalt.id = usuariopesq.id; // mantém o mesmo ID do usuário

        arq = fopen(ARQ_USUARIOS,"rb+"); // abre o arquivo
        fseek(arq, posicao-(long)sizeof(usuario), SEEK_SET); // posiciona no início do registo
        fwrite(&usuarioalt, sizeof(usuario), 1, arq); // grava o registro, sobrescrevendo o anterior
        fclose(arq); // fecha o arquivo

        printf("\nRegistro alterado com sucesso!\n");
    }
}


// função que faz a exclusão de um usuário no sistema
void excluir_usuario()
{
    int posicao, resp;
    usuario usuarioexc;
    FILE* arq;

    printf("\nExcluir usuário\n\n");

    // busca o usuário utilizando a função que dá opção de buscar
    // por id ou parte do nome
    posicao = buscar_usuario(&usuarioexc);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o nome ou id informado.\n");
    }
    else
    {
        printf("\nDados do usuário que será removido\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_usuario(usuarioexc);
        printf("------------------------------------------------------------------\n\n");

        do
        {
            printf("Confirma a exclusão? 1) Sim   2) Não: ");
            scanf("%d", &resp);
            getchar(); // limpa buffer
        } while (resp!=1 && resp!=2); // só sai do loop se a resposta for válida

        if (resp==1)
        {
            // zera as informações sobre o usuário
            usuarioexc.id = 0;
            strcpy(usuarioexc.nome,"");
            strcpy(usuarioexc.endereco,"");
            strcpy(usuarioexc.email,"");
            strcpy(usuarioexc.telefone,"");

            arq = fopen(ARQ_USUARIOS,"rb+"); // abre o arquivo
            fseek(arq, posicao-(long)sizeof(usuario), SEEK_SET); // posiciona no início do registo
            fwrite(&usuarioexc, sizeof(usuario), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo

            printf("\nRegistro excluído com sucesso!\n");
        }
        else
        {
            printf("\nExclusão cancelada!\n");
        }

    }
}


// passa por todos os registros de usuários apresentando seus dados em tela
void listar_usuarios()
{
    int qtd = 0;
    usuario usuariotemp;
    FILE* arq = fopen(ARQ_USUARIOS,"rb");

    printf("\nListagem e número de usuários\n\n");

    printf("------------------------------------------------------------------\n");
    if (arq)
    {
        while (fread(&usuariotemp, sizeof(usuario), 1, arq))
        {
            if (usuariotemp.id>0) // condição para ignorar registros removidos
            {
                mostrar_dados_usuario(usuariotemp);
                printf("------------------------------------------------------------------\n");
                qtd++;
            }
        }
        fclose(arq); // fecha o arquivo
    }
    printf("Total de usuários cadastrados: %d\n\n", qtd);
}


// função que permite consultar um determinado usuário
// e apresentar seus dados completos
void consultar_historico_usuario()
{
    int posusuario;
    usuario usuariopesq;
    printf("\nHistórico de alocações\n");

    do
    {
        printf("\n");
        posusuario = buscar_usuario(&usuariopesq);
    } while (posusuario==-1);

    mostrar_dados_completos_usuario(usuariopesq);
}


// função usada para "limpar" possíveis
// dados acima do limite do tamanho do membro das estruturas
void restringir_dados_livro(livro *l)
{
      l->titulo[TAM_TITULO-1] = '\0';
      l->autor[TAM_NOME-1] = '\0';
}

// Pede os dados do livro via teclado e retorna
// uma variável com estes dados lidos
livro obter_dados_livro()
{
    livro novo;

    printf("Título: ");
    gets(novo.titulo);

    printf("Autor: ");
    gets(novo.autor);

    do
    {
        printf("Qtd. disponível: ");
        scanf("%d", &novo.qtddisponivel);
        getchar(); // limpa buffer
    } while (novo.qtddisponivel<=0);

    return novo;
}

// apresenta os dados do livro em tela
void mostrar_dados_livro(livro l)
{
    printf("#%3d  Título: %s\n", l.id, l.titulo);
    printf("      Autor: %s | Qtd: %d\n", l.autor, l.qtddisponivel);
}

// varre o arquivo de livros e retorna
// o maior ID somado de 1
int obter_novo_codigo_livro()
{
    int ultimo = 0;
    FILE* arq;
    livro livrotemp;
    arq = fopen(ARQ_LIVROS,"rb");

    if (arq)
    {
        while (fread(&livrotemp, sizeof(livro), 1, arq))
        {
            if (livrotemp.id>0) // condição para ignorar registros removidos
                if (livrotemp.id>ultimo)
                    ultimo = livrotemp.id; // guarda o maior id
        }
        fclose(arq); // fecha o arquivo
    }
    return ultimo+1;
}

// função que busca um livro e retorna a posição do cursor dentro do arquivo
// se encontrar o livro, o valor de "livropesq" será o encontrado
int buscar_livro(livro *livropesq)
{
    int tipo, idpesq=-1, posicao=-1;
    char titulopesq[TAM_TITULO]="VAZIO", titulotrunc[TAM_TITULO];
    FILE* arq;

    printf("Pesquisar livro cadastrado\n\n");

    printf("1) Pesquisar por código/id\n");
    printf("2) Pesquisar por parte do título\n");
    do
    {
        printf("Informe o tipo de pesquisa desejada: ");
        scanf("%d", &tipo);
        getchar(); // limpa buffer
    } while(tipo!=1 && tipo!=2);

    if (tipo==1)
    {
        do
        {
            printf("Informe o código/id para pesquisa: ");
            scanf("%d", &idpesq);
            getchar(); // limpa buffer
        } while (idpesq<=0);
    }
    else
    {
        do
        {
            printf("Informe parte do título para pesquisa: ");
            gets(titulopesq);
        } while (strcmp(titulopesq,"")==0);
    }

    arq = fopen(ARQ_LIVROS,"rb");
    if (arq)
    {
        while (fread(livropesq, sizeof(livro), 1, arq))
        {
            // guarda na variável "titulotrunc", a mesma quantidade de caracteres
            // colocada na variável "titulopesq" para comparar só parte do título
            strncpy(titulotrunc, livropesq->titulo, strlen(titulopesq));
            titulotrunc[strlen(titulopesq)] = '\0';  //seta o final da string, pois "strncpy" não faz automático

            // se parte do título ou o id for igual, entra no IF
            if (strcmp(titulotrunc,titulopesq)==0 || livropesq->id == idpesq) {

                posicao = ftell(arq); // guarda a posição onde o registro foi encontrado

                break; // sai do loop
            }
        }
        fclose(arq); // fecha o arquivo
    }

    return posicao;
}

// função que faz a inclusão de um novo livro no sistema
void incluir_livro()
{
    livro novo;
    FILE* arq;

    printf("\nIncluir novo livro\n\n");
    novo = obter_dados_livro();
    novo.id = obter_novo_codigo_livro();

    arq = fopen(ARQ_LIVROS,"ab+");

    if (arq!=NULL)
    {
        if (!fwrite(&novo, sizeof(livro), 1, arq))
            printf("\nErro ao adicionar registro! E01");

        printf("\nRegistro incluido com sucesso!");
    }
    else
    {
        printf("\nErro ao adicionar registro! E02");
    }

    fclose(arq); // fecha o arquivo
}

// função que faz a alteração de um livro no sistema
void alterar_livro()
{
    int posicao;
    livro livropesq, livroalt;
    FILE* arq;

    printf("\nAlterar livro\n\n");

    posicao = buscar_livro(&livropesq);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o título ou id informado.\n");
    }
    else
    {
        printf("\nDados cadastrados atualmente\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_livro(livropesq);
        printf("------------------------------------------------------------------\n\n");

        printf("Informe os novos dados\n\n");
        livroalt = obter_dados_livro(); // pede para o usuário digitar os dados novamente
        livroalt.id = livropesq.id; // mantém o mesmo ID

        arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
        fseek(arq, posicao-(long)sizeof(livro), SEEK_SET); // posiciona no início do registo
        fwrite(&livroalt, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
        fclose(arq); // fecha o arquivo

        printf("\nRegistro alterado com sucesso!\n");
    }
}

// função que faz a exclusão de um livro no sistema
void excluir_livro()
{
    int posicao, resp;
    livro livroexc;
    FILE* arq;

    printf("\nExcluir livro\n\n");

    posicao = buscar_livro(&livroexc);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o título ou id informado.\n");
    }
    else
    {
        printf("\nDados do livro que será removido\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_livro(livroexc);
        printf("------------------------------------------------------------------\n\n");

        do
        {
            printf("Confirma a exclusão? 1) Sim   2) Não: ");
            scanf("%d", &resp);
            getchar(); // limpa buffer
        } while (resp!=1 && resp!=2);

        if (resp==1)
        {
            // zera as informações sobre o livro
            livroexc.id = 0;
            livroexc.qtddisponivel = 0;
            strcpy(livroexc.titulo,"");
            strcpy(livroexc.autor,"");

            arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
            fseek(arq, posicao-(long)sizeof(livro), SEEK_SET); // posiciona no início do registo
            fwrite(&livroexc, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo

            printf("\nRegistro excluído com sucesso!\n");
        }
        else
        {
            printf("\nExclusão cancelada!\n");
        }

    }
}

// varre o arquivo e conta o total de livros cadastrados
int contar_livros_cadastrados()
{
    int qtdlivros = 0;
    FILE* arq;
    livro l;

    arq = fopen(ARQ_LIVROS,"rb");
    if (arq)
    {
        while (fread(&l, sizeof(livro), 1, arq))
            if (l.id>0) // condição para ignorar registros removidos
                qtdlivros++;
        fclose(arq); // fecha o arquivo
    }

    return qtdlivros;
}

// lista todos os livros em tela, independente da situação
void listar_livros()
{
    int qtd = 0;
    livro livrotemp;
    FILE* arq = fopen(ARQ_LIVROS,"rb");

    printf("\nListagem e número de livros\n\n");

    printf("------------------------------------------------------------------\n");
    if (arq)
    {
        while (fread(&livrotemp, sizeof(livro), 1, arq))
        {
            if (livrotemp.id>0) // condição para ignorar registros removidos
            {
                mostrar_dados_livro(livrotemp);
                printf("------------------------------------------------------------------\n");
                qtd++;
            }
        }
        fclose(arq); // fecha o arquivo
    }
    printf("Total de livros cadastrados: %d\n\n", qtd);

}


// função que busca uma alocação e retorna a posição do cursor dentro do arquivo
// se encontrar o usuário+livro, o valor de "alocacaopesq" será o encontrado
int buscar_alocacao(alocacao *alocacaopesq, int idusuario, int idlivro, char *situacao)
{
    int posicao=-1;
    FILE* arq;

    arq = fopen(ARQ_ALOCACOES,"rb");

    if (arq)
    {
        while (fread(alocacaopesq, sizeof(alocacao), 1, arq))
        {
            if (alocacaopesq->idusuario == idusuario && alocacaopesq->idlivro == idlivro) {

                // considera uma situação passada ou ignora a situação se for vazio
                if (strcmp(alocacaopesq->situacao, situacao)==0 || strcmp(situacao, "")==0)
                {
                    posicao = ftell(arq); // guarda a posição onde o registro foi encontrado
                    break; // sai do loop
                }
            }
        }
        fclose(arq); // fecha o arquivo
    }

    return posicao;
}

// função que permite pesquisar um usuário, um livro e registra a alocação do mesmo
// a função faz o controle da quantidade de livros disponíveis
void alocar()
{
    int posusuario, poslivro, posalocacao, resp;
    livro livropesq;
    usuario usuariopesq;
    alocacao nova, alocacaopesq;
    FILE* arq;

    printf("\nAlocação de livro\n");

    // busca um usuário até encontrar
    do
    {
        printf("\n");
        posusuario = buscar_usuario(&usuariopesq);
    } while (posusuario==-1);

    // busca um livro até encontrar
    do
    {
        printf("\n");
        poslivro = buscar_livro(&livropesq);
    } while (poslivro==-1);


    // verifica se o livro encontrado ainda tem disponibilidade
    if (livropesq.qtddisponivel>0)
    {

        // apresenta em tela os dados do usuário e do livro para confirmação

        printf("\n");
        printf("Usuário selecionado\n");
        printf("-------------------\n");
        mostrar_dados_usuario(usuariopesq);

        printf("\n");

        printf("Livro selecionado\n");
        printf("-------------------\n");
        mostrar_dados_livro(livropesq);

        printf("\n");

        // busca se o usuário já tem um alocação deste livro PENDENTE
        posalocacao = buscar_alocacao(&alocacaopesq, usuariopesq.id, livropesq.id, "PENDENTE");

        if (posalocacao>-1)
        {
            printf("\nLivro já está alocado para este usuário!\n");
        }
        else
        {

            // obtém confirmação do usuário até ele responder de forma válida
            do
            {
                printf("Confirma a alocação? 1) Sim   2) Não: ");
                scanf("%d", &resp);
                getchar(); // limpa buffer
            } while (resp!=1 && resp!=2);


            // se o usuário confirmar
            if (resp==1)
            {

                // guarda os dados da nova alocação
                nova.idlivro = livropesq.id;
                nova.idusuario = usuariopesq.id;

                // obtém da data prevista para devolução
                printf("Informe a data prevista de devolução (dd/mm/aaaa): ");
                gets(nova.dtprevista);

                // formata para guardar no arquivo na ordem aaaammdd
                formatar_data(nova.dtprevista, 1);

                // informa a situação como pendente e data de devolução como vazia
                strcpy(nova.situacao,"PENDENTE");
                strcpy(nova.dtdevolucao,"");

                // subtrai a quantidade de livros disponíveis
                livropesq.qtddisponivel--;

                // ------------------------------------------------------
                // atualiza a quantidade disponível do livro
                arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
                fseek(arq, poslivro-(long)sizeof(livro), SEEK_SET); // posiciona no início do registo
                fwrite(&livropesq, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
                fclose(arq); // fecha o arquivo
                // ------------------------------------------------------

                // ------------------------------------------------------
                // grava o registro de empréstimo
                arq = fopen(ARQ_ALOCACOES,"ab+"); // abre o arquivo
                fwrite(&nova, sizeof(alocacao), 1, arq);
                fclose(arq); // fecha o arquivo
                // ------------------------------------------------------

                printf("Alocação registrada com sucesso!\n");

            }
            else
            {
                printf("\nAção cancelada!\n");
            }
        }
    }
    else
    {
        printf("\nLivro não possui exemplares para alocação!\n");
    }
}


// função que permite pesquisar um usuário, um livro e registra a devolução do mesmo
// a função faz o controle da quantida de livros disponíveis
void devolver()
{
    int posusuario, poslivro, posalocacao, resp;
    livro livropesq;
    usuario usuariopesq;
    alocacao alocacaodev;
    FILE* arq;

    printf("\nDevolução de livro\n");

    // busca um usuário até encontrar
    do
    {
        printf("\n");
        posusuario = buscar_usuario(&usuariopesq);
    } while (posusuario==-1);

    // busca um livro até encontrar
    do
    {
        printf("\n");
        poslivro = buscar_livro(&livropesq);
    } while (poslivro==-1);

    // verifica se este livro está alocado para o usuário
    posalocacao = buscar_alocacao(&alocacaodev, usuariopesq.id, livropesq.id, "PENDENTE");


    if (posalocacao>-1)
    {
        // mostra os dados do usuário, do livro e da alocação para confirmação

        printf("\n");
        printf("Usuário selecionado\n");
        printf("-------------------\n");
        mostrar_dados_usuario(usuariopesq);

        printf("\n");

        printf("Livro selecionado\n");
        printf("-------------------\n");
        mostrar_dados_livro(livropesq);

        printf("\n");

        printf("Alocação selecionada\n");
        printf("-------------------\n");
        mostrar_dados_alocacao(alocacaodev);

        printf("\n");

        // pede confirmação ao usuário até uma resposta válida
        do
        {
            printf("Confirma a devolução? 1) Sim   2) Não: ");
            scanf("%d", &resp);
            getchar(); // limpa buffer
        } while (resp!=1 && resp!=2);

        // se confirmar a devolução
        if (resp==1)
        {
            // obtém a data de devolução
            printf("Informe a data de devolução (dd/mm/aaaa): ");
            gets(alocacaodev.dtdevolucao);

            // formata a data para gravar no formato aaaammdd
            formatar_data(alocacaodev.dtdevolucao, 1);

            // informa a situação como DEVOLVIDO
            strcpy(alocacaodev.situacao,"DEVOLVIDO");

            // soma a quantidade de livros disponíveis
            livropesq.qtddisponivel++;

            // ------------------------------------------------------
            // atualiza a quantidade disponível do livro
            arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
            fseek(arq, poslivro-(long)sizeof(livro), SEEK_SET); // posiciona no início do registo
            fwrite(&livropesq, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo
            // ------------------------------------------------------


            // ------------------------------------------------------
            // atualiza o registro de empréstimo
            arq = fopen(ARQ_ALOCACOES,"rb+"); // abre o arquivo
            fseek(arq, posalocacao-(long)sizeof(alocacao), SEEK_SET); // posiciona no início do registo
            fwrite(&alocacaodev, sizeof(alocacao), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo
            // ------------------------------------------------------

            printf("Devolução registrada com sucesso!\n");

        }
        else
        {
            printf("\nAção cancelada!\n");
        }
    }
    else
    {
        printf("\nLivro não possui alocação para este usuário!\n");
    }
}

// lista todas as alocações independente da situação
void listar_alocacoes()
{
    int qtd = 0;
    alocacao alocacaotemp;
    FILE* arq = fopen(ARQ_ALOCACOES,"rb");

    printf("\nListagem e número de alocações\n\n");

    printf("------------------------------------------------------------------\n");
    if (arq)
    {
        while (fread(&alocacaotemp, sizeof(alocacao), 1, arq))
        {
            mostrar_dados_alocacao(alocacaotemp);
            printf("------------------------------------------------------------------\n");
            qtd++;
        }
        fclose(arq); // fecha o arquivo
    }
    printf("Total de alocações cadastradas: %d\n\n", qtd);
}

// conta o total de alocações com situação PENDENTE
int contar_alocacoes_pendentes()
{
    int qtdtotalpendentes = 0;
    FILE* arq;
    alocacao a;

    arq = fopen(ARQ_ALOCACOES,"rb");
    if (arq)
    {
        while (fread(&a, sizeof(alocacao), 1, arq))
            if(strcmp(a.situacao,"PENDENTE")==0)
                qtdtotalpendentes++;
        fclose(arq); // fecha o arquivo
    }

    return qtdtotalpendentes;
}

// conta o total de alocações independente da situação
int contar_alocacoes_totais()
{
    int qtdtotalalocados = 0;
    FILE* arq;
    alocacao a;

    arq = fopen(ARQ_ALOCACOES,"rb");
    if (arq)
    {
        while (fread(&a, sizeof(alocacao), 1, arq))
            qtdtotalalocados++;
        fclose(arq); // fecha o arquivo
    }

    return qtdtotalalocados;
}


// lista todas as alocações pendentes de forma ordenada
// ordem = 1 crescente
// ordem = 2 decrescente
void listar_alocacoes_pendentes_ordenadas(int ordem)
{
    int qtd, i=0;
    alocacao *alocacoes, alocacaotemp;
    FILE* arq;

    qtd = contar_alocacoes_pendentes();

    printf("\nListagem das alocações pendentes por ordem %s de previsão de devolução\n", ordem==1?"crescente":"decrescente");
    printf("------------------------------------------------------------------\n");


    if (qtd>0)
    {
        // aloca memória para a quantidade de empréstimos pendentes
        alocacoes = malloc(sizeof(alocacao)*qtd);

        // insere os dados do arquivo no vetor
        FILE* arq = fopen(ARQ_ALOCACOES,"rb");
        if (arq)
        {
            while (fread(&alocacaotemp, sizeof(alocacao), 1, arq))
            {
                if (strcmp(alocacaotemp.situacao, "PENDENTE")==0)
                {
                    alocacoes[i] = alocacaotemp;
                    i++;
                }
            }
            fclose(arq); // fecha o arquivo
        }

        // orderna o vetor
        ordernar_alocacoes(alocacoes, qtd, ordem);

        // apresenta os dados ordenados
        for(i=0;i<qtd;i++)
        {
            mostrar_dados_alocacao(alocacoes[i]);
            printf("------------------------------------------------------------------\n");
        }

        // libera a memória
        free(alocacoes);
    }

    printf("Total de alocações pendentes cadastradas: %d\n\n", qtd);
}


// menu usado para para dar as opções para o usuário 
int menu()
{
    int resp;

    printf("\nUSUÁRIOS\n");
    printf("1) Incluir\n");
    printf("2) Alterar\n");
    printf("3) Excluir\n");
    printf("4) Listar todos\n\n");

    printf("LIVROS\n");
    printf("5) Incluir\n");
    printf("6) Alterar\n");
    printf("7) Excluir\n");
    printf("8) Listar todos\n\n");

    printf("ALOCAÇÕES\n");
    printf("9) Alocar\n");
    printf("10) Devolver\n");
    printf("11) Listar todas\n\n");

    printf("CONSULTAS\n");
    printf("12) Mostrar estatísticas\n");
    printf("13) Livros alocados por data prev. de devolução crescente\n");
    printf("14) Livros alocados por data prev. de devolução decrescente\n");
    printf("15) Histórico de alocações de um usuário\n\n");

    printf("16) Sair\n");

    printf("\nDigite o número correspondente à opção desejada: ");
    scanf("%d", &resp);

    getchar(); // limpa buffer

    return resp;
}


// utiliza funções que fazem as contagens solicitadas
void mostrar_estatisticas()
{
    int qtdusuarios = 0;
    int qtdlivros = 0;
    int qtdtotalalocados = 0;
    int qtdtotalpendentes = 0;

    qtdlivros = contar_livros_cadastrados();
    qtdusuarios = contar_usuarios_cadastrados();
    qtdtotalpendentes = contar_alocacoes_pendentes();
    qtdtotalalocados = contar_alocacoes_totais();

    printf("\nEstatísticas\n\n");
    printf("Total de usuários cadastrados: %d\n", qtdusuarios);
    printf("Total de livros cadastrados: %d\n", qtdlivros);
    printf("Total de alocações realizadas: %d\n", qtdtotalalocados);
    printf("Total de alocações pendentes de devolução: %d\n", qtdtotalpendentes);

}


/* função principal do programa:
 é chamado a função do menu e a partir do seu retorno, utiliza um switch para chamar
 as funções que o sistema possui */
int main()
{
    setlocale(LC_ALL, "");
    int opcao = 0;

    while(opcao!=16)
    {
        printf("---------> SISTEMA DE CONTROLE DE BIBLIOTECA <----------\n");
        opcao = menu();

        switch(opcao)
        {
            case 1:
                incluir_usuario();
                break;
            case 2:
                alterar_usuario();
                break;
            case 3:
                excluir_usuario();
                break;
            case 4:
                listar_usuarios();
                break;

            case 5:
                incluir_livro();
                break;
            case 6:
                alterar_livro();
                break;
            case 7:
                excluir_livro();
                break;
            case 8:
                listar_livros();
                break;

            case 9:
                alocar();
                break;
            case 10:
                devolver();
                break;
            case 11:
                listar_alocacoes();
                break;

            case 12:
                mostrar_estatisticas();
                break;
            case 13:
                listar_alocacoes_pendentes_ordenadas(1);
                break;
            case 14:
                listar_alocacoes_pendentes_ordenadas(2);
                break;
            case 15:
                consultar_historico_usuario();
                break;

            case 16:
                // sai do programa
                break;

            default:
                printf("Opção inválida!\n");
                break;
        }

        if (opcao!=16)
        {
            printf("\n\nPressione ENTER para continuar...\n");
            getchar();
        }
    }

    return 0;
}
