import re

# words_list = r'''
#     LPAREN,    // (
#     RPAREN,    // )
#     LBRACKET,  // [,
#     RBRACKET,  // ],
#     LBRACE,    // {
#     RBRACE,    // }
#     BACKSLASH, // '\'
#     SEMICOLON, // ;
#     COLON,     // :
#     COMMA,     // ,
#     SINGLE_QUOTE, // '
#     DOLLAR,    // $
#     DOT,       // .
#     ELLIPSIS,  // ...
#     IDENT,     // [a-zA-Z][a-zA-Z0-9]*
#     STRING,    // ".*"
#     CHAR,      // '\n'
#     NUM,       // [0-9]*
#     INC_DEC,   // ++, --
#     OPERATOR,  // +, -, /, %, ~, |, ^, <<, >>, !, &&, ||
#     ARROW,     // ->
#     STAR,      // *
#     ASSIGNOP,  // =, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
#     COMPAREOP, // >, <, !=, <=, =>, ==
#     HASH,      // #
#     DOUBLEHASH,// ##
#     AMPERSAND, // &
#     QUESTIONMARK, // ?
#     /* KEYWORDS */
#     AUTO,
#     CASE,
#     CONST,
#     DEFAULT,
#     DO,
#     ELSE,
#     ENUM,
#     FOR,
#     GOTO,
#     IF,
#     LABEL,
#     RETURN,
#     STRUCT,
#     SWITCH,
#     THEN,
#     TYPEDEF,
#     TYPE,
#     WHILE,
#
#     END_OF_FILE,
#     ERROR
#
# '''

def get_word(line):
    # re_comment = re.compile(r"(//.*)|(/[*].*[*]/)", re.U)
    # line = re_comment.sub('', line)
    line = line.replace(',', '')
    return line.strip()
    
def wrapper(word):
    return "        RETURN_NAME(" + word + ");"


if __name__=='__main__':
    with open('src/frontend/lexer/token.hpp', 'r') as f:
        raw_txt = f.read()
    re_comment = re.compile(r"(//.*)|(/[*].*[*]/)", re.U)
    raw_txt = re_comment.sub('', raw_txt)
    tokens_re = re.compile(r'enum Type {([^}]*)}', re.U | re.DOTALL)

    words_list = tokens_re.search(raw_txt).group(1)

    words_list = words_list.split('\n')
    words_list = [get_word(word) for word in words_list]

    output = '\n'.join(wrapper(word) for word in words_list if word)
    print(output)
    with open('codegen_output.txt', 'w') as f:
        f.write(output)

    with open('src/frontend/lexer/token.cpp', 'r') as f:
        output_old = f.read()
        print(output_old)

    with open('src/frontend/lexer/token.cpp', 'w') as f:
        aim_re = re.compile(r'''
#define RETURN_NAME\(p\) case\(p\): return std::string\(#p\);
    switch\(t\){
(.*)
        default: return "undefined lex type";
    ''', re.U | re.DOTALL)

        old = aim_re.search(output_old).group(1)
        print(old)

        output = output_old.replace(old, output)

        print(output)
        f.write(output)
