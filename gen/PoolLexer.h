
// Generated from PoolLexer.g4 by ANTLR 4.7.2

#pragma once


#include "antlr4-runtime.h"




class  PoolLexer : public antlr4::Lexer {
public:
  enum {
    WS = 1, COMMENT = 2, LINE_COMMENT = 3, SEMI = 4, LP = 5, RP = 6, LB = 7, 
    RB = 8, LSB = 9, RSB = 10, DOT = 11, COMMA = 12, COLON = 13, TRUE = 14, 
    FALSE = 15, NULL_ = 16, DECIMAL_INTEGER_LITERAL = 17, HEX_INTEGER_LITERAL = 18, 
    BIN_INTEGER_LITERAL = 19, FLOAT_LITERAL = 20, STRING_LITERAL = 21, IDENTIFIER = 22, 
    OPERATOR = 23, IDENT_PART = 24, SYMBOL = 25
  };

  PoolLexer(antlr4::CharStream *input);
  ~PoolLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

