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

// conta o total de aloca��es independente da situa��o por usu�rio
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

// busca no arquivo de aloca��es pelo ID do usu�rio e preenche
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

// fun��o usada para "limpar" poss�veis
// dados acima do limite do tamanho do membro das estruturas
void restringir_dados_usuario(usuario *u)
{
      u->nome[TAM_NOME-1] = '\0';
      u->endereco[TAM_ENDERECO-1] = '\0';
      u->email[TAM_EMAIL-1] = '\0';
      u->telefone[TAM_TELEFONE-1] = '\0';
}

// varre o arquivo de usu�rios, contando quantos cadastros tem
int contar_usuarios_cadastrados()
{
    int qtdusuarios = 0;
    FILE* arq;
    usuario u;

    arq = fopen(ARQ_USUARIOS,"rb");
    if (arq)
    {
        while (fread(&u, sizeof(usuario), 1, arq))
            if (u.id>0) // condi��o para ignorar registros removidos
                qtdusuarios++;
        fclose(arq); // fecha o arquivo
    }

    return qtdusuarios;
}

// Pede os dados do usu�rio via teclado e retorna
// uma vari�vel com estes dados lidos
usuario obter_dados_usuario()
{
    usuario novo;

    printf("Nome: ");
    gets(novo.nome);

    printf("Endere�o: ");
    gets(novo.endereco);

    printf("E-mail: ");
    gets(novo.email);

    printf("Telefone: ");
    gets(novo.telefone);

    return novo;
}

// recebe um usu�rio e apresenta seus dados em tela
void mostrar_dados_usuario(usuario u)
{
    printf("#%3d   Nome: %s | Endere�o: %s\n", u.id, u.nome, u.endereco);
    printf("       E-mail: %s | Telefone: %s\n", u.email, u.telefone);
}

// fun��o usada para formatar as datas previstas e de devolu��o
void formatar_data(char *dt, int forma)
{
    char ndt[11];

    if(strlen(dt)>0) // ignora datas vazias
    {
        strcpy(ndt, dt); // faz uma c�pia da data

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


// apresenta os dados da aloca��o em tela
void mostrar_dados_alocacao(alocacao a)
{
    formatar_data(a.dtprevista, 2);
    formatar_data(a.dtdevolucao, 2);

    printf("Usu�rio: #%d\tLivro: #%d\tSitua��o: %s\n", a.idusuario, a.idlivro, a.situacao);
    printf("Dt. prevista: %s\tDt. devolu��o: %s\n", a.dtprevista, a.dtdevolucao);
}


// ordena��o do vetor de alocacoes utilizando o algoritmo selection sort
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


// recebe um usu�rio e, al�m de apresentar seus dados em tela,
// tamb�m consulta os livros j� alocados por ele e mostra e tela
void mostrar_dados_completos_usuario(usuario u)
{
    int i, qtd;
    livro l;
    printf("\n\nUsu�rio selecionado:\n");
    printf("------------------------------------------------------------------\n");

    mostrar_dados_usuario(u); // mostra os dados utilizando fun��o anterior

    // conta as aloca��es de um determinado usu�rio
    qtd = contar_alocacoes_por_usuario(u.id);

    if (qtd>0)
    {

        // aloca mem�ria para guardar as aloca��es
        u.livros = malloc(sizeof(alocacao)*qtd);

        // faz a busca
        buscar_alocacoes_por_usuario(u.id, u.livros);

        printf("Hist�rico de aloca��es de livros em ordem decrescente de\n");
        printf("data prevista para devolu��o, incluindo os devolvidos.\n");
        printf("------------------------------------------------------------------\n");

        if (u.livros!=NULL)
        {
            // ordena os dados antes de apresentar
            ordernar_alocacoes(u.livros, qtd, 2);

            for(i=0; i<qtd; i++)
            {
                // apresenta utlizando a fun��o que mostra os dados da aloca��o
                mostrar_dados_alocacao(u.livros[i]);
                printf("------------------------------------------------------------------\n");
            }
        }

        // libera a mem�ria usada
        free(u.livros);
    }
}

// varre o arquivo de usu�rios e retorna
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
            if (usuariotemp.id>0) // condi��o para ignorar registros removidos
                if (usuariotemp.id>ultimo)
                    ultimo = usuariotemp.id; // guarda o maior encontrado
        }
        fclose(arq); // fecha o arquivo
    }
    return ultimo+1;
}

// func�o que busca um usu�rio e retorna a posi��o do cursor dentro do arquivo
// se encontrar o usu�rio, o valor de "usuariopesq" ser� o encontrado
int buscar_usuario(usuario *usuariopesq)
{
    int tipo, idpesq=-1, posicao=-1;
    char nomepesq[TAM_NOME]="VAZIO", nometrunc[TAM_NOME];
    FILE* arq;

    printf("Pesquisar usu�rio cadastrado\n\n");

    printf("1) Pesquisar por c�digo/id\n");
    printf("2) Pesquisar por parte do nome\n");
    do
    {
        printf("Informe o tipo de pesquisa desejada: ");
        scanf("%d", &tipo);
        getchar(); // limpa buffer
    } while(tipo!=1 && tipo!=2); // s� sai do loop se o tipo for v�lido

    if (tipo==1)
    {
        do
        {
            printf("Informe o c�digo/id para pesquisa: ");
            scanf("%d", &idpesq);
            getchar(); // limpa buffer
        } while (idpesq<=0);  // s� sai do loop se o id for v�lido
    }
    else
    {
        do
        {
            printf("Informe parte do nome para pesquisa: ");
            gets(nomepesq);
        } while (strcmp(nomepesq,"")==0);  // s� sai do loop se informar parte do nome
    }

    arq = fopen(ARQ_USUARIOS,"rb");

    if (arq)
    {
        while (fread(usuariopesq, sizeof(usuario), 1, arq))
        {
            // guarda na vari�vel "nometrunc", a mesma quantidade de caracteres
            // colocada na vari�vel "nomepesq" para comparar s� parte do nome
            strncpy(nometrunc, usuariopesq->nome, strlen(nomepesq));
            nometrunc[strlen(nomepesq)] = '\0';  //seta o final da string, pois "strncpy" n�o faz autom�tico

            // se parte do nome ou o id for igual, entra no IF
            if (strcmp(nometrunc,nomepesq)==0 || usuariopesq->id == idpesq) {

                posicao = ftell(arq); // guarda a posi��o onde o registro foi encontrado

                break; // sai do loop
            }
        }
        fclose(arq); // fecha o arquivo
    }

    return posicao;
}


// fun��o que faz a inclus�o de um novo usu�rio no sistema
void incluir_usuario()
{
    usuario novo;
    FILE* arq;

    printf("\nIncluir novo usu�rio\n\n");

    // obt�m os dados a partir da digita��o
    novo = obter_dados_usuario();

    // busca o novo id no arquivo
    novo.id = obter_novo_codigo_usuario();

    // abre o arquivo para adicionar um registro
    arq = fopen(ARQ_USUARIOS,"ab+");

    if (arq!=NULL)
    {
        // faz a grava��o dos dados
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


// fun��o que faz a altera��o de um usu�rio no sistema
void alterar_usuario()
{
    int posicao;
    usuario usuariopesq, usuarioalt;
    FILE* arq;

    printf("\nAlterar usu�rio\n\n");

    // busca o usu�rio utilizando a fun��o que d� op��o de buscar
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
        usuarioalt = obter_dados_usuario(); // pede para o usu�rio digitar os dados novamente
        usuarioalt.id = usuariopesq.id; // mant�m o mesmo ID do usu�rio

        arq = fopen(ARQ_USUARIOS,"rb+"); // abre o arquivo
        fseek(arq, posicao-(long)sizeof(usuario), SEEK_SET); // posiciona no in�cio do registo
        fwrite(&usuarioalt, sizeof(usuario), 1, arq); // grava o registro, sobrescrevendo o anterior
        fclose(arq); // fecha o arquivo

        printf("\nRegistro alterado com sucesso!\n");
    }
}


// fun��o que faz a exclus�o de um usu�rio no sistema
void excluir_usuario()
{
    int posicao, resp;
    usuario usuarioexc;
    FILE* arq;

    printf("\nExcluir usu�rio\n\n");

    // busca o usu�rio utilizando a fun��o que d� op��o de buscar
    // por id ou parte do nome
    posicao = buscar_usuario(&usuarioexc);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o nome ou id informado.\n");
    }
    else
    {
        printf("\nDados do usu�rio que ser� removido\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_usuario(usuarioexc);
        printf("------------------------------------------------------------------\n\n");

        do
        {
            printf("Confirma a exclus�o? 1) Sim   2) N�o: ");
            scanf("%d", &resp);
            getchar(); // limpa buffer
        } while (resp!=1 && resp!=2); // s� sai do loop se a resposta for v�lida

        if (resp==1)
        {
            // zera as informa��es sobre o usu�rio
            usuarioexc.id = 0;
            strcpy(usuarioexc.nome,"");
            strcpy(usuarioexc.endereco,"");
            strcpy(usuarioexc.email,"");
            strcpy(usuarioexc.telefone,"");

            arq = fopen(ARQ_USUARIOS,"rb+"); // abre o arquivo
            fseek(arq, posicao-(long)sizeof(usuario), SEEK_SET); // posiciona no in�cio do registo
            fwrite(&usuarioexc, sizeof(usuario), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo

            printf("\nRegistro exclu�do com sucesso!\n");
        }
        else
        {
            printf("\nExclus�o cancelada!\n");
        }

    }
}


// passa por todos os registros de usu�rios apresentando seus dados em tela
void listar_usuarios()
{
    int qtd = 0;
    usuario usuariotemp;
    FILE* arq = fopen(ARQ_USUARIOS,"rb");

    printf("\nListagem e n�mero de usu�rios\n\n");

    printf("------------------------------------------------------------------\n");
    if (arq)
    {
        while (fread(&usuariotemp, sizeof(usuario), 1, arq))
        {
            if (usuariotemp.id>0) // condi��o para ignorar registros removidos
            {
                mostrar_dados_usuario(usuariotemp);
                printf("------------------------------------------------------------------\n");
                qtd++;
            }
        }
        fclose(arq); // fecha o arquivo
    }
    printf("Total de usu�rios cadastrados: %d\n\n", qtd);
}


// fun��o que permite consultar um determinado usu�rio
// e apresentar seus dados completos
void consultar_historico_usuario()
{
    int posusuario;
    usuario usuariopesq;
    printf("\nHist�rico de aloca��es\n");

    do
    {
        printf("\n");
        posusuario = buscar_usuario(&usuariopesq);
    } while (posusuario==-1);

    mostrar_dados_completos_usuario(usuariopesq);
}


// fun��o usada para "limpar" poss�veis
// dados acima do limite do tamanho do membro das estruturas
void restringir_dados_livro(livro *l)
{
      l->titulo[TAM_TITULO-1] = '\0';
      l->autor[TAM_NOME-1] = '\0';
}

// Pede os dados do livro via teclado e retorna
// uma vari�vel com estes dados lidos
livro obter_dados_livro()
{
    livro novo;

    printf("T�tulo: ");
    gets(novo.titulo);

    printf("Autor: ");
    gets(novo.autor);

    do
    {
        printf("Qtd. dispon�vel: ");
        scanf("%d", &novo.qtddisponivel);
        getchar(); // limpa buffer
    } while (novo.qtddisponivel<=0);

    return novo;
}

// apresenta os dados do livro em tela
void mostrar_dados_livro(livro l)
{
    printf("#%3d  T�tulo: %s\n", l.id, l.titulo);
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
            if (livrotemp.id>0) // condi��o para ignorar registros removidos
                if (livrotemp.id>ultimo)
                    ultimo = livrotemp.id; // guarda o maior id
        }
        fclose(arq); // fecha o arquivo
    }
    return ultimo+1;
}

// fun��o que busca um livro e retorna a posi��o do cursor dentro do arquivo
// se encontrar o livro, o valor de "livropesq" ser� o encontrado
int buscar_livro(livro *livropesq)
{
    int tipo, idpesq=-1, posicao=-1;
    char titulopesq[TAM_TITULO]="VAZIO", titulotrunc[TAM_TITULO];
    FILE* arq;

    printf("Pesquisar livro cadastrado\n\n");

    printf("1) Pesquisar por c�digo/id\n");
    printf("2) Pesquisar por parte do t�tulo\n");
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
            printf("Informe o c�digo/id para pesquisa: ");
            scanf("%d", &idpesq);
            getchar(); // limpa buffer
        } while (idpesq<=0);
    }
    else
    {
        do
        {
            printf("Informe parte do t�tulo para pesquisa: ");
            gets(titulopesq);
        } while (strcmp(titulopesq,"")==0);
    }

    arq = fopen(ARQ_LIVROS,"rb");
    if (arq)
    {
        while (fread(livropesq, sizeof(livro), 1, arq))
        {
            // guarda na vari�vel "titulotrunc", a mesma quantidade de caracteres
            // colocada na vari�vel "titulopesq" para comparar s� parte do t�tulo
            strncpy(titulotrunc, livropesq->titulo, strlen(titulopesq));
            titulotrunc[strlen(titulopesq)] = '\0';  //seta o final da string, pois "strncpy" n�o faz autom�tico

            // se parte do t�tulo ou o id for igual, entra no IF
            if (strcmp(titulotrunc,titulopesq)==0 || livropesq->id == idpesq) {

                posicao = ftell(arq); // guarda a posi��o onde o registro foi encontrado

                break; // sai do loop
            }
        }
        fclose(arq); // fecha o arquivo
    }

    return posicao;
}

// fun��o que faz a inclus�o de um novo livro no sistema
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

// fun��o que faz a altera��o de um livro no sistema
void alterar_livro()
{
    int posicao;
    livro livropesq, livroalt;
    FILE* arq;

    printf("\nAlterar livro\n\n");

    posicao = buscar_livro(&livropesq);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o t�tulo ou id informado.\n");
    }
    else
    {
        printf("\nDados cadastrados atualmente\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_livro(livropesq);
        printf("------------------------------------------------------------------\n\n");

        printf("Informe os novos dados\n\n");
        livroalt = obter_dados_livro(); // pede para o usu�rio digitar os dados novamente
        livroalt.id = livropesq.id; // mant�m o mesmo ID

        arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
        fseek(arq, posicao-(long)sizeof(livro), SEEK_SET); // posiciona no in�cio do registo
        fwrite(&livroalt, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
        fclose(arq); // fecha o arquivo

        printf("\nRegistro alterado com sucesso!\n");
    }
}

// fun��o que faz a exclus�o de um livro no sistema
void excluir_livro()
{
    int posicao, resp;
    livro livroexc;
    FILE* arq;

    printf("\nExcluir livro\n\n");

    posicao = buscar_livro(&livroexc);

    if (posicao==-1)
    {
        printf("Nenhum registro encontrado com o t�tulo ou id informado.\n");
    }
    else
    {
        printf("\nDados do livro que ser� removido\n");
        printf("------------------------------------------------------------------\n");
        mostrar_dados_livro(livroexc);
        printf("------------------------------------------------------------------\n\n");

        do
        {
            printf("Confirma a exclus�o? 1) Sim   2) N�o: ");
            scanf("%d", &resp);
            getchar(); // limpa buffer
        } while (resp!=1 && resp!=2);

        if (resp==1)
        {
            // zera as informa��es sobre o livro
            livroexc.id = 0;
            livroexc.qtddisponivel = 0;
            strcpy(livroexc.titulo,"");
            strcpy(livroexc.autor,"");

            arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
            fseek(arq, posicao-(long)sizeof(livro), SEEK_SET); // posiciona no in�cio do registo
            fwrite(&livroexc, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo

            printf("\nRegistro exclu�do com sucesso!\n");
        }
        else
        {
            printf("\nExclus�o cancelada!\n");
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
            if (l.id>0) // condi��o para ignorar registros removidos
                qtdlivros++;
        fclose(arq); // fecha o arquivo
    }

    return qtdlivros;
}

// lista todos os livros em tela, independente da situa��o
void listar_livros()
{
    int qtd = 0;
    livro livrotemp;
    FILE* arq = fopen(ARQ_LIVROS,"rb");

    printf("\nListagem e n�mero de livros\n\n");

    printf("------------------------------------------------------------------\n");
    if (arq)
    {
        while (fread(&livrotemp, sizeof(livro), 1, arq))
        {
            if (livrotemp.id>0) // condi��o para ignorar registros removidos
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


// fun��o que busca uma aloca��o e retorna a posi��o do cursor dentro do arquivo
// se encontrar o usu�rio+livro, o valor de "alocacaopesq" ser� o encontrado
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

                // considera uma situa��o passada ou ignora a situa��o se for vazio
                if (strcmp(alocacaopesq->situacao, situacao)==0 || strcmp(situacao, "")==0)
                {
                    posicao = ftell(arq); // guarda a posi��o onde o registro foi encontrado
                    break; // sai do loop
                }
            }
        }
        fclose(arq); // fecha o arquivo
    }

    return posicao;
}

// fun��o que permite pesquisar um usu�rio, um livro e registra a aloca��o do mesmo
// a fun��o faz o controle da quantidade de livros dispon�veis
void alocar()
{
    int posusuario, poslivro, posalocacao, resp;
    livro livropesq;
    usuario usuariopesq;
    alocacao nova, alocacaopesq;
    FILE* arq;

    printf("\nAloca��o de livro\n");

    // busca um usu�rio at� encontrar
    do
    {
        printf("\n");
        posusuario = buscar_usuario(&usuariopesq);
    } while (posusuario==-1);

    // busca um livro at� encontrar
    do
    {
        printf("\n");
        poslivro = buscar_livro(&livropesq);
    } while (poslivro==-1);


    // verifica se o livro encontrado ainda tem disponibilidade
    if (livropesq.qtddisponivel>0)
    {

        // apresenta em tela os dados do usu�rio e do livro para confirma��o

        printf("\n");
        printf("Usu�rio selecionado\n");
        printf("-------------------\n");
        mostrar_dados_usuario(usuariopesq);

        printf("\n");

        printf("Livro selecionado\n");
        printf("-------------------\n");
        mostrar_dados_livro(livropesq);

        printf("\n");

        // busca se o usu�rio j� tem um aloca��o deste livro PENDENTE
        posalocacao = buscar_alocacao(&alocacaopesq, usuariopesq.id, livropesq.id, "PENDENTE");

        if (posalocacao>-1)
        {
            printf("\nLivro j� est� alocado para este usu�rio!\n");
        }
        else
        {

            // obt�m confirma��o do usu�rio at� ele responder de forma v�lida
            do
            {
                printf("Confirma a aloca��o? 1) Sim   2) N�o: ");
                scanf("%d", &resp);
                getchar(); // limpa buffer
            } while (resp!=1 && resp!=2);


            // se o usu�rio confirmar
            if (resp==1)
            {

                // guarda os dados da nova aloca��o
                nova.idlivro = livropesq.id;
                nova.idusuario = usuariopesq.id;

                // obt�m da data prevista para devolu��o
                printf("Informe a data prevista de devolu��o (dd/mm/aaaa): ");
                gets(nova.dtprevista);

                // formata para guardar no arquivo na ordem aaaammdd
                formatar_data(nova.dtprevista, 1);

                // informa a situa��o como pendente e data de devolu��o como vazia
                strcpy(nova.situacao,"PENDENTE");
                strcpy(nova.dtdevolucao,"");

                // subtrai a quantidade de livros dispon�veis
                livropesq.qtddisponivel--;

                // ------------------------------------------------------
                // atualiza a quantidade dispon�vel do livro
                arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
                fseek(arq, poslivro-(long)sizeof(livro), SEEK_SET); // posiciona no in�cio do registo
                fwrite(&livropesq, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
                fclose(arq); // fecha o arquivo
                // ------------------------------------------------------

                // ------------------------------------------------------
                // grava o registro de empr�stimo
                arq = fopen(ARQ_ALOCACOES,"ab+"); // abre o arquivo
                fwrite(&nova, sizeof(alocacao), 1, arq);
                fclose(arq); // fecha o arquivo
                // ------------------------------------------------------

                printf("Aloca��o registrada com sucesso!\n");

            }
            else
            {
                printf("\nA��o cancelada!\n");
            }
        }
    }
    else
    {
        printf("\nLivro n�o possui exemplares para aloca��o!\n");
    }
}


// fun��o que permite pesquisar um usu�rio, um livro e registra a devolu��o do mesmo
// a fun��o faz o controle da quantida de livros dispon�veis
void devolver()
{
    int posusuario, poslivro, posalocacao, resp;
    livro livropesq;
    usuario usuariopesq;
    alocacao alocacaodev;
    FILE* arq;

    printf("\nDevolu��o de livro\n");

    // busca um usu�rio at� encontrar
    do
    {
        printf("\n");
        posusuario = buscar_usuario(&usuariopesq);
    } while (posusuario==-1);

    // busca um livro at� encontrar
    do
    {
        printf("\n");
        poslivro = buscar_livro(&livropesq);
    } while (poslivro==-1);

    // verifica se este livro est� alocado para o usu�rio
    posalocacao = buscar_alocacao(&alocacaodev, usuariopesq.id, livropesq.id, "PENDENTE");


    if (posalocacao>-1)
    {
        // mostra os dados do usu�rio, do livro e da aloca��o para confirma��o

        printf("\n");
        printf("Usu�rio selecionado\n");
        printf("-------------------\n");
        mostrar_dados_usuario(usuariopesq);

        printf("\n");

        printf("Livro selecionado\n");
        printf("-------------------\n");
        mostrar_dados_livro(livropesq);

        printf("\n");

        printf("Aloca��o selecionada\n");
        printf("-------------------\n");
        mostrar_dados_alocacao(alocacaodev);

        printf("\n");

        // pede confirma��o ao usu�rio at� uma resposta v�lida
        do
        {
            printf("Confirma a devolu��o? 1) Sim   2) N�o: ");
            scanf("%d", &resp);
            getchar(); // limpa buffer
        } while (resp!=1 && resp!=2);

        // se confirmar a devolu��o
        if (resp==1)
        {
            // obt�m a data de devolu��o
            printf("Informe a data de devolu��o (dd/mm/aaaa): ");
            gets(alocacaodev.dtdevolucao);

            // formata a data para gravar no formato aaaammdd
            formatar_data(alocacaodev.dtdevolucao, 1);

            // informa a situa��o como DEVOLVIDO
            strcpy(alocacaodev.situacao,"DEVOLVIDO");

            // soma a quantidade de livros dispon�veis
            livropesq.qtddisponivel++;

            // ------------------------------------------------------
            // atualiza a quantidade dispon�vel do livro
            arq = fopen(ARQ_LIVROS,"rb+"); // abre o arquivo
            fseek(arq, poslivro-(long)sizeof(livro), SEEK_SET); // posiciona no in�cio do registo
            fwrite(&livropesq, sizeof(livro), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo
            // ------------------------------------------------------


            // ------------------------------------------------------
            // atualiza o registro de empr�stimo
            arq = fopen(ARQ_ALOCACOES,"rb+"); // abre o arquivo
            fseek(arq, posalocacao-(long)sizeof(alocacao), SEEK_SET); // posiciona no in�cio do registo
            fwrite(&alocacaodev, sizeof(alocacao), 1, arq); // grava o registro, sobrescrevendo o anterior
            fclose(arq); // fecha o arquivo
            // ------------------------------------------------------

            printf("Devolu��o registrada com sucesso!\n");

        }
        else
        {
            printf("\nA��o cancelada!\n");
        }
    }
    else
    {
        printf("\nLivro n�o possui aloca��o para este usu�rio!\n");
    }
}

// lista todas as aloca��es independente da situa��o
void listar_alocacoes()
{
    int qtd = 0;
    alocacao alocacaotemp;
    FILE* arq = fopen(ARQ_ALOCACOES,"rb");

    printf("\nListagem e n�mero de aloca��es\n\n");

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
    printf("Total de aloca��es cadastradas: %d\n\n", qtd);
}

// conta o total de aloca��es com situa��o PENDENTE
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

// conta o total de aloca��es independente da situa��o
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


// lista todas as aloca��es pendentes de forma ordenada
// ordem = 1 crescente
// ordem = 2 decrescente
void listar_alocacoes_pendentes_ordenadas(int ordem)
{
    int qtd, i=0;
    alocacao *alocacoes, alocacaotemp;
    FILE* arq;

    qtd = contar_alocacoes_pendentes();

    printf("\nListagem das aloca��es pendentes por ordem %s de previs�o de devolu��o\n", ordem==1?"crescente":"decrescente");
    printf("------------------------------------------------------------------\n");


    if (qtd>0)
    {
        // aloca mem�ria para a quantidade de empr�stimos pendentes
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

        // libera a mem�ria
        free(alocacoes);
    }

    printf("Total de aloca��es pendentes cadastradas: %d\n\n", qtd);
}


// menu usado para para dar as op��es para o usu�rio 
int menu()
{
    int resp;

    printf("\nUSU�RIOS\n");
    printf("1) Incluir\n");
    printf("2) Alterar\n");
    printf("3) Excluir\n");
    printf("4) Listar todos\n\n");

    printf("LIVROS\n");
    printf("5) Incluir\n");
    printf("6) Alterar\n");
    printf("7) Excluir\n");
    printf("8) Listar todos\n\n");

    printf("ALOCA��ES\n");
    printf("9) Alocar\n");
    printf("10) Devolver\n");
    printf("11) Listar todas\n\n");

    printf("CONSULTAS\n");
    printf("12) Mostrar estat�sticas\n");
    printf("13) Livros alocados por data prev. de devolu��o crescente\n");
    printf("14) Livros alocados por data prev. de devolu��o decrescente\n");
    printf("15) Hist�rico de aloca��es de um usu�rio\n\n");

    printf("16) Sair\n");

    printf("\nDigite o n�mero correspondente � op��o desejada: ");
    scanf("%d", &resp);

    getchar(); // limpa buffer

    return resp;
}


// utiliza fun��es que fazem as contagens solicitadas
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

    printf("\nEstat�sticas\n\n");
    printf("Total de usu�rios cadastrados: %d\n", qtdusuarios);
    printf("Total de livros cadastrados: %d\n", qtdlivros);
    printf("Total de aloca��es realizadas: %d\n", qtdtotalalocados);
    printf("Total de aloca��es pendentes de devolu��o: %d\n", qtdtotalpendentes);

}


/* fun��o principal do programa:
 � chamado a fun��o do menu e a partir do seu retorno, utiliza um switch para chamar
 as fun��es que o sistema possui */
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
                printf("Op��o inv�lida!\n");
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
