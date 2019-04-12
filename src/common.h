#ifndef COMMON_H
#define COMMON_H

#define DEBUG_LEXER
#ifdef  DEBUG_LEXER
#define log_l printf
#else
#define log_l(...) {}
#endif

#endif //LEXER_H
