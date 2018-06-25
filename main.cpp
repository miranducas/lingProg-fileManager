#include <iostream>
#include "CORE/EXTERN.h"
#include "CORE/perl.h"
#include "CORE/XSUB.h"
#include <string>
#include <stddef.h>
#include "Classes/Exclusao.h"
#include "Classes/Pesquisa.h"
#include "Classes/Menu.h"

using namespace std;
 
EXTERN_C void xs_init (pTHX);
 
EXTERN_C void boot_DynaLoader (pTHX_ CV* cv);
//EXTERN_C void PL_memory_wrap (pTHX_ CV* cv);
 
EXTERN_C void
xs_init(pTHX)
{
    static const char file[] = __FILE__;
    dXSUB_SYS;
    PERL_UNUSED_CONTEXT;
 
    /* DynaLoader is a special case */
    newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file);
}
 
PerlInterpreter *my_perl;

char * pesquisaRetorna (char *arquivoParam) {
    dSP;
    ENTER;
    SAVETMPS;
    PUSHMARK(SP);
    XPUSHs(sv_2mortal(newSVpv(arquivoParam,strlen(arquivoParam))));
    PUTBACK;
    call_pv("fileManager::pesquisaRetorna", G_SCALAR);
    SPAGAIN;
 
    char *arquivoRet = POPp;
    PUTBACK;
    FREETMPS;
    LEAVE;
 
    return arquivoRet;
}

void criarArquivo(string nomeArquivo){
    FILE *arquivo;
    const char *nome;
    nomeArquivo = nomeArquivo+".txt";
    nome = nomeArquivo.c_str();
    //strcat(nome,".txt");
    arquivo = fopen (nome,"a+");

    cout<<"O arquivo foi criado com o nome: "<<nomeArquivo;
    fclose (arquivo);
}

int main(int argc, char **argv, char **env) {
 
    char _MYARGV_PERL_MODULE_NAME[] = "main.pl"; //evita warnings
    char _MYARGV_NOTHING_NAME[] = ""; //evita warnings
    char *my_argv[] = {static_cast<char*> (_MYARGV_NOTHING_NAME), static_cast<char*>
    (_MYARGV_PERL_MODULE_NAME)}; //nome do arquivo como argv[1]

    //char *my_argv[] = { "", "main.pl"};
    PERL_SYS_INIT3 (&argc, &argv, &env);
 
    my_perl = perl_alloc();
    perl_construct(my_perl);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
 
    //perl_parse(my_perl, NULL, 2, my_argv, (char **)NULL);
    perl_parse(my_perl, xs_init, 2, my_argv, (char **)NULL);
    perl_run(my_perl);

    /******************PESQUISA******************/
  
    char *nomeArqParam = static_cast<char*>("parametros.txt");

    cout << "Arquivo passado: " << nomeArqParam << endl;

    char *nomeArqRet = pesquisaRetorna (nomeArqParam);

    cout << "Arquivo retornado: " << nomeArqRet << endl;
    /******************************************/
    unsigned int opcao;
    Menu menu;

    string nomeArqParamstr = "parametros.txt";
    string nomeArqRetStr = "lista-de-retorno.txt";

    string strDsj;
    Pesquisa pesquisa (nomeArqParam, nomeArqRetStr);
    //cout << "Digite o nome do arquivo que deseja excluir: "<<endl;
    //getline (cin, strDsj);

    do {
        opcao = menu.exibe ();
        Pesquisa exibicao (nomeArqParamstr, nomeArqRetStr);
        Exclusao exclusao (pesquisa.extraiString ());

        switch (opcao) {
            case 0:
                menu.termina ();
                break;                       
            case 1:
                cout << "Opcao escolhida: " << opcao << endl;
                break;        
            case 2:
                
                cout << "Opcao escolhida: " << opcao << endl;
                //EXIBICAO DE ARQUIVOS
                //Pesquisa exibicao (nomeArqParamstr, nomeArqRetStr);
                //nomeArqRet = pesquisaRetorna (nomeArqParamstr);
                //exibicao.mostra (nomeArqRetStr);
                exibicao.geraParametros ("SARQ","");

                break;
            case 3:
                cout << "Opcao escolhida: " << opcao << endl;
                cout << "Digite o nome do arquivo que deseja criar: "<<endl;
                cin.ignore();
                getline(cin,strDsj);
                criarArquivo(strDsj);
                break;
            case 4:
                //EXCLUSAO DE ARQUIVOS
                //filtra para achar o arquivo desejado
                //cout << "Digite o nome do arquivo que deseja excluir: "<<endl;
                //getline (cin, strDsj);
                cout << "Opcao escolhida: " << opcao << endl;
                cout << "Digite o nome do arquivo que deseja excluir: "<<endl;
                cin.ignore();
                getline(cin,strDsj);
                cout << strDsj;
                //pesquisa.geraParametros ("FSTR",strDsj);
                //cria um objeto de exclusao a partir do nome extraido de parametros.txt
                //exclusao.setNome(pesquisa.extraiString ());
                exclusao.setNome(strDsj);
                //verifica se pode ser excluido, e exclui no caso de sucesso
                if (exclusao.exclui ()) {
                    cout << "Arquivo " << exclusao.getNome () << " excluido com sucesso!" << endl;
                }
                else {
                    cout << "O arquivo " << exclusao.getNome () << "nao existe ou nao pode ser excluido!" << endl;
                }
                break;                
            default:
                cout << "Opcao Invalida" << endl;
                break;
        }
    } while (opcao != 0);

    return 0;

 }