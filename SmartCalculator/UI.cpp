#include "UI.h"
#include <cstdlib> 
#include "Calculator.h"
#include <conio.h>
#include <sstream>
#include <iomanip>
#include "Token.h"
#include <fstream>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int _getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

int _kbhit() {
    struct timeval tv = { 0, 0 };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
#endif

void UI::ClearConsole()
{
#ifdef _WIN32
    system("cls"); // Windows
#else
    system("clear"); // Linux/macOS
#endif
}

bool UI::Menu()
{
    std::cout << " 1. 'N'ew expression \t2. 'V'iew previous results. \t3. 'C'lear console. \t4. 'S'ettings. \t5. 'E'xit. \n 9. 'H'elp" << std::endl;
    while (true) {
        
        if (_kbhit()) { 
            char ch = _getch(); 

            if (ch == 13 || ch == 10) {
                continue;
            }

            switch (ch)
            {
            case 'n':
            case 'N':
            case '1':
                NewExpression();
                std::cout << " 1. 'N'ew expression \t2. 'V'iew previous results. \t3. 'C'lear console. \t4. 'S'ettings. \t5. 'E'xit. \n 9. 'H'elp" << std::endl;
                continue;

            case 'v':
            case 'V':
            case '2':
                ViewTable();
                std::cout << " 1. 'N'ew expression \t2. 'V'iew previous results. \t3. 'C'lear console. \t4. 'S'ettings. \t5. 'E'xit. \n 9. 'H'elp" << std::endl;
                continue;

            case 'c':
            case 'C':
            case '3':
                ClearConsole();
                std::cout << " 1. 'N'ew expression \t2. 'V'iew previous results. \t3. 'C'lear console. \t4. 'S'ettings. \t5. 'E'xit. \n 9. 'H'elp" << std::endl;
                continue;

            case 's':
            case 'S':
            case '4':
                ClearConsole();
                Settings();
                std::cout << " 1. 'N'ew expression \t2. 'V'iew previous results. \t3. 'C'lear console. \t4. 'S'ettings. \t5. 'E'xit. \n 9. 'H'elp" << std::endl;

                continue;
            case 'e':
            case 'E':
            case '5':
                    return 0;
            
            case 'h':
            case 'H':
            case '9':
                std::cout << "\t This program is a console engineering calculator." << std::endl;
                std::cout << "\t The following functions are implemented in this application:" << std::endl;
                std::cout << "\t Basic math operations ('+','-','*','/','^')" << std::endl;
                std::cout << "\t Basic trigonometric functions (sin(x), cos(x), tan(x), cot(x))" << std::endl;
                std::cout << "\t Logarithmic operations (log(x) - decimal logarithm, ln(x) - natural logarithm)" << std::endl;
                std::cout << "\t It is also possible to extract roots from a number or expression." << std::endl;
                std::cout << " 1. 'N'ew expression \t2. 'V'iew previous results. \t3. 'C'lear console. \t4. 'S'ettings. \t5. 'E'xit." <<std::endl<<"9.'H'elp" << std::endl;

                continue;
            default:
                std::cout << "Wrong symbol. Try again." << std::endl;
                continue;
            }
        }
    }
}
    //Main block of calculations
void UI::NewExpression()
{
    std::string expression;
    std::cout << "Enter an expression: ";

    while (expression.empty()) {
        std::getline(std::cin, expression);
    }
    try {
        std::vector<Token> tokens = Token().Tokenize(expression);

        Calculator calc;
        if (calc.validateExpression(tokens, expression))
        {
            double result = calc.evaluation(tokens);
            std::cout << "Result: " << result << std::endl;
            ResTable.push_front(result);

        }
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}


bool UI::ViewTable()
{
    ClearConsole();
    std::cout << std::setw(5) << "No."
        << std::setw(20) << "Expression"
        << std::setw(20) << "Value" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;


    int index = 1;
    auto resIt = ResTable.begin();          
    auto exprIt = Token::AllExpressions.begin(); 

    while (resIt != ResTable.end() && exprIt != Token::AllExpressions.end()) {
        std::cout << std::setw(5) << index
            << std::setw(20) << *exprIt    
            << std::setw(20) << *resIt    
            << std::endl;
        ++index;
        ++resIt;  
        ++exprIt;  
    }

    std::cout << " 1.'B'ack to menu. \t2. 'C'lear result \t3. Create .txt 'f'ile" << std::endl;
    while (true) {
        if (_kbhit()) {
            char ct = _getch();

            if (ct == 13 || ct == 10) {
                continue;
            }

            switch (ct)
            {
            case 'b':
            case 'B':
            case '1':
                return 0;

            case 'c':
            case 'C':
            case '2':
                ResTable.clear();
                std::cout << " Back to menu. "<< std::endl;

                return 0;

            case 'f':
            case 'F':
            case '3':
                UI::CreateTXT();
                std::cout << " 1.'B'ack to menu. \t2. 'C'lear result \t3. Create .txt 'f'ile" << std::endl;
                continue;

            default:
                std::cout << " Wrong symbol. Try again." << std::endl;
                continue;
            }
        }
    }

}

void UI::CreateTXT()
{
    std::ofstream outFile("output.txt");
    if (!outFile) {
        throw std::runtime_error("Error opening the file!");
    }

    outFile << std::setw(5) << "No."
        << std::setw(20) << "Expression"
        << std::setw(20) << "Value" << std::endl;
    outFile << "----------------------------------------------------" << std::endl;

    int index = 1;
    auto resIt = ResTable.begin();
    auto exprIt = Token::AllExpressions.begin();

    while (resIt != ResTable.end() && exprIt != Token::AllExpressions.end()) {
        outFile << std::setw(5) << index
            << std::setw(20) << *exprIt
            << std::setw(20) << *resIt
            << std::endl;
        ++index;
        ++resIt;
        ++exprIt;
    }

    outFile.close();

    std::cout << "File 'output.txt' has been created and data has been written to it." << std::endl;
}

bool UI::Settings()
{
    std::cout << " 1. Set 'P'recision. \t2.'E'xit." << std::endl;

    while (true) {
        if (_kbhit()) {
            char set = _getch();

            if (set == 13 || set == 10) {
                continue;
            }

            switch (set)
            {
            case 'P':
            case 'p':
            case '1':

                std::cout << "Enter new precision: ";
                std::cin >> this->m;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid input. Please enter a valid number.\n";
                    std::cout << " 1. Set 'P'recision. \t2.'B'ack to menu." << std::endl;
                    continue; 
                }

   
                std::cout << std::fixed << std::setprecision(m);
                std::cout << "New precision: " << this->m << std::endl;

                std::cout << " 1. Set 'P'recision. \t2.'B'ack to menu." << std::endl;
                continue;
            case 'b':
            case 'B':
            case '2':
                return 0;

            default:
                std::cout << "Wrong symbol. Try again." << std::endl;
                continue;
            }
        }
    }
}
