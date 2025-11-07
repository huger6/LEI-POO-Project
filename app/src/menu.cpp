#include "menu.hpp"

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <list>

using namespace std;

Menu::Menu() : fs(), running(true), loaded(false) {
    // Construtor: Inicializa o menu
}

void Menu::run() {
    // Loop principal do menu
    while (running) {
        clearScreen();
        displayMainMenu();
        int choice = getUserChoice();
        handleMainMenuInput(choice);
    }
}

/**
 * @brief Mostra o menu principal.
 */
void Menu::displayMainMenu() {
    cout << "╔=======================================╗\n";
    cout << "║      Gestor de Sistema de Ficheiros   ║\n";
    cout << "║           (Modo de Amostragem)        ║\n";
    cout << "╚=======================================╝\n";
    cout << "Estado: " << (loaded ? "Carregado" : "Nao Carregado") << "\n\n";

    cout << "1. Carregar File System (Obrigatorio para testar)\n";
    
    if (loaded) {
        cout << "2. Mostrar Tree\n";
        cout << "3. Mostrar Estatisticas\n";
        cout << "4. Operacoes de Pesquisa\n";
        cout << "5. Verificar Ficheiros Duplicados\n";
    }
    
    cout << "0. Sair\n"; // No menu principal, 0 = Sair
    cout << "=======================================\n";
    cout << "Escolha uma opcao: ";
}

/**
 * @brief Gere a escolha do menu principal.
 */
void Menu::handleMainMenuInput(int choice) {
    switch (choice) {
        case 1:
            loadFileSystem();
            break;
        case 0:
            // No menu principal, '0' chama exit() para terminar o programa
            exit(); 
            break;
        default:
            if (loaded) { // So permite opcoes > 1 se o FS estiver carregado
                switch (choice) {
                    case 2: showTree(); break;
                    case 3: showFileSystemStats(); break;
                    case 4: showSearchMenu(); break;
                    case 5: checkDuplicates(); break;
                    default:
                        cout << "Opcao invalida. Tente novamente.\n";
                        pause();
                        break;
                }
            } else {
                cout << "Opcao invalida. Carregue um sistema de ficheiros primeiro (Opcao 1).\n";
                pause();
            }
            break;
    }
}

/**
 * @brief Acao para carregar o sistema de ficheiros.
 */
void Menu::loadFileSystem() {
    string path = getStringInput("Introduza o caminho do directorio raiz: ");
    cout << "A carregar... (A chamar fs.load()).\n";
    
    // Chama a funcao de load do seu FileSystem
    if (fs.load(path)) {
        loaded = true;
        cout << "Sistema de ficheiros carregado com sucesso.\n";
    } else {
        loaded = false;
        cout << "Falha ao carregar o sistema de ficheiros (fs.load() retornou false).\n";
    }
    pause();
}

/**
 * @brief Acao para mostrar a arvore de directorios.
 */
void Menu::showTree() {
    clearScreen();
    cout << "Mostrando Arvore do Sistema de Ficheiros (A chamar fs.tree()):\n";
    
    // Chama a funcao de tree do seu FileSystem
    fs.tree(cout);
    pause();
}

/**
 * @brief Acao para mostrar as estatisticas.
 */
void Menu::showFileSystemStats() {
    clearScreen();
    cout << "===== Estatisticas do Sistema de Ficheiros =====\n";
    
    // Chama as funcoes de estatisticas do seu FileSystem
    cout << "Total de Ficheiros: " << fs.countFiles() << "\n";
    cout << "Total de Directorios: " << fs.countDirectories() << "\n";
    cout << "Memoria Total (bytes): " << fs.memory() << "\n\n";

    string* most = fs.mostElementsDirectory();
    cout << "Directorio com mais elementos: " << (most ? *most : "N/A") << "\n";

    string* least = fs.leastElementsDirectory();
    cout << "Directorio com menos elementos: " << (least ? *least : "N/A") << "\n";

    string* largest = fs.largestDirectory();
    cout << "Maior Directorio (por tamanho): " << (largest ? *largest : "N/A") << "\n\n";

    string file = getStringInput("Obter data do ficheiro (deixe em branco para saltar): ");
    if (!file.empty()) {
        string* date = fs.getFileDate(file);
        cout << "Data do Ficheiro: " << (date ? *date : "Ficheiro nao encontrado") << "\n";
    }
    
    pause();
}

/**
 * @brief Acao para mostrar o sub-menu de pesquisa.
 */
void Menu::showSearchMenu() {
    // Variavel de controlo do loop deste sub-menu
    bool back = false; 
    
    while (!back) { // O loop executa enquanto 'back' for false
        clearScreen();
        cout << "===== Operacoes de Pesquisa =====\n";
        cout << "1. Pesquisar (Ficheiro/Directorio por nome)\n";
        cout << "2. Pesquisar Todos os Directorios (por nome)\n";
        cout << "3. Pesquisar Todos os Ficheiros (por nome)\n";
        
        cout << "0. Voltar ao Menu Principal\n"; // Num sub-menu, 0 = Voltar
        cout << "=================================\n";
        cout << "Escolha uma opcao: ";
        
        int choice = getUserChoice();
        string s1;
        int type;
        list<string> results; // Para searchAll

        switch (choice) {
            case 1: // Pesquisar
                s1 = getStringInput("Nome a pesquisar: ");
                cout << "Tipo (ex: 1 para Ficheiro, 2 para Directorio): ";
                type = getUserChoice();
                {
                    string* result = fs.search(s1, type);
                    cout << "Encontrado em: " << (result ? *result : "Nao encontrado") << endl;
                }
                pause();
                break;
            case 2: // Pesquisar Todos os Directorios
                results.clear();
                s1 = getStringInput("Nome do directorio: ");
                fs.searchAllDirs(results, s1);
                cout << "Encontrados " << results.size() << " directorios:\n";
                for (const auto& res : results) cout << "- " << res << endl;
                pause();
                break;
            case 3: // Pesquisar Todos os Ficheiros
                results.clear();
                s1 = getStringInput("Nome do ficheiro: ");
                fs.searchAllFiles(results, s1);
                cout << "Encontrados " << results.size() << " ficheiros:\n";
                for (const auto& res : results) cout << "- " << res << endl;
                pause();
                break;
            case 0:
                // '0' mete 'back' a true, quebrando o loop 'while'
                // e voltando ao menu principal.
                back = true; 
                break;
            default:
                cout << "Opcao invalida.\n";
                pause();
                break;
        }
    }
}

/**
 * @brief Acao para verificar duplicados.
 */
void Menu::checkDuplicates() {
    clearScreen();
    cout << "A verificar ficheiros duplicados (A chamar fs.checkDupFiles())...\n";
    
    bool found = fs.checkDupFiles();
    
    if (found) {
        cout << "Foram encontrados ficheiros duplicados.\n";
    } else {
        cout << "Nao foram encontrados ficheiros duplicados.\n";
    }
    pause();
}

/**
 * @brief Altera a flag 'running' para false, terminando o loop principal.
 */
void Menu::exit() {
    running = false;
    cout << "A sair...\n";
}

// --- Métodos Utilitários (Helpers) ---

/**
 * @brief Le um inteiro do utilizador de forma segura.
 */
int Menu::getUserChoice() const {
    int choice;
    // Loop ate que um inteiro valido seja inserido
    while (!(cin >> choice)) {
        cout << "Input invalido. Por favor, introduza um numero: ";
        cin.clear(); // Limpa o estado de erro
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Descarta a linha invalida
    }
    // Limpa o buffer de input (para o caso de 'getline' ser usado a seguir)
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

/**
 * @brief Le uma string completa (linha) do utilizador.
 */
string Menu::getStringInput(const string& prompt) const {
    cout << prompt;
    string input;
    getline(cin, input);
    return input;
}

/**
 * @brief Limpa o ecra da consola.
 */
void Menu::clearScreen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear"); // Assume POSIX (Linux, macOS)
    #endif
}

/**
 * @brief Pausa o programa e espera que o utilizador prima Enter.
 */
void Menu::pause() const {
    cout << "\nPressione Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}