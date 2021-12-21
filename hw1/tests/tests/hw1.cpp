#include "tokens.hpp"
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

extern int yyleng;
extern int yylineno;
extern char* yytext;
extern int yylex();



bool validHex(const char* buf, int idx)
{
    auto char_in_range = [] (char low, char tested, char high) {
        return (low <= tested) && (tested <= high);
    };
    char x = buf[idx];
    if (x != 'x') return false;
    char c1 = buf[idx + 1];
    if (! char_in_range('0', c1, '7')) return false;
    char c2 = buf[idx + 2];
    return (char_in_range('0', c2, '9') || char_in_range('A', c2, 'F') || char_in_range('a', c2, 'f'));
}

bool validEscape(const char* buf, int idx)
{
    vector<char> escape_chars{'t', '\\', '0', 'n', '"', 'r'};
    if(find(escape_chars.begin(), escape_chars.end(), buf[idx]) != escape_chars.end()) {
        return true;
    } else {
        return false;
    }
}

char getHex(const char* buf, int idx){
    string hex;
    hex.push_back(yytext[idx + 1]);
    hex.push_back(yytext[idx + 2]);
    char hex_char = stoul(hex, nullptr, 16);
    return hex_char;
}


void parseString() {
    string result;
    for (int i = 1; i < yyleng - 1; i++)
    {
        if (yytext[i] == '\\')
        {
            i++;
            switch (yytext[i]) {
                case ('x'): result.push_back(getHex(yytext, i)); i += 2; break;
                case('t'): result.push_back('\t'); break;
                case('\\'): result.push_back('\\'); break;
                case('0'): result.push_back('\0'); break;
                case ('n'): result.push_back('\n'); break;
                case('\"'): result.push_back('\"'); break;
                case('r'): result.push_back('\r'); break;
            }
        }
        else
        {
            result.push_back(yytext[i]);
        }
    }
    cout << yylineno << " " << "STRING" << " " << result << '\n';
}

void showToken(const char* token){
    string token_name(token);
    if (token_name == "STRING")
    {
        parseString();
    }
    else if (token_name == "COMMENT")
    {
        cout << yylineno << " " << token << " " << "//\n";
    } else
    {
        cout << yylineno << " " << token << " " << yytext << "\n";
    }
}

void errorHandler(int error){
    switch(error){
        case UNDEFINED_ESCAPE: {
            int i = 1;
            while (i < yyleng - 1) {
                if (yytext[i] == '\\') {
                    i++;
                    if (validEscape(yytext, i)) i++;
                    else if (validHex(yytext, i)) i += 3;
                    else break;
                }
                else i++;
            }
            string undefined_escape;
            if (yytext[i] == 'x') {
                int undef_esc_idx_max_limit = i + 3;
                while (i < undef_esc_idx_max_limit && ((yytext[i] != '\0' && yytext[i] != '\"'))) {
                    undefined_escape.push_back(yytext[i]);
                    i++;
                }
            } else {
                undefined_escape.push_back(yytext[i]);
            }
            cout << "Error undefined escape sequence " << undefined_escape << "\n";
            break;
        }
        case UNCLOSED_STRING: {
            cout << "Error unclosed string" << "\n";
            break;
        }
        default:
            cout << "Error" << " " << yytext << "\n";
    }
    exit(0);
}

int main()
{
	int token;
	while((token = yylex())) {
        switch (token) {
            case VOID:          showToken("VOID");    break;
            case INT:           showToken("INT");    break;
            case BYTE:          showToken("BYTE");    break;
            case B:             showToken("B");    break;
            case BOOL:          showToken("BOOL");    break;
            case AND:           showToken("AND");    break;
            case OR:            showToken("OR");    break;
            case NOT:           showToken("NOT");    break;
            case TRUE:          showToken("TRUE");    break;
            case FALSE:         showToken("FALSE");    break;
            case RETURN:        showToken("RETURN");    break;
            case IF:            showToken("IF");    break;
            case ELSE:          showToken("ELSE");    break;
            case WHILE:         showToken("WHILE");    break;
            case BREAK:         showToken("BREAK");    break;
            case CONTINUE:      showToken("CONTINUE");    break;
            case SC:            showToken("SC");    break;
            case COMMA:         showToken("COMMA");    break;
            case LPAREN:        showToken("LPAREN");    break;
            case RPAREN:        showToken("RPAREN");    break;
            case LBRACE:        showToken("LBRACE");    break;
            case RBRACE:        showToken("RBRACE");    break;
            case ASSIGN:        showToken("ASSIGN");    break;
            case RELOP:         showToken("RELOP");    break;
            case BINOP:         showToken("BINOP");    break;
            case COMMENT:       showToken("COMMENT");   break;
            case ID:            showToken("ID");    break;
            case NUM:           showToken("NUM");    break;
            case STRING:        showToken("STRING");    break;
            case UNCLOSED_STRING:   errorHandler(UNCLOSED_STRING);  break;
            case UNDEFINED_ESCAPE:  errorHandler(UNDEFINED_ESCAPE); break;
            case UNDEFINED_CHAR:     errorHandler(UNDEFINED_CHAR);    break;
            default:        cout << "Error:" << " " << yytext << " should not be here!\n";
        }
	}
	return 0;
}