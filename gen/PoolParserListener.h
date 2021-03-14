
// Generated from PoolParser.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "PoolParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by PoolParser.
 */
class  PoolParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProgram(PoolParser::ProgramContext *ctx) = 0;
  virtual void exitProgram(PoolParser::ProgramContext *ctx) = 0;

  virtual void enterStatement(PoolParser::StatementContext *ctx) = 0;
  virtual void exitStatement(PoolParser::StatementContext *ctx) = 0;

  virtual void enterExpression(PoolParser::ExpressionContext *ctx) = 0;
  virtual void exitExpression(PoolParser::ExpressionContext *ctx) = 0;

  virtual void enterAssignment(PoolParser::AssignmentContext *ctx) = 0;
  virtual void exitAssignment(PoolParser::AssignmentContext *ctx) = 0;

  virtual void enterAccess(PoolParser::AccessContext *ctx) = 0;
  virtual void exitAccess(PoolParser::AccessContext *ctx) = 0;

  virtual void enterCall(PoolParser::CallContext *ctx) = 0;
  virtual void exitCall(PoolParser::CallContext *ctx) = 0;

  virtual void enterArgs(PoolParser::ArgsContext *ctx) = 0;
  virtual void exitArgs(PoolParser::ArgsContext *ctx) = 0;

  virtual void enterTerm(PoolParser::TermContext *ctx) = 0;
  virtual void exitTerm(PoolParser::TermContext *ctx) = 0;

  virtual void enterPar(PoolParser::ParContext *ctx) = 0;
  virtual void exitPar(PoolParser::ParContext *ctx) = 0;

  virtual void enterBlock(PoolParser::BlockContext *ctx) = 0;
  virtual void exitBlock(PoolParser::BlockContext *ctx) = 0;

  virtual void enterArr(PoolParser::ArrContext *ctx) = 0;
  virtual void exitArr(PoolParser::ArrContext *ctx) = 0;

  virtual void enterFun(PoolParser::FunContext *ctx) = 0;
  virtual void exitFun(PoolParser::FunContext *ctx) = 0;

  virtual void enterNum(PoolParser::NumContext *ctx) = 0;
  virtual void exitNum(PoolParser::NumContext *ctx) = 0;

  virtual void enterString(PoolParser::StringContext *ctx) = 0;
  virtual void exitString(PoolParser::StringContext *ctx) = 0;


};

