
// Generated from PoolParser.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"
#include "PoolParserListener.h"


/**
 * This class provides an empty implementation of PoolParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  PoolParserBaseListener : public PoolParserListener {
public:

  virtual void enterProgram(PoolParser::ProgramContext * /*ctx*/) override { }
  virtual void exitProgram(PoolParser::ProgramContext * /*ctx*/) override { }

  virtual void enterStatement(PoolParser::StatementContext * /*ctx*/) override { }
  virtual void exitStatement(PoolParser::StatementContext * /*ctx*/) override { }

  virtual void enterExpression(PoolParser::ExpressionContext * /*ctx*/) override { }
  virtual void exitExpression(PoolParser::ExpressionContext * /*ctx*/) override { }

  virtual void enterAssignment(PoolParser::AssignmentContext * /*ctx*/) override { }
  virtual void exitAssignment(PoolParser::AssignmentContext * /*ctx*/) override { }

  virtual void enterAccess(PoolParser::AccessContext * /*ctx*/) override { }
  virtual void exitAccess(PoolParser::AccessContext * /*ctx*/) override { }

  virtual void enterCall(PoolParser::CallContext * /*ctx*/) override { }
  virtual void exitCall(PoolParser::CallContext * /*ctx*/) override { }

  virtual void enterArgs(PoolParser::ArgsContext * /*ctx*/) override { }
  virtual void exitArgs(PoolParser::ArgsContext * /*ctx*/) override { }

  virtual void enterTerm(PoolParser::TermContext * /*ctx*/) override { }
  virtual void exitTerm(PoolParser::TermContext * /*ctx*/) override { }

  virtual void enterPar(PoolParser::ParContext * /*ctx*/) override { }
  virtual void exitPar(PoolParser::ParContext * /*ctx*/) override { }

  virtual void enterBlock(PoolParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(PoolParser::BlockContext * /*ctx*/) override { }

  virtual void enterArr(PoolParser::ArrContext * /*ctx*/) override { }
  virtual void exitArr(PoolParser::ArrContext * /*ctx*/) override { }

  virtual void enterFun(PoolParser::FunContext * /*ctx*/) override { }
  virtual void exitFun(PoolParser::FunContext * /*ctx*/) override { }

  virtual void enterNum(PoolParser::NumContext * /*ctx*/) override { }
  virtual void exitNum(PoolParser::NumContext * /*ctx*/) override { }

  virtual void enterString(PoolParser::StringContext * /*ctx*/) override { }
  virtual void exitString(PoolParser::StringContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

