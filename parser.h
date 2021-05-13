#ifndef MLANG_INTERPRETER_PARSER_H
#define MLANG_INTERPRETER_PARSER_H

#include "stack"
#include "vector"
#include "algorithm"

template <class T, class T_EL>
void from_st (T& st, T_EL& i) {
    i = st.top();
    st.pop();
}


vector<vector<int>> TGOTO;
stack<vector<int>>  STBREAK;

class Parser {
    Lex         curr_lex;
    type_of_lex c_type;
    int         c_val;
    Scanner     scan;
    stack<pair<type_of_lex, int>> st_lex;

    void Program();

    void StructDeclars();
    void StructDeclar();
    void StructVarDeclar(int struct_type_idx);
    type_of_lex StructVarType();
    void StructVar(type_of_lex type, int struct_type_idx);
    void StructConst(int type, int struct_type_idx, int id_idx);

    void Declars();
    void Declar();
    void Operats();
    void Operat();
    void CompOperat();
    void TaggedOperat();
    void ExprOperat();
    type_of_lex Type(int& struct_type_idx);
    void Var(type_of_lex type, int struct_type_idx);
    void Const(type_of_lex type, int id_idx);
    void Expr();
    void Assign();
    void Or();
    void And();
    void Comp();
    void Ar_expr();
    void Term();
    void Factor();
    void Id(bool address, bool push);

    void dec(type_of_lex type, int struct_type_idx, int id_idx);
    void check_id(Ident ident, bool push);
    void check_op();
    void check_not();
    void check_minus();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void dec_label(int id_idx);
    void goto_label();
    void dec_struct_type(int id_idx);
    int dec_struct_var(type_of_lex type, int struct_type_idx);
    void init_str_vars(int struct_idx);
    void check_labels();

    void gl() {
        curr_lex  = scan.get_lex();
        c_type    = curr_lex.get_type();
        c_val     = curr_lex.get_value();
    }
    void ungl(const Lex& lex) {
        scan.unget_lex(lex);
    }

public:
    vector<Lex> poliz;
    explicit Parser(const char* program) : scan(program) {}
    void analyze();
};

void Parser::analyze() {
    gl();
    Program();
    if (c_type != LEX_FIN) {
        throw curr_lex;
    }
    check_labels();
    for (Lex l : poliz)
        cout << l;
    cout << endl << "Yes!!!" << endl;
}

// StructDeclars -> /{ StructDeclar /}
void Parser::StructDeclars() {
    while (c_type == LEX_STRUCT) {
        StructDeclar();
    }
}

// StructDeclar -> struct ident { Declar ; /{ Declar ; /} };
void Parser::StructDeclar() {
    if (c_type == LEX_STRUCT) {
        gl();
    } else {
        throw curr_lex;
    }
    if (c_type == LEX_ID) {
        dec_struct_type(c_val);
    } else {
        throw curr_lex;
    }
    int struct_type_idx = TID[c_val].get_value();
    gl();
    if (c_type == LEX_LBRACE) {
        gl();
    } else {
        throw curr_lex;
    }
    StructVarDeclar(struct_type_idx);
    if (c_type == LEX_SEMICOLON) {
        gl();
    } else {
        throw curr_lex;
    }
    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
        StructVarDeclar(struct_type_idx);
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    }
    if (c_type == LEX_RBRACE) {
        gl();
    } else {
        throw curr_lex;
    }
    if (c_type == LEX_SEMICOLON) {
        gl();
    } else {
        throw curr_lex;
    }
}

void Parser::StructVarDeclar(int struct_type_idx) {
    type_of_lex type = StructVarType();
    StructVar(type, struct_type_idx);
    while (c_type == LEX_COMMA) {
        gl();
        StructVar(type, struct_type_idx);
    }
}

//  Type -> int | string | boolean
type_of_lex Parser::StructVarType() {
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
        type_of_lex type = c_type;
        gl();
        return type;
    } else {
        throw curr_lex;
    }
}

// Var -> ident | ident = const
void Parser::StructVar(type_of_lex type, int struct_type_idx) {
    int id_idx;
    if (c_type == LEX_ID) {
        id_idx = dec_struct_var(type, struct_type_idx);
    } else {
        throw curr_lex;
    }
    gl();
    if (c_type == LEX_ASSIGN) {
        gl();
        StructConst(type, struct_type_idx, id_idx);
    }
}

// Const -> const
void Parser::StructConst(int type, int struct_type_idx, int id_idx) {
    if (c_type == LEX_NUM) {
        if (type != LEX_INT) {
            throw curr_lex;
        }
        TTYPE[struct_type_idx].tid[id_idx].put_value(c_val);
        gl();
    } else if (c_type == LEX_STR) {
        if (type != LEX_STRING) {
            throw curr_lex;
        }
        TTYPE[struct_type_idx].tid[id_idx].put_value(c_val);
        gl();
    } else if ((c_type == LEX_FALSE) || (c_type == LEX_TRUE)) {
        if (type != LEX_BOOL) {
            throw curr_lex;
        }
        TTYPE[struct_type_idx].tid[id_idx].put_value(c_type == LEX_TRUE ? 1 : 0);
        gl();
    } else {
        throw curr_lex;
    }
}

// Program -> program { Declars Operats }
void Parser::Program() {
    if (c_type == LEX_PROGRAM) {
        gl();
    } else {
        throw curr_lex;
    }
    if (c_type == LEX_LBRACE) {
        gl();
    } else {
        throw curr_lex;
    }
    StructDeclars();
    Declars();
    Operats();
    if (c_type == LEX_RBRACE) {
        gl();
    } else {
        throw curr_lex;
    }
}


// Declars -> /{ Declar ; /}
void Parser::Declars() {
    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL || c_type == LEX_ID) {
        if (c_type == LEX_ID) {
            if (TID[c_val].get_type() != LEX_STRUCT_TYPE) {
                return;
            }
        }
        Declar();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    }

}

// Type Var \{ , Var /}
void Parser::Declar() {
    int struct_type_idx = -1;
    type_of_lex type = Type(struct_type_idx);
    Var(type, struct_type_idx);
    while (c_type == LEX_COMMA) {
        gl();
        Var(type, struct_type_idx);
    }
}

//  Type -> int | string | boolean
type_of_lex Parser::Type(int& struct_type_idx) {
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
        type_of_lex type = c_type;
        gl();
        return type;
    } else if (c_type == LEX_ID) {
        if (TID[c_val].get_type() == LEX_STRUCT_TYPE) {
            struct_type_idx = TID[c_val].get_value();
            gl();
            return LEX_STRUCT;
        } else {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}

// Var -> ident | ident = const
void Parser::Var(type_of_lex type, int struct_type_idx) {
    if (c_type == LEX_ID) {
        dec(type, struct_type_idx, c_val);
    } else {
        throw curr_lex;
    }
    int id_idx = c_val;
    gl();
    if (type != LEX_STRUCT) {
        if (c_type == LEX_ASSIGN) {
            gl();
            Const(type, id_idx);
        }
    }
}

// Const -> const
void Parser::Const(type_of_lex type, int id_idx) {
    if (c_type == LEX_NUM) {
        if (type != LEX_INT) {
            throw curr_lex;
        }
        TID[id_idx].put_value(c_val);
        TID[id_idx].put_assign();
        gl();
    } else if (c_type == LEX_STR) {
        if (type != LEX_STRING) {
            throw curr_lex;
        }
        TSTR[TID[id_idx].get_value()] = TSTR[c_val];
        TID[id_idx].put_assign();
        gl();
    } else if ((c_type == LEX_FALSE) || (c_type == LEX_TRUE)) {
        if (type != LEX_BOOL) {
            throw curr_lex;
        }
        TID[id_idx].put_value(c_type == LEX_TRUE ? 1 : 0);
        TID[id_idx].put_assign();
        gl();
    } else {
        throw curr_lex;
    }
}

// Operats -> /{ Operat /}
void Parser::Operats() {
    while ((c_type == LEX_IF) || (c_type == LEX_FOR) || (c_type == LEX_WHILE) ||
           (c_type == LEX_BREAK) || (c_type == LEX_GOTO) || (c_type == LEX_READ) ||
           (c_type == LEX_WRITE) || (c_type == LEX_ID) || (c_type == LEX_NOT) ||
           (c_type == LEX_MINUS) || (c_type == LEX_NUM) || (c_type == LEX_STR) ||
           (c_type == LEX_FALSE) || (c_type == LEX_TRUE) || (c_type == LEX_LPAREN) ||
           (c_type == LEX_LBRACE)) {
        Operat();
    }
}

void Parser::Operat() {
//  cout << "Operat " << curr_lex << endl;
    int if_pl0, if_pl1, wh_pl0, wh_pl1, for_pl0, for_pl1, for_pl2, for_pl3;
    if (c_type == LEX_IF) {
        // if ( Expr ) Operat else Operat
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            eq_bool();
            if_pl0 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                Operat();
                if_pl1 = poliz.size ();
                poliz.push_back(Lex());
                poliz.push_back(Lex(POLIZ_GO));
                poliz[if_pl0] = Lex(POLIZ_LABEL,poliz.size());
                if (c_type == LEX_ELSE) {
                    gl();
                    Operat();
                    poliz[if_pl1] = Lex(POLIZ_LABEL, poliz.size());
                } else {
                    throw curr_lex;
                }
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_FOR) {
        STBREAK.push(vector<int>());
        // for ( /[ Expr /] ; /[ Expr /] ; /[ Expr /] ) Operat
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            if ((c_type == LEX_ID) || (c_type == LEX_NOT) ||
                (c_type == LEX_MINUS) || (c_type == LEX_NUM) ||
                (c_type == LEX_STR) || (c_type == LEX_FALSE) ||
                (c_type == LEX_TRUE) || (c_type == LEX_LPAREN)) {
                Expr();
                st_lex.pop();
                poliz.push_back(Lex(LEX_SEMICOLON));
            }
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw curr_lex;
            }

            for_pl0 = poliz.size();
            if ((c_type == LEX_ID) || (c_type == LEX_NOT) ||
                (c_type == LEX_MINUS) || (c_type == LEX_NUM) ||
                (c_type == LEX_STR) || (c_type == LEX_FALSE) ||
                (c_type == LEX_TRUE) || (c_type == LEX_LPAREN)) {
                Expr();
                eq_bool();
            }
            for_pl1 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_FGO));

            for_pl2 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_GO));
            for_pl3 = poliz.size();
            if (c_type == LEX_SEMICOLON) {
                gl();
            } else {
                throw curr_lex;
            }
            if ((c_type == LEX_ID) || (c_type == LEX_NOT) ||
                (c_type == LEX_MINUS) || (c_type == LEX_NUM) ||
                (c_type == LEX_STR) || (c_type == LEX_FALSE) ||
                (c_type == LEX_TRUE) || (c_type == LEX_LPAREN)) {
                Expr();
                st_lex.pop();
                poliz.push_back(Lex(LEX_SEMICOLON));
            }
            poliz.push_back(Lex(POLIZ_LABEL, for_pl0));
            poliz.push_back(Lex(POLIZ_GO));
            if (c_type == LEX_RPAREN) {
                gl();
            } else {
                throw curr_lex;
            }
            poliz[for_pl2] = Lex(POLIZ_LABEL, poliz.size());
            Operat();
            poliz.push_back(Lex(POLIZ_LABEL, for_pl3));
            poliz.push_back(Lex(POLIZ_GO));
            poliz[for_pl1] = Lex(POLIZ_LABEL, poliz.size());
            while (!STBREAK.top().empty()) {
                poliz[STBREAK.top().back()] = Lex(POLIZ_LABEL, poliz.size());
                STBREAK.top().pop_back();
            }
            STBREAK.pop();
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_WHILE) {
        // while ( Expr ) Operat
        wh_pl0 = poliz.size();
        STBREAK.push(vector<int>());
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            eq_bool();
            wh_pl1 = poliz.size();
            poliz.push_back(Lex());
            poliz.push_back(Lex(POLIZ_FGO));
            if (c_type == LEX_RPAREN) {
                gl();
                Operat();
                poliz.push_back(Lex(POLIZ_LABEL, wh_pl0));
                poliz.push_back(Lex(POLIZ_GO));
                poliz[wh_pl1] = Lex(POLIZ_LABEL, poliz.size());
                while (!STBREAK.top().empty()) {
                    poliz[STBREAK.top().back()] = Lex(POLIZ_LABEL, poliz.size());
                    STBREAK.top().pop_back();
                }
                STBREAK.pop();
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_BREAK) {
        // break ;
        if (STBREAK.size() == 0) {
            throw "break occurred out of cycle";
        }
        STBREAK.top().push_back(poliz.size());
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_GO));
        gl();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_GOTO) {
        gl();
        if (c_type == LEX_ID) {
            goto_label();
            gl();
        } else {
            throw curr_lex;
        }
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_READ) {
        // read ( ident );
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
        } else {
            throw curr_lex;
        }
        if (c_type == LEX_ID) {
            check_id_in_read();
        } else {
            throw curr_lex;
        }
        if (c_type == LEX_RPAREN) {
            gl();
        } else {
            throw curr_lex;
        }
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_WRITE) {
        // write ( Expr /{ , Expr /} ) ;
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
        } else {
            throw curr_lex;
        }
        Expr();
        if (st_lex.top().first == LEX_STRING) {
            poliz.push_back(Lex(LEX_SWRITE));
        } else {
            poliz.push_back(Lex(LEX_WRITE));
        }
        st_lex.pop();
        while (c_type == LEX_COMMA) {
            gl();
            Expr();
            if (st_lex.top().first == LEX_STRING) {
                poliz.push_back(Lex(LEX_SWRITE));
            } else {
                poliz.push_back(Lex(LEX_WRITE));
            }
            st_lex.pop();
        }
        if (c_type == LEX_RPAREN) {
            gl();
        } else {
            throw curr_lex;
        }
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_LBRACE) {
        // CompOperat
        CompOperat();
    } else if (c_type == LEX_ID) {
        Lex lex1 = curr_lex;
        gl();
        if (c_type == LEX_COLON) {
            ungl(curr_lex);
            ungl(lex1);
            gl();
            TaggedOperat();
        } else {
            ungl(curr_lex);
            ungl(lex1);
            gl();
            ExprOperat();
        }
    } else if ((c_type == LEX_NOT) || (c_type == LEX_MINUS) ||
               (c_type == LEX_NUM) || (c_type == LEX_STR) ||
               (c_type == LEX_FALSE) || (c_type == LEX_TRUE) ||
               (c_type == LEX_LPAREN)) {
        ExprOperat();
    } else {
        throw curr_lex;
    }
}

// ident : operat
void Parser::TaggedOperat() {
    if (c_type != LEX_ID) {
        throw curr_lex;
    }
    dec_label(c_val);
    gl();
    if (c_type == LEX_COLON) {
        gl();
        Operat();
    } else {
        throw curr_lex;
    }
}

// CompOperat -> { Operats }
void Parser::CompOperat() {
    if (c_type == LEX_LBRACE) {
        gl();
    } else {
        throw curr_lex;
    }
    Operats();
    if (c_type == LEX_RBRACE) {
        gl();
    } else {
        throw curr_lex;
    }
}

// ExprOperat -> Expr ;
void Parser::ExprOperat() {
    Expr();
    st_lex.pop();
    poliz.push_back(Lex(LEX_SEMICOLON));
    if (c_type == LEX_SEMICOLON) {
        gl();
    } else {
        throw curr_lex;
    }
}

// Expr -> Assign
void Parser::Expr() {
//    cout << "Expr " << curr_lex << endl;
    Assign();
}

// Assign -> ident = Assign | Or
void Parser::Assign() {
//    cout << "Assign " << curr_lex << endl;
    if (c_type == LEX_ID) {
        Lex lex1 = curr_lex, lex2, lex3;
        gl();
        if (c_type == LEX_ASSIGN) {
            ungl(curr_lex);
            ungl(lex1);
            gl();
            Id(true, true);
            gl();
            Assign();
            eq_type();
        } else {
            if (c_type == LEX_POINT) {
                lex2 = curr_lex;
                gl();
                if (c_type == LEX_ID) {
                    lex3 = curr_lex;
                    gl();
                } else {
                    throw curr_lex;
                }
                if (c_type == LEX_ASSIGN) {
                    ungl(curr_lex);
                    ungl(lex3);
                    ungl(lex2);
                    ungl(lex1);
                    gl();
                    Id(true, true);
                    gl();
                    Assign();
                    eq_type();
                } else {
                    ungl(curr_lex);
                    ungl(lex3);
                    ungl(lex2);
                    ungl(lex1);
                    gl();
                    Or();
                }

            } else {
                ungl(curr_lex);
                ungl(lex1);
                gl();
                Or();
            }
        }
    } else {
        Or();
    }
}

// Or -> And {or And}
void Parser::Or() {
//    cout << "Or " << curr_lex << endl;
    And();
    while (c_type == LEX_OR) {
        st_lex.push(pair<type_of_lex, int>(c_type, -1));
        gl();
        And();
        check_op();
    }
}

// And -> Comp {and Comp}
void Parser::And() {
//    cout << "And " << curr_lex << endl;
    Comp();
    while (c_type == LEX_AND) {
        st_lex.push(pair<type_of_lex, int>(c_type, -1));
        gl();
        Comp();
        check_op();
    }
}

// Comp -> Ar_expr [<>= Ar_expr]
void Parser::Comp() {
//    cout << "Comp " << curr_lex << endl;
    Ar_expr();
    if (c_type == LEX_EQ || c_type == LEX_NEQ || c_type == LEX_LSS ||
        c_type == LEX_GTR || c_type == LEX_LEQ || c_type == LEX_GEQ) {
        st_lex.push(pair<type_of_lex, int>(c_type, -1));
        gl ();
        Ar_expr();
        check_op();
    }
}

// Ar_expr -> Term {+- Term}
void Parser::Ar_expr() {
//    cout << "Ar_expr " << curr_lex << endl;
    Term();
    while (c_type == LEX_MINUS || c_type == LEX_PLUS) {
        st_lex.push(pair<type_of_lex, int>(c_type, -1));
        gl();
        Term();
        check_op();
    }
}

// Term -> Factor {* / Factor}
void Parser::Term() {
//    cout << "Term " << curr_lex << endl;
    Factor();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH) {
        st_lex.push(pair<type_of_lex, int>(c_type, -1));
        gl();
        Factor();
        check_op();
    }
}

// Factor -> [not, -] Factor | const | ID | (Assign)
void Parser::Factor() {
//    cout << "Factor " << curr_lex << endl;
    if (c_type == LEX_NOT) {
        gl();
        Factor();
        check_not();
    } else if (c_type == LEX_MINUS) {
        gl();
        poliz.push_back(Lex(LEX_NUM, 0));
        Factor();
        check_minus();
    } else if (c_type == LEX_NUM) {
        st_lex.push(pair<type_of_lex, int>(LEX_INT, -1));
        poliz.push_back (curr_lex);
        gl();
    } else if (c_type == LEX_STR) {
        st_lex.push(pair<type_of_lex, int>(LEX_STRING, -1));
        poliz.push_back (curr_lex);
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(pair<type_of_lex, int>(LEX_BOOL, -1));
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(pair<type_of_lex, int>(LEX_BOOL, -1));
        poliz.push_back(Lex(LEX_TRUE, 1));
        gl();
    } else if (c_type == LEX_ID) {
        Id(false, true);
    } else if (c_type == LEX_LPAREN) {
        gl();
        Assign();
        if (c_type == LEX_RPAREN) {
            gl();
        } else {
            throw curr_lex;
        }
    } else {
        throw curr_lex;
    }
}

void Parser::Id(bool address, bool push) {
//    cout << "Ident " << curr_lex << endl;
    type_of_lex type = LEX_ID;
    if (address) {
        type = POLIZ_ADDRESS;
    }
    if (c_type != LEX_ID) {
        throw curr_lex;
    }
//    poliz.push_back(Lex(LEX_ID, c_val));
    if (TID[c_val].get_type() == LEX_STRUCT_TYPE) {
        throw curr_lex;
    }
    if (TID[c_val].get_type() != LEX_STRUCT) {
        check_id(TID[c_val], push);
        poliz.push_back(Lex(type, c_val));
    } else {
        int struct_idx = TID[c_val].get_value();
        int id_idx = c_val;
        Lex lex1 = curr_lex;
        gl();
        if (c_type == LEX_POINT) {
            gl();
            if (c_type != LEX_ID) {
                throw curr_lex;
            }
            auto iter = find(TSTRUCT[struct_idx].tid.begin(), TSTRUCT[struct_idx].tid.end(),
                                                TID[c_val].get_name());
            if (iter != TSTRUCT[struct_idx].tid.end()) {
                check_id(*iter, push);
                poliz.push_back(Lex(type, iter - TSTRUCT[struct_idx].tid.begin(), struct_idx));
            } else {
                throw curr_lex;
            }
        } else {
            ungl(curr_lex);
            ungl(lex1);
            gl();
            check_id(TID[c_val], push);
            poliz.push_back(Lex(type, id_idx));
        }
    }
    gl();
}

void Parser::dec(type_of_lex type, int struct_type_idx, int id_idx) {
    if (TID[id_idx].get_declare())
        throw "twice";
    else {
        TID[id_idx].put_declare();
        if (type == LEX_STRING) {
            TID[id_idx].put_value(TSTR.size());
            TSTR.push_back(string());
        } else if (type == LEX_STRUCT) {
            TID[id_idx].put_value(TSTRUCT.size());
            TID[id_idx].put_struct_type(struct_type_idx);
            TSTRUCT.push_back(TTYPE[struct_type_idx]);
            TSTRUCT.back().name = TID[id_idx].get_name();
            TID[id_idx].put_assign();
            init_str_vars(TID[id_idx].get_value());
        }
        TID[id_idx].put_type(type);
    }
}

void Parser::check_id(Ident ident, bool push) {
    if (ident.get_declare()) {
        if (push) {
            if (ident.get_type() != LEX_STRUCT) {
                st_lex.push(pair<type_of_lex, int>(ident.get_type(), -1));
            } else {
                st_lex.push(pair<type_of_lex, int>(LEX_STRUCT, ident.get_struct_type()));
            }
        }
    } else {
        throw "not declared";
    }
}

void Parser::check_op() {
    pair<type_of_lex, int> t1, t2, op_pair;

    from_st(st_lex, t2);
    from_st(st_lex, op_pair);
    from_st(st_lex, t1);
    type_of_lex op = op_pair.first;

    if (t1 == t2 && t1.first == LEX_INT) {
        if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES ||
            op == LEX_SLASH) {
            st_lex.push(pair<type_of_lex, int>(LEX_INT, -1));
        } else if (op == LEX_EQ || op == LEX_NEQ || op == LEX_LSS ||
                op == LEX_GTR || op == LEX_LEQ || op == LEX_GEQ) {
            st_lex.push(pair<type_of_lex, int>(LEX_BOOL, -1));
        } else {
            throw "wrong types are in operation";
        }
    } else if (t1 == t2 && t1.first == LEX_STRING) {
        if (op == LEX_PLUS) {
            st_lex.push(pair<type_of_lex, int>(LEX_STRING, -1));
            op = LEX_SPLUS;
        } else if (op == LEX_EQ || op == LEX_NEQ ||
                op == LEX_LSS || op == LEX_GTR) {
            st_lex.push(pair<type_of_lex, int>(LEX_BOOL, -1));
            if (op == LEX_EQ) {
                op = LEX_SEQ;
            } else if (op == LEX_NEQ) {
                op = LEX_SNEQ;
            } else if (op == LEX_LSS) {
                op = LEX_SLSS;
            } if (op == LEX_GTR) {
                op = LEX_SGTR;
            }
        } else {
            throw "wrong types are in operation";
        }
    } else if (t1 == t2 && t1.first == LEX_BOOL) {
            if (op == LEX_OR || op == LEX_AND) {
                st_lex.push(pair<type_of_lex, int>(LEX_BOOL, -1));
            } else {
                throw "wrong types are in operation";
            }
    } else {
        throw "wrong types are in operation";
    }
    poliz.push_back(Lex(op));
}

void Parser::check_not() {
    if (st_lex.top().first != LEX_BOOL) {
        throw "wrong type is in not";
    } else {
        poliz.push_back(Lex(LEX_NOT));
    }
}

void Parser::check_minus() {
    if (st_lex.top().first != LEX_INT) {
        throw "wrong type is in minus";
    } else {
        poliz.push_back(Lex(LEX_MINUS));
    }
}

void Parser::eq_type() {
    pair<type_of_lex, int> t;
    from_st(st_lex, t);
    if (t != st_lex.top()) {
        throw "wrong types are in :=";
    }
    if (t.first == LEX_STRING) {
        poliz.push_back(Lex(LEX_SASSIGN));
    } else if (t.first == LEX_STRUCT) {
        poliz.push_back(Lex(LEX_STRASSIGN));
    } else {
        poliz.push_back(Lex(LEX_ASSIGN));
    }
}

void Parser::eq_bool() {
    if (st_lex.top().first != LEX_BOOL) {
        throw "expression is not boolean";
    }
    st_lex.pop();
}

void Parser::check_id_in_read() {
    Id(true, false);
    poliz.push_back(Lex(LEX_READ));
}

void Parser::dec_label(int id_idx) {
    if (TID[id_idx].get_declare()) {
        if (TID[id_idx].get_type() != LEX_GOTO) {
            throw "variable with this name already exists";
        }
        if (TID[id_idx].get_assign()) {
            throw "two lables one name";
        } else {
            while (!TGOTO[TID[id_idx].get_value()].empty()) {
                poliz[TGOTO[TID[id_idx].get_value()].back()] =
                        Lex(POLIZ_LABEL, poliz.size());
                TGOTO[TID[id_idx].get_value()].pop_back();
            }
            TID[id_idx].put_value(poliz.size());
            TID[id_idx].put_assign();
        }
    } else {
        TID[id_idx].put_declare();
        TID[id_idx].put_assign();
        TID[id_idx].put_type(LEX_GOTO);
        TID[id_idx].put_value(poliz.size());
    }
}

void Parser::goto_label() {
    if (TID[c_val].get_declare()) {
        if (TID[c_val].get_type() == LEX_GOTO) {
            if (TID[c_val].get_assign()) {
                poliz.push_back(Lex(POLIZ_LABEL, TID[c_val].get_value()));
                poliz.push_back(Lex(POLIZ_GO));
            } else {
                TGOTO[TID[c_val].get_value()].push_back(poliz.size());
                poliz.push_back(Lex());
                poliz.push_back(Lex(POLIZ_GO));
            }
        } else {
            throw "expected label";
        }
    } else {
        TID[c_val].put_declare();
        TID[c_val].put_type(LEX_GOTO);
        TID[c_val].put_value(TGOTO.size());
        TGOTO.push_back(vector<int>());
        TGOTO[TID[c_val].get_value()].push_back(poliz.size());
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_GO));
    }
}

void Parser::dec_struct_type(int id_idx) {
    if (TID[id_idx].get_declare())
        throw "twice";
    else {
        TID[id_idx].put_declare();
        TID[id_idx].put_type(LEX_STRUCT_TYPE);
        TID[id_idx].put_value(TTYPE.size());
        TTYPE.push_back(Struct());
        TTYPE.back().name = TID[id_idx].get_name();
    }
}

int Parser::dec_struct_var(type_of_lex type, int struct_type_idx) {
    if (find(TTYPE[struct_type_idx].tid.begin(), TTYPE[struct_type_idx].tid.end(), TID[c_val].get_name())
            == TTYPE[struct_type_idx].tid.end()) {
        int id_idx = TTYPE[struct_type_idx].tid.size();
        TTYPE[struct_type_idx].tid.push_back(Ident(TID[c_val].get_name()));
        TTYPE[struct_type_idx].tid.back().put_declare();
        TTYPE[struct_type_idx].tid.back().put_type(type);
        return id_idx;
    } else {
        throw "twice";
    }
}

void Parser::init_str_vars(int struct_idx) {
    auto iter = TSTRUCT[struct_idx].tid.begin();
    while (iter != TSTRUCT[struct_idx].tid.end()) {
        if (iter->get_type() == LEX_STRING) {
            iter->put_value(TSTR.size());
            TSTR.push_back(string());
        }
        iter += 1;
    }
}

void Parser::check_labels() {
    for (auto iter = TGOTO.begin(); iter != TGOTO.end(); iter++ ) {
        if (!iter->empty()) {
            throw "label used but not assigned";
        }
    }
}


#endif //MLANG_INTERPRETER_PARSER_H