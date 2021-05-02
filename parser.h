#ifndef PRAK4_PARSER_H
#define PRAK4_PARSER_H

#include "stack"
#include "vector"

template <class T, class T_EL>
void from_st (T& st, T_EL& i) {
    i = st.top();
    st.pop();
}

vector<vector<int>> TGOTO;
stack<vector<int>> TBREAK;

class Parser {
    Lex         curr_lex;
    type_of_lex c_type;
    int         c_val;
    Scanner     scan;
    stack<int>          st_int;
    stack<type_of_lex>  st_lex;
    int         cycle_depth;

    void Program();
    void Declars();
    void Declar();
    void Operats();
    void Operat();
    void CompOperat();
    void TaggedOperat();
    void ExprOperat();
    type_of_lex Type();
    void Var(type_of_lex type);
    void Const(type_of_lex type, int id_idx);
    void Expr();
    void Assign();
    void Or();
    void And();
    void Comp();
    void Ar_expr();
    void Term();
    void Factor();

    void dec(type_of_lex type, int id_idx);
    void check_id();
    void check_op();
    void check_not();
    void check_minus();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void dec_label(int id_idx);
    void goto_label();

    void gl() {
        curr_lex  = scan.get_lex();
        c_type    = curr_lex.get_type();
        c_val     = curr_lex.get_value();
    }
    void ungl(Lex& lex) {
        scan.unget_lex(lex);
    }

public:
    vector<Lex> poliz;
    explicit Parser(const char* program) : scan(program), cycle_depth(0) {}
    void analyze();
};

void Parser::analyze() {
    gl();
    Program();
    if (c_type != LEX_FIN) {
        throw curr_lex;
    }
    for (Lex l : poliz)
        cout << l;
    cout << endl << "Yes!!!" << endl;
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
    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
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
    type_of_lex type = Type();
    Var(type);
    while (c_type == LEX_COMMA) {
        gl();
        Var(type);
    }
}

//  Type -> int | string | boolean
type_of_lex Parser::Type() {
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
        type_of_lex type = c_type;
        gl();
        return type;
    } else {
        throw curr_lex;
    }
}

// Var -> ident | ident = const
void Parser::Var(type_of_lex type) {
    if (c_type == LEX_ID) {
        dec(type, c_val);
    } else {
        throw curr_lex;
    }
    int id_idx = c_val;
    gl();
    if (c_type == LEX_ASSIGN) {
        gl();
        Const(type, id_idx);
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
           (c_type == LEX_FALSE) || (c_type == LEX_TRUE) || (c_type == LEX_LPAREN)) {
        Operat();
    }
}

void Parser::Operat() {
    cout << "Operat " << curr_lex << endl;
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
        cycle_depth += 1;
        TBREAK.push(vector<int>());
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
            while (!TBREAK.top().empty()) {
                poliz[TBREAK.top().back()] = Lex(POLIZ_LABEL, poliz.size());
                TBREAK.top().pop_back();
            }
            TBREAK.pop();
        } else {
            throw curr_lex;
        }
        cycle_depth -= 1;
    } else if (c_type == LEX_WHILE) {
        // while ( Expr ) Operat
        wh_pl0 = poliz.size();
        cycle_depth += 1;
        TBREAK.push(vector<int>());
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
                while (!TBREAK.top().empty()) {
                    poliz[TBREAK.top().back()] = Lex(POLIZ_LABEL, poliz.size());
                    TBREAK.top().pop_back();
                }
                TBREAK.pop();
            } else {
                throw curr_lex;
            }
        } else {
            throw curr_lex;
        }
        cycle_depth -= 1;
    } else if (c_type == LEX_BREAK) {
        // break ;
        if (cycle_depth == 0) {
            throw "break out of cycle";
        }
        TBREAK.top().push_back(poliz.size());
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
            gl();
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
        st_lex.pop();
        if (poliz.back().get_type() == LEX_STRING) {
            poliz.push_back(Lex(LEX_SWRITE));
        } else {
            poliz.push_back(Lex(LEX_WRITE));
        }
        while (c_type == LEX_COMMA) {
            gl();
            Expr();
            st_lex.pop();
            poliz.push_back(Lex(LEX_WRITE));
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
            int id_val = lex1.get_value();
            dec_label(id_val);
            gl();
            Operat();
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
    cout << "Expr " << curr_lex << endl;
    Assign();
}

// Assign -> ident = Assign | Or
void Parser::Assign() {
    cout << "Assign " << curr_lex << endl;
    if (c_type == LEX_ID) {
        Lex lex1 = curr_lex;
        gl();
        if (c_type == LEX_ASSIGN) {
//            int temp_c_val = curr_lex.get_value();
            int temp_c_val = c_val;
            c_val = lex1.get_value();
            check_id();
            poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
            c_val = temp_c_val;
            gl();
            Assign();
            eq_type();
        } else {
            ungl(curr_lex);
            ungl(lex1);
            gl();
            Or();
        }
    } else {
        Or();
    }
}

// Or -> And {or And}
void Parser::Or() {
    cout << "Or " << curr_lex << endl;
    And();
    while (c_type == LEX_OR) {
        st_lex.push(c_type);
        gl();
        And();
        check_op();
    }
}

// And -> Comp {and Comp}
void Parser::And() {
    cout << "And " << curr_lex << endl;
    Comp();
    while (c_type == LEX_AND) {
        st_lex.push(c_type);
        gl();
        Comp();
        check_op();
    }
}

// Comp -> Ar_expr [<>= Ar_expr]
void Parser::Comp() {
    cout << "Comp " << curr_lex << endl;
    Ar_expr();
    if (c_type == LEX_EQ || c_type == LEX_NEQ || c_type == LEX_LSS ||
        c_type == LEX_GTR || c_type == LEX_LEQ || c_type == LEX_GEQ) {
        st_lex.push(c_type);
        gl ();
        Ar_expr();
        check_op();
    }
}

// Ar_expr -> Term {+- Term}
void Parser::Ar_expr() {
    cout << "Ar_expr " << curr_lex << endl;
    Term();
    while (c_type == LEX_MINUS || c_type == LEX_PLUS) {
        st_lex.push(c_type);
        gl();
        Term();
        check_op();
    }
}

// Term -> Factor {* / Factor}
void Parser::Term() {
    cout << "Term " << curr_lex << endl;
    Factor();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH) {
        st_lex.push(c_type);
        gl();
        Factor();
        check_op();
    }
}

// Factor -> [not, -] Factor | const | ID | (Assign)
void Parser::Factor() {
    cout << "Factor " << curr_lex << endl;
    type_of_lex type;
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
        st_lex.push(LEX_INT);
        poliz.push_back (curr_lex);
        gl();
    } else if (c_type == LEX_STR) {
        st_lex.push(LEX_STRING);
        poliz.push_back (curr_lex);
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_FALSE, 0));
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        poliz.push_back(Lex(LEX_TRUE, 1));
        gl();
    } else if (c_type == LEX_ID) {
        check_id();
        poliz.push_back(Lex(LEX_ID, c_val));
        type = TID[c_val].get_type();
        gl();
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

void Parser::dec(type_of_lex type, int id_idx) {
    if (TID[id_idx].get_declare())
        throw "twice";
    else {
        TID[id_idx].put_declare();
        if (type == LEX_STRING) {
            TID[id_idx].put_value(TSTR.size());
            TSTR.push_back(string());
        }
        TID[id_idx].put_type(type);
    }
}

void Parser::check_id() {
    if (TID[c_val].get_declare()) {
        st_lex.push(TID[c_val].get_type());
    } else {
        throw "not declared";
    }
}

void Parser::check_op() {
    type_of_lex t1, t2, op;

    from_st(st_lex, t2);
    from_st(st_lex, op);
    from_st(st_lex, t1);

    if (t1 == t2 && t1 == LEX_INT) {
        if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES ||
            op == LEX_SLASH) {
            st_lex.push(LEX_INT);
        } else if (op == LEX_EQ || op == LEX_NEQ || op == LEX_LSS ||
                op == LEX_GTR || op == LEX_LEQ || op == LEX_GEQ) {
            st_lex.push(LEX_BOOL);
        } else {
            throw "wrong types are in operation";
        }
    } else if (t1 == t2 && t1 == LEX_STRING) {
        if (op == LEX_PLUS) {
            st_lex.push(LEX_STRING);
            op = LEX_SPLUS;
        } else if (op == LEX_EQ || op == LEX_NEQ ||
                op == LEX_LSS || op == LEX_GTR) {
            st_lex.push(LEX_BOOL);
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
    } else if (t1 == t2 && t1 == LEX_BOOL) {
            if (op == LEX_OR || op == LEX_AND) {
                st_lex.push(LEX_BOOL);
            } else {
                throw "wrong types are in operation";
            }
    } else {
        throw "wrong types are in operation";
    }
    poliz.push_back(Lex(op));
}

void Parser::check_not() {
    if (st_lex.top() != LEX_BOOL) {
        throw "wrong type is in not";
    } else {
        poliz.push_back(Lex(LEX_NOT));
    }
}

void Parser::check_minus() {
    if (st_lex.top() != LEX_INT) {
        throw "wrong type is in minus";
    } else {
        poliz.push_back(Lex(LEX_MINUS));
    }
}

void Parser::eq_type() {
    type_of_lex t;
    from_st(st_lex, t);
    if (t != st_lex.top()) {
        throw "wrong types are in :=";
    }
    if (t == LEX_STRING) {
        poliz.push_back(Lex(LEX_SASSIGN));
    } else {
        poliz.push_back(Lex(LEX_ASSIGN));
    }
}

void Parser::eq_bool() {
    if (st_lex.top() != LEX_BOOL) {
        throw "expression is not boolean";
    }
    st_lex.pop();
}

void Parser::check_id_in_read() {
    if (!TID[c_val].get_declare()) {
        throw "not declared";
    }
    poliz.push_back(Lex(POLIZ_ADDRESS, c_val));
}

void Parser::dec_label(int id_idx) {
    if (TID[id_idx].get_declare())
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
    else {
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
        TID[c_val].put_value(TGOTO.size());
        TGOTO.push_back(vector<int>());
        TGOTO[TID[c_val].get_value()].push_back(poliz.size());
        poliz.push_back(Lex());
        poliz.push_back(Lex(POLIZ_GO));
    }
}

#endif //PRAK4_PARSER_H
