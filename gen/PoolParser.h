
// Generated from PoolParser.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  PoolParser : public antlr4::Parser {
public:
  enum {
    WS = 1, COMMENT = 2, LINE_COMMENT = 3, SEMI = 4, LP = 5, RP = 6, LB = 7, 
    RB = 8, LSB = 9, RSB = 10, DOT = 11, COMMA = 12, EQ = 13, CEQ = 14, 
    DOTDOT = 15, COLON = 16, HASH = 17, AT = 18, DOTS = 19, DECIMAL_INTEGER_LITERAL = 20, 
    HEX_INTEGER_LITERAL = 21, BIN_INTEGER_LITERAL = 22, FLOAT_LITERAL = 23, 
    STRING_LITERAL = 24, NATIVE_SYMBOL = 25, ID = 26
  };

  enum {
    RuleProgram = 0, RuleStatement = 1, RuleExpression = 2, RuleAssignment = 3, 
    RuleAccess = 4, RuleCall = 5, RuleArgs = 6, RuleArg = 7, RuleTerm = 8, 
    RulePar = 9, RuleBlock = 10, RuleArray = 11, RuleFun = 12, RuleParam = 13, 
    RuleNum = 14, RuleString = 15
  };

  explicit PoolParser(antlr4::TokenStream *input);

  PoolParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~PoolParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ProgramContext;
  class StatementContext;
  class ExpressionContext;
  class AssignmentContext;
  class AccessContext;
  class CallContext;
  class ArgsContext;
  class ArgContext;
  class TermContext;
  class ParContext;
  class BlockContext;
  class ArrayContext;
  class FunContext;
  class ParamContext;
  class NumContext;
  class StringContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

   
  };

  ProgramContext* program();

  class  StatementContext : public antlr4::ParserRuleContext {
  public:
    StatementContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SEMI();
    ExpressionContext *expression();

   
  };

  StatementContext* statement();

  class  ExpressionContext : public antlr4::ParserRuleContext {
  public:
    PoolParser::AssignmentContext *a = nullptr;
    PoolParser::CallContext *c = nullptr;
    ExpressionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    AssignmentContext *assignment();
    CallContext *call();

   
  };

  ExpressionContext* expression();

  class  AssignmentContext : public antlr4::ParserRuleContext {
  public:
    enum Type {V,C} type;
    PoolParser::AccessContext *assignee = nullptr;
    PoolParser::CallContext *value = nullptr;
    AssignmentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EQ();
    AccessContext *access();
    CallContext *call();
    antlr4::tree::TerminalNode *CEQ();

   
  };

  AssignmentContext* assignment();

  class  AccessContext : public antlr4::ParserRuleContext {
  public:
    enum Type {G,L,I} type;
    PoolParser::CallContext *callee = nullptr;
    AccessContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *ID();
    CallContext *call();
    antlr4::tree::TerminalNode *DOTDOT();

   
  };

  AccessContext* access();

  class  CallContext : public antlr4::ParserRuleContext {
  public:
    enum Type {IA,ILA,A,T} type;
    PoolParser::CallContext *callee = nullptr;
    PoolParser::ArgsContext *a = nullptr;
    CallContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    TermContext *term();
    antlr4::tree::TerminalNode *DOT();
    antlr4::tree::TerminalNode *ID();
    CallContext *call();
    ArgsContext *args();
    antlr4::tree::TerminalNode *DOTDOT();

   
  };

  CallContext* call();
  CallContext* call(int precedence);
  class  ArgsContext : public antlr4::ParserRuleContext {
  public:
    ArgsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    std::vector<ArgContext *> arg();
    ArgContext* arg(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

   
  };

  ArgsContext* args();

  class  ArgContext : public antlr4::ParserRuleContext {
  public:
    ArgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    CallContext *call();
    antlr4::tree::TerminalNode *DOTS();

   
  };

  ArgContext* arg();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    enum Type {NUM,STR,FUN,PAR,BLK,ARR,NSM,IDT} type;
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NumContext *num();
    StringContext *string();
    FunContext *fun();
    ParContext *par();
    BlockContext *block();
    ArrayContext *array();
    antlr4::tree::TerminalNode *NATIVE_SYMBOL();
    antlr4::tree::TerminalNode *ID();

   
  };

  TermContext* term();

  class  ParContext : public antlr4::ParserRuleContext {
  public:
    ParContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    ExpressionContext *expression();
    antlr4::tree::TerminalNode *RP();

   
  };

  ParContext* par();

  class  BlockContext : public antlr4::ParserRuleContext {
  public:
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

   
  };

  BlockContext* block();

  class  ArrayContext : public antlr4::ParserRuleContext {
  public:
    ArrayContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LSB();
    antlr4::tree::TerminalNode *RSB();
    std::vector<ArgContext *> arg();
    ArgContext* arg(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);

   
  };

  ArrayContext* array();

  class  FunContext : public antlr4::ParserRuleContext {
  public:
    FunContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LP();
    antlr4::tree::TerminalNode *RP();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *LB();
    antlr4::tree::TerminalNode *RB();
    std::vector<ParamContext *> param();
    ParamContext* param(size_t i);
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    std::vector<StatementContext *> statement();
    StatementContext* statement(size_t i);

   
  };

  FunContext* fun();

  class  ParamContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *name = nullptr;
    antlr4::Token *type = nullptr;
    ParamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    antlr4::tree::TerminalNode *DOTS();
    antlr4::tree::TerminalNode *COLON();

   
  };

  ParamContext* param();

  class  NumContext : public antlr4::ParserRuleContext {
  public:
    enum Type {DEC,HEX,BIN,FLT} type;
    NumContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *DECIMAL_INTEGER_LITERAL();
    antlr4::tree::TerminalNode *HEX_INTEGER_LITERAL();
    antlr4::tree::TerminalNode *BIN_INTEGER_LITERAL();
    antlr4::tree::TerminalNode *FLOAT_LITERAL();

   
  };

  NumContext* num();

  class  StringContext : public antlr4::ParserRuleContext {
  public:
    StringContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *STRING_LITERAL();

   
  };

  StringContext* string();


  bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

  bool callSempred(CallContext *_localctx, size_t predicateIndex);

  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

