#ifndef PRAK4_PARSER_H
#define PRAK4_PARSER_H

#include "stack"
#include "vector"

template <class T, class T_EL>
void from_st (T& st, T_EL& i) {
    i = st.top();
    st.pop();
}

//class StructIdent {
//public:
//    string name;
//    vector<Ident> tid;
//};
//
//class StructType {
//public:
//    string name;
//    vector<Ident> tid;
//    vector<StructIdent> members;
//
//};
//
//vector<StructType> TSTRUCT;

class Parser {
    Lex         curr_lex;
    type_of_lex c_type;
    int         c_val;
    Scanner     scan;
    stack<int>          st_int;
//    stack<int>  st_lex;
    stack<type_of_lex>  st_lex;

    int         cycle_depth;

    void Program();
//    void StructDeclars();
//    void StructDeclar();
//    void StructVarDeclar(StructType& struct_type);
//    int StructVarType();
//    void StructVar(int type_val, StructType struct_type);
    void Declars();
    void Declar();
    void Operats();
    void Operat();
    void CompOperat();
    void TaggedOperat();
    void ExprOperat();
//    int Type(bool& in_struct);
//    void Var(int type_val, bool is_struct = false);
//    void Const(int id_type, int id_val);
    type_of_lex Type();
    void Var(type_of_lex type);
    void Const(type_of_lex type, int id_val);
    void Expr();
    void Assign();
    void Or();
    void And();
    void Comp();
    void Ar_expr();
    void Term();
    void Factor();
//    void Ident(int name_space = 0);

//    void dec(int type_val, int id_val, bool is_struct);
    void dec(type_of_lex type, int id_val);
    void check_id();
    void check_op();
    void check_not();
    void check_minus();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
//    void check_not_struct();
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
    // for (Lex l : poliz) cout << l;
    cout << endl << "Yes!!!" << endl;
}

//// StructDeclars -> /{ StructDeclar /}
//void Parser::StructDeclars() {
//    while (c_type == LEX_STRUCT) {
//        StructDeclar();
//    }
//}
//
//// StructDeclar -> struct ident { Declar ; /{ Declar ; /} };
//void Parser::StructDeclar() {
//    if (c_type == LEX_STRUCT) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//    if (c_type == LEX_ID) {
//        dec(LEX_STRUCT, c_val, 0);
//    } else {
//        throw curr_lex;
//    }
//    int struct_type_idx = TSTRUCT.size();
//    TID[c_val].put_value(struct_type_idx);
//    TSTRUCT.push_back(StructType());
//    gl();
//    if (c_type == LEX_LBRACE) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//    // Declar(struct_id);
//    StructVarDeclar(TSTRUCT[struct_type_idx]);
//    if (c_type == LEX_SEMICOLON) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//    while (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
//        StructVarDeclar(TSTRUCT[struct_type_idx]);
//        if (c_type == LEX_SEMICOLON) {
//            gl();
//        } else {
//            throw curr_lex;
//        }
//    }
//    if (c_type == LEX_RBRACE) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//}
//
//void Parser::StructVarDeclar(StructType& struct_type) {
//    int type_val = StructVarType();
//    StructVar(type_val, struct_type);
//    while (c_type == LEX_COMMA) {
//        gl();
//        Var(type_val);
//    }
//};
//
//
////  Type -> int | string | boolean
//int Parser::StructVarType() {
//    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOL) {
//        return c_type;
//    } else {
//        throw curr_lex;
//    }
//}
//
//// Var -> ident | ident = const
//void Parser::StructVar(int type_val, StructType struct_type) {
//    if (c_type == LEX_ID) {
//        StructDec(type_val, struct_type);
//    } else {
//        throw curr_lex;
//    }
//    int id_val = c_val;
//    gl();
//    if (c_type == LEX_ASSIGN) {
//        gl();
//        Const(type_val, id_val);
//    }
//}
//
//// Const -> const
//void Parser::Const(int type_val, int id_val) {
//    if (c_type == LEX_NUM) {
//        if (type_val != LEX_INT) {
//            cout << "?" << endl;
//            throw curr_lex;
//        }
//        TID[id_val].put_value(c_val);
//        gl();
//    } else if (c_type == LEX_STR) {
//        if (type_val != LEX_STRING) {
//            throw curr_lex;
//        }
//        TID[id_val].put_value(c_val);
//        gl();
//    } else if ((c_type == LEX_FALSE) || (c_type == LEX_TRUE)) {
//        if (type_val != LEX_BOOL) {
//            throw curr_lex;
//        }
//        TID[id_val].put_value(c_type == LEX_TRUE ? 1 : 0);
//        gl();
//    } else {
//        throw curr_lex;
//    }
//}
//
//void Parser::StructDec(int type_val, StructType& struct_type) {
//    int id_idx = struct_type.tid.size();
//    struct_type.tid.push_back(Ident())
//}

//// Program -> program { StructDeclars Declars Operats }
//void Parser::Program() {
//    if (c_type == LEX_PROGRAM) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//    if (c_type == LEX_LBRACE) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//    StructDeclars();
//    Declars();
//    Operats();
//    if (c_type == LEX_RBRACE) {
//        gl();
//    } else {
//        throw curr_lex;
//    }
//}

// Program -> program { StructDeclars Declars Operats }
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

//// Type Var \{ , Var /}
//void Parser::Declar() {
//    bool is_struct;
//    int type_val = Type(is_struct);
//    Var(type_val, is_struct);
//    while (c_type == LEX_COMMA) {
//        gl();
//        Var(type_val, is_struct);
//    }
//}

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

//// Var -> ident | ident = const
//void Parser::Var(int type_val, bool is_struct) {
//    if (c_type == LEX_ID) {
//        dec(type_val, c_val, is_struct);
//    } else {
//        throw curr_lex;
//    }
//    int id_val = c_val;
//    gl();
//    if (c_type == LEX_ASSIGN) {
//        if (is_struct) {
//            throw curr_lex;
//        }
//        gl();
//        Const(type_val, id_val);
//    }
//}

// Var -> ident | ident = const
void Parser::Var(type_of_lex type) {
    if (c_type == LEX_ID) {
        dec(type, c_val);
    } else {
        throw curr_lex;
    }
    int id_val = c_val;
    gl();
    if (c_type == LEX_ASSIGN) {
        gl();
        Const(type, id_val);
    }
}


//// Const -> const
//void Parser::Const(int type_val, int id_val) {
//    if (c_type == LEX_NUM) {
//        if (type_val != LEX_INT) {
//            cout << "?" << endl;
//            throw curr_lex;
//        }
//        TID[id_val].put_value(c_val);
//        gl();
//    } else if (c_type == LEX_STR) {
//        if (type_val != LEX_STRING) {
//            throw curr_lex;
//        }
//        TID[id_val].put_value(c_val);
//        gl();
//    } else if ((c_type == LEX_FALSE) || (c_type == LEX_TRUE)) {
//        if (type_val != LEX_BOOL) {
//            throw curr_lex;
//        }
//        TID[id_val].put_value(c_type == LEX_TRUE ? 1 : 0);
//        gl();
//    } else {
//        throw curr_lex;
//    }
//}

// Const -> const
void Parser::Const(type_of_lex type, int id_val) {
    if (c_type == LEX_NUM) {
        if (type != LEX_INT) {
            cout << "?" << endl;
            throw curr_lex;
        }
        TID[id_val].put_value(c_val);
        gl();
    } else if (c_type == LEX_STR) {
        if (type != LEX_STRING) {
            throw curr_lex;
        }
        TID[id_val].put_value(c_val);
        gl();
    } else if ((c_type == LEX_FALSE) || (c_type == LEX_TRUE)) {
        if (type != LEX_BOOL) {
            throw curr_lex;
        }
        TID[id_val].put_value(c_type == LEX_TRUE ? 1 : 0);
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
    cout << "Operat" << endl;
    if (c_type == LEX_IF) {
        // if ( Expr ) Operat else Operat
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            eq_bool();
            if (c_type == LEX_RPAREN) {
                gl();
                Operat();
                if (c_type == LEX_ELSE) {
                    gl();
                    Operat();
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
            }
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
                eq_bool();
            }
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
            }
            if (c_type == LEX_RPAREN) {
                gl();
            } else {
                throw curr_lex;
            }
            Operat();
        } else {
            throw curr_lex;
        }
        cycle_depth -= 1;
    } else if (c_type == LEX_WHILE) {
        cycle_depth += 1;
        // while ( Expr ) Operat
        gl();
        if (c_type == LEX_LPAREN) {
            gl();
            Expr();
            eq_bool();
            if (c_type == LEX_RPAREN) {
                gl();
                Operat();
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
        gl();
        if (c_type == LEX_SEMICOLON) {
            gl();
        } else {
            throw curr_lex;
        }
    } else if (c_type == LEX_GOTO) {
        // goto ident ;
        gl();
        if (c_type == LEX_ID) {
            check_id();
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
        while (c_type == LEX_COMMA) {
            gl();
            Expr();
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
            int id_val = lex1.get_value();
            dec(LEX_GOTO, id_val);
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
//    check_not_struct();
}

// Assign -> ident = Assign | Or
void Parser::Assign() {
    cout << "Assign " << curr_lex << endl;
    if (c_type == LEX_ID) {
        Lex lex1 = curr_lex;
        gl();
        if (c_type == LEX_ASSIGN) {
            int temp_c_val = curr_lex.get_value();
            c_val = lex1.get_value();
            check_id();
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
    if (c_type == LEX_NOT) {
        gl();
        Factor();
        check_not();
    } else if (c_type == LEX_MINUS) {
        gl();
        Factor();
        check_minus();
    } else if (c_type == LEX_NUM) {
        st_lex.push(LEX_INT);
        gl();
    } else if (c_type == LEX_STR) {
        st_lex.push(LEX_STRING);
        gl();
    } else if (c_type == LEX_FALSE) {
        st_lex.push(LEX_BOOL);
        gl();
    } else if (c_type == LEX_TRUE) {
        st_lex.push(LEX_BOOL);
        gl();
    } else if (c_type == LEX_ID) {
        check_id();
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

//void Parser::Ident(int name_space) {
//    if (c_type == LEX_ID) {
//        if (TID[c_val].get_namespace() != name_space) {
//            throw curr_lex;
//        }
//        if (TID[c_val].get_type() == LEX_STRUCT) {
//            int struct_val = c_val;
//            gl();
//            if (c_type == LEX_POINT) {
//                gl();
//                Ident(struct_val);
//            } else {
//
//            }
//        }
//    } else {
//        throw curr_lex;
//    }
//}



//void Parser::dec(int type_val, int id_val, bool is_struct) {
//    if (TID[id_val].get_declare())
//        throw "twice";
//    else {
//        TID[id_val].put_declare();
//        if (is_struct) {
//            TID[id_val].put_is_struct();
//            TID[id_val].put_type(type_val);
//        } else {
//            TID[id_val].put_type(type_val);
//        }
//    }
//}

void Parser::dec(type_of_lex type, int id_val) {
    if (TID[id_val].get_declare())
        throw "twice";
    else {
        TID[id_val].put_declare();
        TID[id_val].put_type(type);
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
        } else if (op == LEX_EQ || op == LEX_NEQ ||
                op == LEX_LSS || op == LEX_GTR) {
            st_lex.push(LEX_BOOL);
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
}

//void Parser::check_not_struct() {
//    if (st_lex.top() == LEX_STRUCT) {
//        throw "struct ident in expression";
//    }
//}

#endif //PRAK4_PARSER_H
