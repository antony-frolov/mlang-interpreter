#ifndef MLANG_INTERPRETER_EXECUTER_H
#define MLANG_INTERPRETER_EXECUTER_H

class Executer {
public:
    void execute(vector<Lex>& poliz);
};

void Executer::execute(vector<Lex>& poliz) {
    Lex pc_el;
    stack<pair<int, int>> args;
    int index = 0, size = poliz.size();
    pair<int, int> i, j;
    Ident* ident_ptr;
    while (index < size) {
        pc_el = poliz[index];
        cout << "-" << index << endl;
        switch (pc_el.get_type()) {
            case LEX_TRUE: case LEX_FALSE: case LEX_NUM: case LEX_STR: case POLIZ_ADDRESS: case POLIZ_LABEL:
                args.push(pair<int, int>(pc_el.get_value(), pc_el.get_aux_value()));
                break;
            case LEX_ID:
                if (pc_el.get_aux_value() == -1) {
                    ident_ptr = &TID[pc_el.get_value()];
                } else {
                    ident_ptr = &TSTRUCT[pc_el.get_aux_value()].tid[pc_el.get_value()];
                }
                if (ident_ptr->get_assign()) {
                    args.push(pair<int, int>(ident_ptr->get_value(), -1));
                    break;
                } else {
                    throw "POLIZ: indefinite identifier";
                }

            case LEX_NOT:
                from_st(args, i);
                args.push(pair<int, int>(!i.first, -1));
                break;

            case LEX_OR:
                from_st (args, i);
                from_st (args, j);
                args.push (pair<int, int>(j.first || i.first, -1));
                break;

            case LEX_AND:
                from_st (args, i);
                from_st (args, j);
                args.push (pair<int, int>(j.first && i.first, -1));
                break;

            case POLIZ_GO:
                from_st(args, i);
                index = i.first - 1;
                break;

            case POLIZ_FGO:
                from_st (args, i);
                from_st (args, j);
                if (!j.first) index = i.first - 1;
                break;

            case LEX_WRITE:
                from_st(args, j);
                cout << j.first << endl;
                break;

            case LEX_SWRITE:
                from_st(args, j);
                cout << TSTR[j.first] << endl;
                break;


            case LEX_READ:
                int k;
                from_st (args, i);
                if (i.second == -1) {
                    ident_ptr = &TID[i.first];
                } else {
                    ident_ptr = &TSTRUCT[i.second].tid[i.first];
                }
                if (ident_ptr->get_type() == LEX_INT) {
                    cout << "Input int value for " << ident_ptr->get_name() << endl;
                    cin >> k;
                    ident_ptr->put_value(k);
                } else if (ident_ptr->get_type() == LEX_STRING) {
                    string s;
                    cout << "Input string for " << ident_ptr->get_name() << endl;
                    cin >> s;
                    TSTR[ident_ptr->get_value()] = s;
                } else {
                    string j;
                    while (1) {
                        cout << "Input boolean value (true or false) for " << ident_ptr->get_name() << endl;
                        cin >> j;
                        if (j != "true" && j != "false") {
                            cout << "Error in input:true/false" << endl;
                            continue;
                        }
                        k = (j == "true") ? 1 : 0;
                        break;
                    }
                    ident_ptr->put_value(k);
                }
                ident_ptr->put_assign();
                break;

            case LEX_PLUS:
                from_st (args, i);
                from_st (args, j);
                args.push (pair<int, int>(i.first + j.first, -1));
                break;

            case LEX_SPLUS:
                from_st (args, i);
                from_st (args, j);
                args.push(pair<int, int>(TSTR.size(), -1));
                TSTR.push_back(TSTR[j.first] + TSTR[i.first]);
                break;

            case LEX_TIMES:
                from_st (args, i);
                from_st (args, j);
                args.push (pair<int, int>(i.first * j.first, -1));
                break;

            case LEX_MINUS:
                from_st (args, i);
                from_st (args, j);
                args.push (pair<int, int>(j.first - i.first, -1));
                break;

            case LEX_SLASH:
                from_st ( args, i );
                from_st ( args, j );
                if (!i.first) {
                    args.push (pair<int, int>(j.first / i.first, -1));
                    break;
                }
                else
                    throw "POLIZ:divide by zero";

            case LEX_EQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(i.first == j.first, -1));
                break;

            case LEX_LSS:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(j.first < i.first, -1));
                break;

            case LEX_GTR:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(j.first > i.first, -1));
                break;

            case LEX_LEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(j.first <= i.first, -1));
                break;

            case LEX_GEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(j.first >= i.first, -1));
                break;

            case LEX_NEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(j.first != i.first, -1));
                break;

            case LEX_SEQ:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(TSTR[j.first].compare(TSTR[i.first]) == 0, -1));
                break;

            case LEX_SLSS:
                from_st ( args, i );
                from_st ( args, j );
                args.push (pair<int, int>(TSTR[j.first].compare(TSTR[i.first]) < 0, -1));
                break;

            case LEX_SGTR:
                from_st(args, i);
                from_st(args, j);
                args.push (pair<int, int>(TSTR[j.first].compare(TSTR[i.first]) > 0, -1));
                break;

            case LEX_SNEQ:
                from_st(args, i);
                from_st(args, j);
                args.push (pair<int, int>(TSTR[j.first].compare(TSTR[i.first]) != 0, -1));
                break;

            case LEX_ASSIGN:
                from_st(args, i);
                from_st(args, j);
                if (j.second == -1) {
                    ident_ptr = &TID[j.first];
                } else {
                    ident_ptr = &TSTRUCT[j.second].tid[j.first];

                }
                ident_ptr->put_value(i.first);
                ident_ptr->put_assign();
                args.push(pair<int, int>(ident_ptr->get_value(), -1));
                break;

            case LEX_SASSIGN:
                from_st(args, i);
                from_st(args, j);
                if (j.second == -1) {
                    ident_ptr = &TID[j.first];
                } else {
                    ident_ptr = &TSTRUCT[j.second].tid[j.first];

                }
                TSTR[ident_ptr->get_value()] = TSTR[i.first];
                ident_ptr->put_assign();
                args.push(pair<int, int>(ident_ptr->get_value(), -1));
                break;

            case LEX_STRASSIGN:
                from_st(args, i);
                from_st(args, j);
                TSTRUCT[TID[j.first].get_value()] = TSTRUCT[i.first];
                TID[j.first].put_assign();
                args.push(pair<int, int>(TID[j.first].get_value(), -1));
                break;

            case LEX_SEMICOLON:
                from_st(args, j);
                break;

            default:
                throw "POLIZ: unexpected elem";
        }//end of switch
        ++index;
    };//end of while
    cout << "Finish of executing!!!" << endl;
}

class Interpretator {
    Parser   pars;
    Executer E;
public:
    Interpretator ( const char* program ): pars (program) {}
    void     interpretation ();
};

void Interpretator::interpretation () {
    pars.analyze ();
    E.execute ( pars.poliz );
}

#endif //MLANG_INTERPRETER_EXECUTER_H
