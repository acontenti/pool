
// Generated from PoolLexer.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  PoolLexer : public antlr4::Lexer {
public:
  enum {
    WS = 1, COMMENT = 2, LINE_COMMENT = 3, SEMI = 4, LP = 5, RP = 6, LB = 7, 
    RB = 8, LSB = 9, RSB = 10, DOT = 11, COMMA = 12, EQ = 13, CEQ = 14, 
    DOTDOT = 15, COLON = 16, HASH = 17, AT = 18, QM = 19, DOTS = 20, DECIMAL_INTEGER_LITERAL = 21, 
    HEX_INTEGER_LITERAL = 22, BIN_INTEGER_LITERAL = 23, FLOAT_LITERAL = 24, 
    STRING_LITERAL = 25, NATIVE_SYMBOL = 26, ID = 27
  };

  explicit PoolLexer(antlr4::CharStream *input);

  ~PoolLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

