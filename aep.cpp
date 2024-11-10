#include <stdio.h>
#include <string.h>

void criptografar(char texto[]) {
    int i;
    for(i = 0; texto[i] != '\0' && texto[i] != '\n'; i++) {
        if(texto[i] >= 32 && texto[i] <= 126) { 
            texto[i] = ((texto[i] - 32 + 3) % 95) + 32;
        }
    }
}

void descriptografar(char texto[]) {
    int i;
    for(i = 0; texto[i] != '\0' && texto[i] != '\n'; i++) {
        if(texto[i] >= 32 && texto[i] <= 126) {
            texto[i] = ((texto[i] - 32 - 3 + 95) % 95) + 32;
        }
    }
}

int main(){
    int operacao;
    FILE *arquivo1, *arquivo_temp, *arquivo2;
    char nomes[100], nome[100], palavra[100], senha[100], usuario_excluir[100], usuario_temp[100], troca[100];
    int usuario_existe=0, encontrado=0, contador_linha=0, troca_nome=0, linha_excluir = 0;
    
    printf("Qual funcionalidade quer usar?\n\n");
    printf("Inclusao de novo Usuario = 1\n");
    printf("Alteracao de Usuario = 2\n");
    printf("Exclusao de Usuario = 3\n");
    printf("Listagem dos Usuarios = 4\n\n");
    scanf("%d",&operacao);
    
    switch(operacao){
        case(1):
            arquivo1 = fopen("Usuarios.txt", "r");
            arquivo2 = fopen("Senhas.txt", "r");
            if(arquivo1 == NULL || arquivo2 == NULL){
                printf("Arquivo nao encontrado!");
                return 0;
            }

            printf("Inclusao de usuario:\n");
            printf("Digite o novo usuario: ");
            scanf("%s", nome);
            
            while (fscanf(arquivo1, "%s", palavra) != EOF) {
                descriptografar(palavra);
                if (strcmp(palavra, nome) == 0) {  
                    usuario_existe = 1;
                }
            }
            fclose(arquivo1);
            if (usuario_existe==1) {
                printf("Usuario ja existe!\n");
                return 0;
            } else {
                printf("Digite sua senha forte: ");
                scanf("%s", senha);
                
                if (strlen(senha) > 8) {
                    printf("Senha cadastrada com sucesso!\n");
                } else {
                    printf("Senha fraca! deve ter mais que 8 caracteres.\n");
                    scanf("%s", senha);
                }
                
                if(strlen(senha)>8){
                    criptografar(nome);
                    criptografar(senha);
                    
                    arquivo1 = fopen("Usuarios.txt", "a");
                    fprintf(arquivo1, "%s\n", nome);
                    fclose(arquivo1);
                    arquivo2 = fopen("Senhas.txt", "a");
                    printf("Usuario foi adicionado com sucesso!\n");
                    fprintf(arquivo2, "%s\n", senha);
                    fclose(arquivo2);
                }
            }
            break;
            
        case(2):
            printf("Qual o nome do usuario que deseja trocar? \n");
            scanf("%s", troca);
        
            arquivo1 = fopen("Usuarios.txt", "r");
            if (arquivo1 == NULL) {
                printf("Arquivo não encontrado!\n");
                return 0;
            }
        
            while (fscanf(arquivo1, "%s", palavra) != EOF) {
                descriptografar(palavra);
                if (strcmp(palavra, troca) == 0) {
                    troca_nome = 1;
                    break;
                }
            }
            fclose(arquivo1);
        
            if (troca_nome == 0) {
                printf("Usuario nao encontrado!\n");
                return 0;
            } else {
                printf("Quer trocar o usuario '%s' por qual nome? ", troca);
                scanf("%s", usuario_temp);
        
                arquivo1 = fopen("Usuarios.txt", "r");
                arquivo_temp = fopen("usuarios_temp.txt", "w");
        
                while (fscanf(arquivo1, "%s", palavra) != EOF) {
                    char palavra_temp[100];
                    strcpy(palavra_temp, palavra);
                    descriptografar(palavra_temp);
                    if (strcmp(palavra_temp, troca) == 0) {
                        criptografar(usuario_temp);
                        fprintf(arquivo_temp, "%s\n", usuario_temp);
                    } else {
                        fprintf(arquivo_temp, "%s\n", palavra);
                    }
                }
        
                fclose(arquivo1);
                fclose(arquivo_temp);
        
                remove("Usuarios.txt");
                rename("usuarios_temp.txt", "Usuarios.txt");
        
                printf("Nome do usuario foi mudado com sucesso!\n");
            }
            break;

        case(3):
            printf("Digite o nome do usuario que deseja excluir: ");
            scanf("%s", usuario_excluir);
        
            arquivo1 = fopen("Usuarios.txt", "r");
            if (arquivo1 == NULL) {
                printf("Arquivo nao encontrado!\n");
                break;
            }
        
            while (fscanf(arquivo1, "%s", palavra) != EOF) {
                char palavra_temp[100];
                strcpy(palavra_temp, palavra);
                descriptografar(palavra_temp);
                if (strcmp(palavra_temp, usuario_excluir) == 0) {
                    encontrado = 1;
                    break;
                }
            }
            fclose(arquivo1);
        
            if (!encontrado) {
                printf("Usuario nao encontrado!\n");
                return 0;
            }
        
            arquivo1 = fopen("Usuarios.txt", "r");
            arquivo_temp = fopen("usuarios_temp.txt", "w");
        
            while (fscanf(arquivo1, "%s", palavra) != EOF) {
                char palavra_temp[100];
                strcpy(palavra_temp, palavra);
                descriptografar(palavra_temp);
                if (strcmp(palavra_temp, usuario_excluir) != 0) {
                    fprintf(arquivo_temp, "%s\n", palavra);
                }
            }
            fclose(arquivo1);
            fclose(arquivo_temp);
        
            arquivo2 = fopen("Senhas.txt", "r");
            arquivo_temp = fopen("senhas_temp.txt", "w");
        
            arquivo1 = fopen("Usuarios.txt", "r");
            contador_linha = 0;
            while (fscanf(arquivo1, "%s", palavra) != EOF) {
                char palavra_temp[100];
                strcpy(palavra_temp, palavra);
                descriptografar(palavra_temp);
                if (strcmp(palavra_temp, usuario_excluir) == 0) {
                    linha_excluir = contador_linha;
                    break;
                }
                contador_linha++;
            }
            fclose(arquivo1);
        
            contador_linha = 0;
            while (fscanf(arquivo2, "%s", palavra) != EOF) {
                if (contador_linha != linha_excluir) {
                    fprintf(arquivo_temp, "%s\n", palavra);
                }
                contador_linha++;
            }
            fclose(arquivo2);
            fclose(arquivo_temp);
        
            remove("Usuarios.txt");
            rename("usuarios_temp.txt", "Usuarios.txt");
        
            remove("Senhas.txt");
            rename("senhas_temp.txt", "Senhas.txt");
        
            printf("Usuario %s excluido com sucesso!\n", usuario_excluir);
            break;
        
        case(4):
            arquivo1 = fopen("Usuarios.txt", "r");
            if(arquivo1 == NULL){
                printf("Arquivo nao encontrado!");
                return 0;
            }
            printf("Usuarios cadastrados:\n");
            while (fscanf(arquivo1, "%s", nomes) != EOF){
                descriptografar(nomes);
                printf("%s\n", nomes);
            }
            
            fclose(arquivo1);
            break;    
            
        default:
            printf("Opcao invalida!\n");
            break;
    }
    
    return 0;
}
