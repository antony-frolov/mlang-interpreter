#include "/Users/antony/CLionProjects/mlang_interpreter/scanner.h"
#include "/Users/antony/CLionProjects/mlang_interpreter/parser.h"
// test Scanner
//int main() {
//    Scanner scan("/Users/antony/CLionProjects/prak4/model_lang/model_program.txt");
//    Lex lex;
//    do {
//        try {
//            lex = scan.get_lex();
//        } catch (char c) {
//            if (c != EOF) {
//                cout << "Caught an exception -- " << c << endl;
//            } else {
//                cout << "Caught an exception -- EOF" << endl;
//            }
//            return 0;
//        }
//        cout << lex << endl;
//    } while (lex.get_type() != LEX_FIN);
//    return 0;
//}

// test Parser
int main() {
    try {
        Parser P("/Users/antony/CLionProjects/mlang_interpreter/model_program.txt");
        P.analyze();
        return 0;
    }
    catch (char c) {
        cout << "unexpected symbol " << c << endl;
        return 1;
    }
    catch (Lex l) {
        cout << "unexpected lexeme" << l << endl;
        return 1;
    }
    catch (const char* source) {
        cout << source << endl;
        return 1;
    }
}