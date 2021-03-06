#ifndef MLANG_INTERPRETER_SCANNER_H
#define MLANG_INTERPRETER_SCANNER_H

#include "iostream"
#include "cstdio"
#include "vector"
#include "algorithm"
#include "stack"
using namespace std;

enum type_of_lex {
    LEX_NULL, // 0

    LEX_PROGRAM,
    LEX_BOOL, LEX_INT, LEX_STRING, LEX_STRUCT,
    LEX_TRUE, LEX_FALSE,
    LEX_IF, LEX_ELSE, LEX_WHILE, LEX_FOR, LEX_BREAK,
    LEX_AND, LEX_OR, LEX_NOT,
    LEX_READ, LEX_WRITE,
    LEX_GOTO, // 18

    LEX_FIN,
    LEX_COMMA, LEX_SEMICOLON, LEX_COLON, LEX_POINT,
    LEX_ASSIGN,
    LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH,
    LEX_EQ, LEX_NEQ, LEX_LSS, LEX_GTR, LEX_LEQ, LEX_GEQ,
    LEX_LPAREN, LEX_RPAREN, LEX_LBRACE, LEX_RBRACE, // 38

    LEX_SPLUS,
    LEX_SEQ, LEX_SNEQ, LEX_SLSS, LEX_SGTR,
    LEX_SASSIGN,
    LEX_SWRITE, // 45

    LEX_STRUCT_TYPE,
    LEX_STRASSIGN, // 47

    LEX_NUM,
    LEX_STR,
    LEX_ID, // 50

    POLIZ_LABEL, // 51
    POLIZ_ADDRESS, // 52
    POLIZ_GO, // 53
    POLIZ_FGO // 54
};

class Lex {
    type_of_lex t_lex;
    int v_lex;
    int aux_v_lex;
public:
    explicit Lex(type_of_lex t = LEX_NULL, int v = 0, int aux_v = -1) {
        t_lex = t;
        v_lex = v;
        aux_v_lex = aux_v;
    }
    type_of_lex get_type() const { return t_lex; }
    int get_value() const { return v_lex; }
    int get_aux_value() const { return aux_v_lex; }
    friend ostream& operator<<(ostream& s, Lex l);
};

class Ident {
    string      name;
    bool        declare;
    type_of_lex type;
    bool        assign;
    int         value;
    int         struct_type;
public:
    explicit Ident() {
        declare = false;
        assign = false;
    }
    explicit Ident (const string& n) {
        name = n;
        declare = false;
        assign = false;
    }
    bool operator==(const string& s) const {
        return name == s;
    }
    string get_name() const { return name; }
    bool get_declare() const { return declare; }
    void put_declare() { declare = true; }
    type_of_lex get_type() const { return type; }
    void put_type(type_of_lex t) { type = t; }
    bool get_assign() const { return assign; }
    void put_assign() { assign = true; }
    int get_value() const { return value; }
    void put_value(int v) { value = v; }
    int get_struct_type() const { return struct_type; }
    void put_struct_type(int t) { struct_type = t; }
};

vector<Ident> TID;

vector<string> TSTR;

class Struct {
public:
    string name;
    vector<Ident> tid;
};

vector<Struct> TTYPE;
vector<Struct> TSTRUCT;

int put (const string& buf) {
    vector<Ident>::const_iterator k;
    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end()) {
        return k - TID.begin();
    }
    TID.push_back(Ident(buf));
    return TID.size() - 1;
}

int put_str(const string& buf) {
    vector<string>::const_iterator k;
    if ((k = find(TSTR.begin(), TSTR.end(), buf)) != TSTR.end() ) {
        return k - TSTR.begin();
    }
    TSTR.push_back(buf);
    return TSTR.size() - 1;
}

class Scanner {
    FILE *  fp;
    char    c;
    static int look(const string& buf, const vector<const char*>& list) {
        int i = 0;
        while (list[i]) {
            if (buf == list[i]) {
                return i;
            }
            i++;
        }
        return 0;
    }
    void gc() { c = fgetc(fp); }
public:
    stack<const Lex> st_lex;
    static const vector<const char*> TW, TD;
    explicit Scanner(const char* program) {
        fp = fopen(program,"r");
    }
    Lex get_lex();
    void unget_lex(const Lex& lex) {
        st_lex.push(lex);
    }
};

const vector<const char*> Scanner::TW = {
        "", // 0
        "program", // 1
        "boolean", // 2
        "int", // 3
        "string", // 4
        "struct", // 5
        "true", // 6
        "false", // 7
        "if", // 8
        "else", // 9
        "while", // 10
        "for", // 11
        "break", // 12
        "and", // 13
        "or", // 14
        "not", // 15
        "read", // 16
        "write", // 17
        "goto", // 18
        nullptr
};

const vector<const char*> Scanner::TD = {
        "EOF", // 0
        ",", // 1
        ";", // 2
        ":", // 3
        ".", // 4
        "=", // 5
        "+", // 6
        "-", // 7
        "*", // 8
        "/", // 9
        "==", // 10
        "!=", // 11
        "<", // 12
        ">", // 13
        "<=", // 14
        ">=", // 15
        "(", // 16
        ")", // 17
        "{", // 18
        "}", // 19
        nullptr
};


ostream& operator<<(ostream &s, Lex l) {
    string t;
    if (l.t_lex <= LEX_GOTO)
        t = Scanner::TW[l.t_lex];
    else if (l.t_lex >= LEX_FIN && l.t_lex <= LEX_RBRACE)
        t = Scanner::TD[l.t_lex - LEX_FIN];
    else if (l.t_lex == LEX_NUM)
        t = "Int";
    else if (l.t_lex == LEX_STR)
        t = "Str";
    else if (l.t_lex == LEX_ID)
        if (l.aux_v_lex == -1) {
            t = "Id " + TID[l.v_lex].get_name();
        } else {
            t = "Id " + TSTRUCT[l.aux_v_lex].name + "." + TSTRUCT[l.aux_v_lex].tid[l.v_lex].get_name();
        }
    else if ( l.t_lex == POLIZ_LABEL )
        t = "Label";
    else if ( l.t_lex == POLIZ_ADDRESS )
        t = "Addr";
    else if ( l.t_lex == POLIZ_GO )
        t = "!";
    else if ( l.t_lex == POLIZ_FGO )
        t = "!F";
    else if ( l.t_lex == LEX_SWRITE )
        t = "str write";
    else if ( l.t_lex == LEX_SASSIGN )
        t = "str =";
    else if ( l.t_lex == LEX_SPLUS )
        t = "str +";
    else if ( l.t_lex == LEX_SEQ )
        t = "str ==";
    else if ( l.t_lex == LEX_SNEQ )
        t = "str !=";
    else if ( l.t_lex == LEX_SGTR )
        t = "str >";
    else if ( l.t_lex == LEX_SLSS )
        t = "str <";
    else if (l.t_lex == LEX_STRASSIGN)
        t = "struct =";
    else
        throw l;
    s << '(' << t << ',' << l.v_lex << ");" << endl;
    return s;
}

Lex Scanner::get_lex() {
    if (st_lex.size() > 0) {
        Lex top_lex = st_lex.top();
        st_lex.pop();
        return top_lex;
    }
    enum    state { H, IDENT, NUMB, STR, COM, CBEG, CEND, ALGE, NEQ, MINUS, PLUS };
    state   CS = H;
    string  buf;
    int     d, j, s = 1;
    do {
        gc();
        switch (CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {}
                else if (isalpha(c)) {
                    buf.push_back(c);
                    CS = IDENT;
                } else if (isdigit(c)) {
                    d = c - '0';
                    CS = NUMB;
                } else if (c == '-') {
                    CS = MINUS;
                } else if (c == '+') {
                    CS = PLUS;
                } else if (c == '"') {
                    CS = STR;
                } else if ( c == '/' ) {
                    CS = CBEG;
                } else if (c == '=' || c == '<' || c == '>') {
                    buf.push_back(c);
                    CS = ALGE;
                } else if (c == EOF) {
                    return Lex(LEX_FIN);
                } else if (c == '!') {
                    buf.push_back(c);
                    CS = NEQ;
                } else {
                    buf.push_back(c);
                    if ((j = look (buf, TD))) {
                        return Lex((type_of_lex) (j + (int) LEX_FIN), j);
                    } else {
                        throw c;
                    }
                }
                break;
            case IDENT:
                if ( isalpha(c) || isdigit(c) ) {
                    buf.push_back(c);
                }
                else {
                    ungetc(c, fp);
                    if ((j = look (buf, TW)))
                        return Lex ((type_of_lex) j, j );
                    else {
                        j = put(buf);
                        return Lex(LEX_ID, j);
                    }
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                } else {
                    ungetc(c, fp);
                    return Lex(LEX_NUM, s * d);
                }
                break;
            case CBEG:
                if ( c == '*' ) {
                    CS = COM;
                }
                else {
                    return Lex((type_of_lex) (LEX_SLASH), LEX_SLASH - LEX_FIN);
                }
                break;
            case COM:
                if ( c == '*' ) {
                    CS = CEND;
                }
                else if (c == EOF) {
                    throw (char) EOF;
                }
                break;
            case CEND:
                if ( c == '/' ) {
                    CS = H;
                }
                else if (c == EOF) {
                    throw (char) EOF;
                } else {
                    CS = COM;
                }
                break;
            case ALGE:
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN), j);
                } else {
                    ungetc(c, fp);
                    j = look(buf, TD);
                    return Lex((type_of_lex) (j + (int) LEX_FIN), j);
                }
            case MINUS:
                if (isdigit(c)) {
                    s = -1;
                    d = c - '0';
                    CS = NUMB;
                } else {
                    ungetc(c, fp);
                    return Lex((type_of_lex) (LEX_MINUS), LEX_MINUS - LEX_FIN);
                }
                break;
            case PLUS:
                 if (isdigit(c)) {
                     d = c - '0';
                     CS = NUMB;
                 } else {
                     ungetc(c, fp);
                     return Lex((type_of_lex) (LEX_PLUS), LEX_PLUS - LEX_FIN);
                 }
                break;
            case NEQ:
                if (c == '=') {
                    return Lex ((type_of_lex) (LEX_NEQ), LEX_NEQ - LEX_FIN);
                }
                else {
                    throw '!';
                }
            case STR:
                if ( c == '"' ) {
                    j = put_str(buf);
                    return Lex(LEX_STR, j);
                }
                else if (c == EOF) {
                    throw (char) EOF;
                } else {
                    buf.push_back(c);
                }
                break;
        } //end of switch
    }
    while (true);
} // end of getlex()

#endif //MLANG_INTERPRETER_SCANNER_H