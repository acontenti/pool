
// Generated from PoolParser.g4 by ANTLR 4.7.2


#include "PoolParserListener.h"

#include "PoolParser.h"


using namespace antlrcpp;
using namespace antlr4;

PoolParser::PoolParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

PoolParser::~PoolParser() {
  delete _interpreter;
}

std::string PoolParser::getGrammarFileName() const {
  return "PoolParser.g4";
}

const std::vector<std::string>& PoolParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& PoolParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgramContext ------------------------------------------------------------------

PoolParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::ProgramContext::EOF() {
  return getToken(PoolParser::EOF, 0);
}

std::vector<PoolParser::StatementContext *> PoolParser::ProgramContext::statement() {
  return getRuleContexts<PoolParser::StatementContext>();
}

PoolParser::StatementContext* PoolParser::ProgramContext::statement(size_t i) {
  return getRuleContext<PoolParser::StatementContext>(i);
}


size_t PoolParser::ProgramContext::getRuleIndex() const {
  return PoolParser::RuleProgram;
}

void PoolParser::ProgramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProgram(this);
}

void PoolParser::ProgramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProgram(this);
}

PoolParser::ProgramContext* PoolParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, PoolParser::RuleProgram);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(31);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(28);
      statement();
      setState(33);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(34);
    match(PoolParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatementContext ------------------------------------------------------------------

PoolParser::StatementContext::StatementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::StatementContext::SEMI() {
  return getToken(PoolParser::SEMI, 0);
}

PoolParser::CallContext* PoolParser::StatementContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}


size_t PoolParser::StatementContext::getRuleIndex() const {
  return PoolParser::RuleStatement;
}

void PoolParser::StatementContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStatement(this);
}

void PoolParser::StatementContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStatement(this);
}

PoolParser::StatementContext* PoolParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, PoolParser::RuleStatement);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(37);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(36);
      call(0);
    }
    setState(39);
    match(PoolParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CallContext ------------------------------------------------------------------

PoolParser::CallContext::CallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PoolParser::TermContext* PoolParser::CallContext::term() {
  return getRuleContext<PoolParser::TermContext>(0);
}

tree::TerminalNode* PoolParser::CallContext::OPERATOR() {
  return getToken(PoolParser::OPERATOR, 0);
}

std::vector<PoolParser::CallContext *> PoolParser::CallContext::call() {
  return getRuleContexts<PoolParser::CallContext>();
}

PoolParser::CallContext* PoolParser::CallContext::call(size_t i) {
  return getRuleContext<PoolParser::CallContext>(i);
}

tree::TerminalNode* PoolParser::CallContext::DOT() {
  return getToken(PoolParser::DOT, 0);
}

tree::TerminalNode* PoolParser::CallContext::IDENTIFIER() {
  return getToken(PoolParser::IDENTIFIER, 0);
}

PoolParser::ArgsContext* PoolParser::CallContext::args() {
  return getRuleContext<PoolParser::ArgsContext>(0);
}


size_t PoolParser::CallContext::getRuleIndex() const {
  return PoolParser::RuleCall;
}

void PoolParser::CallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCall(this);
}

void PoolParser::CallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCall(this);
}


PoolParser::CallContext* PoolParser::call() {
   return call(0);
}

PoolParser::CallContext* PoolParser::call(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  PoolParser::CallContext *_localctx = _tracker.createInstance<CallContext>(_ctx, parentState);
  PoolParser::CallContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 4;
  enterRecursionRule(_localctx, 4, PoolParser::RuleCall, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(42);
    term();
    dynamic_cast<CallContext *>(_localctx)->type = CallContext::T;
    _ctx->stop = _input->LT(-1);
    setState(74);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(72);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(45);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(46);
          match(PoolParser::OPERATOR);
          setState(47);
          dynamic_cast<CallContext *>(_localctx)->arg = call(3);
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::TOC;
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(50);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(51);
          match(PoolParser::DOT);
          setState(52);
          match(PoolParser::IDENTIFIER);
          setState(53);
          args();
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::TIA;
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(56);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(57);
          match(PoolParser::DOT);
          setState(58);
          match(PoolParser::IDENTIFIER);
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::TI;
          break;
        }

        case 4: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(60);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(61);
          args();
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::TA;
          break;
        }

        case 5: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(64);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(65);
          match(PoolParser::OPERATOR);
          setState(66);
          args();
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::TOA;
          break;
        }

        case 6: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(69);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(70);
          match(PoolParser::OPERATOR);
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::TO;
          break;
        }

        } 
      }
      setState(76);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ArgsContext ------------------------------------------------------------------

PoolParser::ArgsContext::ArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::ArgsContext::LP() {
  return getToken(PoolParser::LP, 0);
}

tree::TerminalNode* PoolParser::ArgsContext::RP() {
  return getToken(PoolParser::RP, 0);
}

std::vector<PoolParser::CallContext *> PoolParser::ArgsContext::call() {
  return getRuleContexts<PoolParser::CallContext>();
}

PoolParser::CallContext* PoolParser::ArgsContext::call(size_t i) {
  return getRuleContext<PoolParser::CallContext>(i);
}

std::vector<tree::TerminalNode *> PoolParser::ArgsContext::COMMA() {
  return getTokens(PoolParser::COMMA);
}

tree::TerminalNode* PoolParser::ArgsContext::COMMA(size_t i) {
  return getToken(PoolParser::COMMA, i);
}


size_t PoolParser::ArgsContext::getRuleIndex() const {
  return PoolParser::RuleArgs;
}

void PoolParser::ArgsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgs(this);
}

void PoolParser::ArgsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgs(this);
}

PoolParser::ArgsContext* PoolParser::args() {
  ArgsContext *_localctx = _tracker.createInstance<ArgsContext>(_ctx, getState());
  enterRule(_localctx, 6, PoolParser::RuleArgs);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(77);
    match(PoolParser::LP);
    setState(79);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(78);
      call(0);
    }
    setState(85);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(81);
      match(PoolParser::COMMA);
      setState(82);
      call(0);
      setState(87);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(88);
    match(PoolParser::RP);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TermContext ------------------------------------------------------------------

PoolParser::TermContext::TermContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PoolParser::NullContext* PoolParser::TermContext::null() {
  return getRuleContext<PoolParser::NullContext>(0);
}

PoolParser::NumContext* PoolParser::TermContext::num() {
  return getRuleContext<PoolParser::NumContext>(0);
}

PoolParser::BooleanContext* PoolParser::TermContext::boolean() {
  return getRuleContext<PoolParser::BooleanContext>(0);
}

PoolParser::StringContext* PoolParser::TermContext::string() {
  return getRuleContext<PoolParser::StringContext>(0);
}

PoolParser::FunContext* PoolParser::TermContext::fun() {
  return getRuleContext<PoolParser::FunContext>(0);
}

PoolParser::ArrayContext* PoolParser::TermContext::array() {
  return getRuleContext<PoolParser::ArrayContext>(0);
}

PoolParser::ParContext* PoolParser::TermContext::par() {
  return getRuleContext<PoolParser::ParContext>(0);
}

PoolParser::BlockContext* PoolParser::TermContext::block() {
  return getRuleContext<PoolParser::BlockContext>(0);
}

PoolParser::IdContext* PoolParser::TermContext::id() {
  return getRuleContext<PoolParser::IdContext>(0);
}


size_t PoolParser::TermContext::getRuleIndex() const {
  return PoolParser::RuleTerm;
}

void PoolParser::TermContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTerm(this);
}

void PoolParser::TermContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTerm(this);
}

PoolParser::TermContext* PoolParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 8, PoolParser::RuleTerm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(117);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(90);
      null();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NIL;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(93);
      num();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NUM;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(96);
      boolean();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::BLN;
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(99);
      string();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::STR;
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(102);
      fun();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::FUN;
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(105);
      array();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::ARR;
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(108);
      par();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::PAR;
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(111);
      block();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::BLK;
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(114);
      id();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::IDT;
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParContext ------------------------------------------------------------------

PoolParser::ParContext::ParContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::ParContext::LP() {
  return getToken(PoolParser::LP, 0);
}

tree::TerminalNode* PoolParser::ParContext::RP() {
  return getToken(PoolParser::RP, 0);
}

PoolParser::CallContext* PoolParser::ParContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}


size_t PoolParser::ParContext::getRuleIndex() const {
  return PoolParser::RulePar;
}

void PoolParser::ParContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPar(this);
}

void PoolParser::ParContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPar(this);
}

PoolParser::ParContext* PoolParser::par() {
  ParContext *_localctx = _tracker.createInstance<ParContext>(_ctx, getState());
  enterRule(_localctx, 10, PoolParser::RulePar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(119);
    match(PoolParser::LP);
    setState(121);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(120);
      call(0);
    }
    setState(123);
    match(PoolParser::RP);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

PoolParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::BlockContext::LB() {
  return getToken(PoolParser::LB, 0);
}

tree::TerminalNode* PoolParser::BlockContext::RB() {
  return getToken(PoolParser::RB, 0);
}

std::vector<PoolParser::StatementContext *> PoolParser::BlockContext::statement() {
  return getRuleContexts<PoolParser::StatementContext>();
}

PoolParser::StatementContext* PoolParser::BlockContext::statement(size_t i) {
  return getRuleContext<PoolParser::StatementContext>(i);
}


size_t PoolParser::BlockContext::getRuleIndex() const {
  return PoolParser::RuleBlock;
}

void PoolParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void PoolParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}

PoolParser::BlockContext* PoolParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 12, PoolParser::RuleBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(125);
    match(PoolParser::LB);
    setState(129);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(126);
      statement();
      setState(131);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(132);
    match(PoolParser::RB);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArrayContext ------------------------------------------------------------------

PoolParser::ArrayContext::ArrayContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::ArrayContext::LSB() {
  return getToken(PoolParser::LSB, 0);
}

tree::TerminalNode* PoolParser::ArrayContext::RSB() {
  return getToken(PoolParser::RSB, 0);
}

std::vector<PoolParser::CallContext *> PoolParser::ArrayContext::call() {
  return getRuleContexts<PoolParser::CallContext>();
}

PoolParser::CallContext* PoolParser::ArrayContext::call(size_t i) {
  return getRuleContext<PoolParser::CallContext>(i);
}

std::vector<tree::TerminalNode *> PoolParser::ArrayContext::COMMA() {
  return getTokens(PoolParser::COMMA);
}

tree::TerminalNode* PoolParser::ArrayContext::COMMA(size_t i) {
  return getToken(PoolParser::COMMA, i);
}


size_t PoolParser::ArrayContext::getRuleIndex() const {
  return PoolParser::RuleArray;
}

void PoolParser::ArrayContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArray(this);
}

void PoolParser::ArrayContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArray(this);
}

PoolParser::ArrayContext* PoolParser::array() {
  ArrayContext *_localctx = _tracker.createInstance<ArrayContext>(_ctx, getState());
  enterRule(_localctx, 14, PoolParser::RuleArray);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(134);
    match(PoolParser::LSB);
    setState(136);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(135);
      call(0);
    }
    setState(146);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(139); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(138);
        match(PoolParser::COMMA);
        setState(141); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PoolParser::COMMA);
      setState(143);
      call(0);
      setState(148);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(149);
    match(PoolParser::RSB);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunContext ------------------------------------------------------------------

PoolParser::FunContext::FunContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::FunContext::COLON() {
  return getToken(PoolParser::COLON, 0);
}

tree::TerminalNode* PoolParser::FunContext::LB() {
  return getToken(PoolParser::LB, 0);
}

tree::TerminalNode* PoolParser::FunContext::RB() {
  return getToken(PoolParser::RB, 0);
}

std::vector<tree::TerminalNode *> PoolParser::FunContext::IDENTIFIER() {
  return getTokens(PoolParser::IDENTIFIER);
}

tree::TerminalNode* PoolParser::FunContext::IDENTIFIER(size_t i) {
  return getToken(PoolParser::IDENTIFIER, i);
}

std::vector<PoolParser::StatementContext *> PoolParser::FunContext::statement() {
  return getRuleContexts<PoolParser::StatementContext>();
}

PoolParser::StatementContext* PoolParser::FunContext::statement(size_t i) {
  return getRuleContext<PoolParser::StatementContext>(i);
}

tree::TerminalNode* PoolParser::FunContext::LP() {
  return getToken(PoolParser::LP, 0);
}

tree::TerminalNode* PoolParser::FunContext::RP() {
  return getToken(PoolParser::RP, 0);
}

std::vector<tree::TerminalNode *> PoolParser::FunContext::COMMA() {
  return getTokens(PoolParser::COMMA);
}

tree::TerminalNode* PoolParser::FunContext::COMMA(size_t i) {
  return getToken(PoolParser::COMMA, i);
}


size_t PoolParser::FunContext::getRuleIndex() const {
  return PoolParser::RuleFun;
}

void PoolParser::FunContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFun(this);
}

void PoolParser::FunContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFun(this);
}

PoolParser::FunContext* PoolParser::fun() {
  FunContext *_localctx = _tracker.createInstance<FunContext>(_ctx, getState());
  enterRule(_localctx, 16, PoolParser::RuleFun);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(164);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::IDENTIFIER: {
        setState(151);
        match(PoolParser::IDENTIFIER);
        break;
      }

      case PoolParser::LP: {
        setState(152);
        match(PoolParser::LP);
        setState(154);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == PoolParser::IDENTIFIER) {
          setState(153);
          match(PoolParser::IDENTIFIER);
        }
        setState(160);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == PoolParser::COMMA) {
          setState(156);
          match(PoolParser::COMMA);
          setState(157);
          match(PoolParser::IDENTIFIER);
          setState(162);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(163);
        match(PoolParser::RP);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    setState(166);
    match(PoolParser::COLON);
    setState(167);
    match(PoolParser::LB);
    setState(171);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::TRUE)
      | (1ULL << PoolParser::FALSE)
      | (1ULL << PoolParser::NULL_)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::IDENTIFIER))) != 0)) {
      setState(168);
      statement();
      setState(173);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(174);
    match(PoolParser::RB);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IdContext ------------------------------------------------------------------

PoolParser::IdContext::IdContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::IdContext::IDENTIFIER() {
  return getToken(PoolParser::IDENTIFIER, 0);
}


size_t PoolParser::IdContext::getRuleIndex() const {
  return PoolParser::RuleId;
}

void PoolParser::IdContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterId(this);
}

void PoolParser::IdContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitId(this);
}

PoolParser::IdContext* PoolParser::id() {
  IdContext *_localctx = _tracker.createInstance<IdContext>(_ctx, getState());
  enterRule(_localctx, 18, PoolParser::RuleId);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(176);
    match(PoolParser::IDENTIFIER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumContext ------------------------------------------------------------------

PoolParser::NumContext::NumContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::NumContext::DECIMAL_INTEGER_LITERAL() {
  return getToken(PoolParser::DECIMAL_INTEGER_LITERAL, 0);
}

tree::TerminalNode* PoolParser::NumContext::HEX_INTEGER_LITERAL() {
  return getToken(PoolParser::HEX_INTEGER_LITERAL, 0);
}

tree::TerminalNode* PoolParser::NumContext::BIN_INTEGER_LITERAL() {
  return getToken(PoolParser::BIN_INTEGER_LITERAL, 0);
}

tree::TerminalNode* PoolParser::NumContext::FLOAT_LITERAL() {
  return getToken(PoolParser::FLOAT_LITERAL, 0);
}


size_t PoolParser::NumContext::getRuleIndex() const {
  return PoolParser::RuleNum;
}

void PoolParser::NumContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNum(this);
}

void PoolParser::NumContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNum(this);
}

PoolParser::NumContext* PoolParser::num() {
  NumContext *_localctx = _tracker.createInstance<NumContext>(_ctx, getState());
  enterRule(_localctx, 20, PoolParser::RuleNum);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(186);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::DECIMAL_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(178);
        match(PoolParser::DECIMAL_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::DEC;
        break;
      }

      case PoolParser::HEX_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(180);
        match(PoolParser::HEX_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::HEX;
        break;
      }

      case PoolParser::BIN_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(182);
        match(PoolParser::BIN_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::BIN;
        break;
      }

      case PoolParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(184);
        match(PoolParser::FLOAT_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::FLT;
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StringContext ------------------------------------------------------------------

PoolParser::StringContext::StringContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::StringContext::STRING_LITERAL() {
  return getToken(PoolParser::STRING_LITERAL, 0);
}


size_t PoolParser::StringContext::getRuleIndex() const {
  return PoolParser::RuleString;
}

void PoolParser::StringContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterString(this);
}

void PoolParser::StringContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitString(this);
}

PoolParser::StringContext* PoolParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 22, PoolParser::RuleString);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(188);
    match(PoolParser::STRING_LITERAL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanContext ------------------------------------------------------------------

PoolParser::BooleanContext::BooleanContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::BooleanContext::TRUE() {
  return getToken(PoolParser::TRUE, 0);
}

tree::TerminalNode* PoolParser::BooleanContext::FALSE() {
  return getToken(PoolParser::FALSE, 0);
}


size_t PoolParser::BooleanContext::getRuleIndex() const {
  return PoolParser::RuleBoolean;
}

void PoolParser::BooleanContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBoolean(this);
}

void PoolParser::BooleanContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBoolean(this);
}

PoolParser::BooleanContext* PoolParser::boolean() {
  BooleanContext *_localctx = _tracker.createInstance<BooleanContext>(_ctx, getState());
  enterRule(_localctx, 24, PoolParser::RuleBoolean);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(194);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::TRUE: {
        setState(190);
        match(PoolParser::TRUE);
        dynamic_cast<BooleanContext *>(_localctx)->value =  true;
        break;
      }

      case PoolParser::FALSE: {
        setState(192);
        match(PoolParser::FALSE);
        dynamic_cast<BooleanContext *>(_localctx)->value =  false;
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NullContext ------------------------------------------------------------------

PoolParser::NullContext::NullContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::NullContext::NULL_() {
  return getToken(PoolParser::NULL_, 0);
}


size_t PoolParser::NullContext::getRuleIndex() const {
  return PoolParser::RuleNull;
}

void PoolParser::NullContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNull(this);
}

void PoolParser::NullContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNull(this);
}

PoolParser::NullContext* PoolParser::null() {
  NullContext *_localctx = _tracker.createInstance<NullContext>(_ctx, getState());
  enterRule(_localctx, 26, PoolParser::RuleNull);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(196);
    match(PoolParser::NULL_);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool PoolParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 2: return callSempred(dynamic_cast<CallContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool PoolParser::callSempred(CallContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 7);
    case 2: return precpred(_ctx, 6);
    case 3: return precpred(_ctx, 5);
    case 4: return precpred(_ctx, 4);
    case 5: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> PoolParser::_decisionToDFA;
atn::PredictionContextCache PoolParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN PoolParser::_atn;
std::vector<uint16_t> PoolParser::_serializedATN;

std::vector<std::string> PoolParser::_ruleNames = {
  "program", "statement", "call", "args", "term", "par", "block", "array", 
  "fun", "id", "num", "string", "boolean", "null"
};

std::vector<std::string> PoolParser::_literalNames = {
  "", "", "", "", "';'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
  "','", "':'", "'true'", "'false'", "'null'"
};

std::vector<std::string> PoolParser::_symbolicNames = {
  "", "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", "LSB", 
  "RSB", "DOT", "COMMA", "COLON", "TRUE", "FALSE", "NULL_", "DECIMAL_INTEGER_LITERAL", 
  "HEX_INTEGER_LITERAL", "BIN_INTEGER_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", 
  "IDENTIFIER", "OPERATOR"
};

dfa::Vocabulary PoolParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> PoolParser::_tokenNames;

PoolParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x19, 0xc9, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x4, 
    0xf, 0x9, 0xf, 0x3, 0x2, 0x7, 0x2, 0x20, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 
    0x23, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x5, 0x3, 0x28, 0xa, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x4b, 0xa, 
    0x4, 0xc, 0x4, 0xe, 0x4, 0x4e, 0xb, 0x4, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 
    0x52, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 0x56, 0xa, 0x5, 0xc, 0x5, 
    0xe, 0x5, 0x59, 0xb, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x78, 0xa, 0x6, 0x3, 0x7, 
    0x3, 0x7, 0x5, 0x7, 0x7c, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 
    0x8, 0x7, 0x8, 0x82, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x85, 0xb, 0x8, 0x3, 
    0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x8b, 0xa, 0x9, 0x3, 0x9, 
    0x6, 0x9, 0x8e, 0xa, 0x9, 0xd, 0x9, 0xe, 0x9, 0x8f, 0x3, 0x9, 0x7, 0x9, 
    0x93, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x96, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0x9d, 0xa, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x7, 0xa, 0xa1, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0xa4, 0xb, 0xa, 0x3, 
    0xa, 0x5, 0xa, 0xa7, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 
    0xac, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 0xaf, 0xb, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
    0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0xbd, 0xa, 0xc, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 0xc5, 0xa, 0xe, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x2, 0x3, 0x6, 0x10, 0x2, 0x4, 0x6, 0x8, 
    0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x2, 0x2, 0x2, 
    0xd9, 0x2, 0x21, 0x3, 0x2, 0x2, 0x2, 0x4, 0x27, 0x3, 0x2, 0x2, 0x2, 
    0x6, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x8, 0x4f, 0x3, 0x2, 0x2, 0x2, 0xa, 0x77, 
    0x3, 0x2, 0x2, 0x2, 0xc, 0x79, 0x3, 0x2, 0x2, 0x2, 0xe, 0x7f, 0x3, 0x2, 
    0x2, 0x2, 0x10, 0x88, 0x3, 0x2, 0x2, 0x2, 0x12, 0xa6, 0x3, 0x2, 0x2, 
    0x2, 0x14, 0xb2, 0x3, 0x2, 0x2, 0x2, 0x16, 0xbc, 0x3, 0x2, 0x2, 0x2, 
    0x18, 0xbe, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xc4, 0x3, 0x2, 0x2, 0x2, 0x1c, 
    0xc6, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x20, 0x5, 0x4, 0x3, 0x2, 0x1f, 0x1e, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x23, 0x3, 0x2, 0x2, 0x2, 0x21, 0x1f, 0x3, 
    0x2, 0x2, 0x2, 0x21, 0x22, 0x3, 0x2, 0x2, 0x2, 0x22, 0x24, 0x3, 0x2, 
    0x2, 0x2, 0x23, 0x21, 0x3, 0x2, 0x2, 0x2, 0x24, 0x25, 0x7, 0x2, 0x2, 
    0x3, 0x25, 0x3, 0x3, 0x2, 0x2, 0x2, 0x26, 0x28, 0x5, 0x6, 0x4, 0x2, 
    0x27, 0x26, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x3, 0x2, 0x2, 0x2, 0x28, 
    0x29, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x7, 0x6, 0x2, 0x2, 0x2a, 0x5, 
    0x3, 0x2, 0x2, 0x2, 0x2b, 0x2c, 0x8, 0x4, 0x1, 0x2, 0x2c, 0x2d, 0x5, 
    0xa, 0x6, 0x2, 0x2d, 0x2e, 0x8, 0x4, 0x1, 0x2, 0x2e, 0x4c, 0x3, 0x2, 
    0x2, 0x2, 0x2f, 0x30, 0xc, 0x5, 0x2, 0x2, 0x30, 0x31, 0x7, 0x19, 0x2, 
    0x2, 0x31, 0x32, 0x5, 0x6, 0x4, 0x5, 0x32, 0x33, 0x8, 0x4, 0x1, 0x2, 
    0x33, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0xc, 0x9, 0x2, 0x2, 0x35, 
    0x36, 0x7, 0xd, 0x2, 0x2, 0x36, 0x37, 0x7, 0x18, 0x2, 0x2, 0x37, 0x38, 
    0x5, 0x8, 0x5, 0x2, 0x38, 0x39, 0x8, 0x4, 0x1, 0x2, 0x39, 0x4b, 0x3, 
    0x2, 0x2, 0x2, 0x3a, 0x3b, 0xc, 0x8, 0x2, 0x2, 0x3b, 0x3c, 0x7, 0xd, 
    0x2, 0x2, 0x3c, 0x3d, 0x7, 0x18, 0x2, 0x2, 0x3d, 0x4b, 0x8, 0x4, 0x1, 
    0x2, 0x3e, 0x3f, 0xc, 0x7, 0x2, 0x2, 0x3f, 0x40, 0x5, 0x8, 0x5, 0x2, 
    0x40, 0x41, 0x8, 0x4, 0x1, 0x2, 0x41, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x42, 
    0x43, 0xc, 0x6, 0x2, 0x2, 0x43, 0x44, 0x7, 0x19, 0x2, 0x2, 0x44, 0x45, 
    0x5, 0x8, 0x5, 0x2, 0x45, 0x46, 0x8, 0x4, 0x1, 0x2, 0x46, 0x4b, 0x3, 
    0x2, 0x2, 0x2, 0x47, 0x48, 0xc, 0x4, 0x2, 0x2, 0x48, 0x49, 0x7, 0x19, 
    0x2, 0x2, 0x49, 0x4b, 0x8, 0x4, 0x1, 0x2, 0x4a, 0x2f, 0x3, 0x2, 0x2, 
    0x2, 0x4a, 0x34, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x3a, 0x3, 0x2, 0x2, 0x2, 
    0x4a, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x42, 0x3, 0x2, 0x2, 0x2, 0x4a, 
    0x47, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4a, 
    0x3, 0x2, 0x2, 0x2, 0x4c, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x7, 0x3, 
    0x2, 0x2, 0x2, 0x4e, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x51, 0x7, 0x7, 
    0x2, 0x2, 0x50, 0x52, 0x5, 0x6, 0x4, 0x2, 0x51, 0x50, 0x3, 0x2, 0x2, 
    0x2, 0x51, 0x52, 0x3, 0x2, 0x2, 0x2, 0x52, 0x57, 0x3, 0x2, 0x2, 0x2, 
    0x53, 0x54, 0x7, 0xe, 0x2, 0x2, 0x54, 0x56, 0x5, 0x6, 0x4, 0x2, 0x55, 
    0x53, 0x3, 0x2, 0x2, 0x2, 0x56, 0x59, 0x3, 0x2, 0x2, 0x2, 0x57, 0x55, 
    0x3, 0x2, 0x2, 0x2, 0x57, 0x58, 0x3, 0x2, 0x2, 0x2, 0x58, 0x5a, 0x3, 
    0x2, 0x2, 0x2, 0x59, 0x57, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x8, 
    0x2, 0x2, 0x5b, 0x9, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 0x5, 0x1c, 0xf, 
    0x2, 0x5d, 0x5e, 0x8, 0x6, 0x1, 0x2, 0x5e, 0x78, 0x3, 0x2, 0x2, 0x2, 
    0x5f, 0x60, 0x5, 0x16, 0xc, 0x2, 0x60, 0x61, 0x8, 0x6, 0x1, 0x2, 0x61, 
    0x78, 0x3, 0x2, 0x2, 0x2, 0x62, 0x63, 0x5, 0x1a, 0xe, 0x2, 0x63, 0x64, 
    0x8, 0x6, 0x1, 0x2, 0x64, 0x78, 0x3, 0x2, 0x2, 0x2, 0x65, 0x66, 0x5, 
    0x18, 0xd, 0x2, 0x66, 0x67, 0x8, 0x6, 0x1, 0x2, 0x67, 0x78, 0x3, 0x2, 
    0x2, 0x2, 0x68, 0x69, 0x5, 0x12, 0xa, 0x2, 0x69, 0x6a, 0x8, 0x6, 0x1, 
    0x2, 0x6a, 0x78, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x6c, 0x5, 0x10, 0x9, 0x2, 
    0x6c, 0x6d, 0x8, 0x6, 0x1, 0x2, 0x6d, 0x78, 0x3, 0x2, 0x2, 0x2, 0x6e, 
    0x6f, 0x5, 0xc, 0x7, 0x2, 0x6f, 0x70, 0x8, 0x6, 0x1, 0x2, 0x70, 0x78, 
    0x3, 0x2, 0x2, 0x2, 0x71, 0x72, 0x5, 0xe, 0x8, 0x2, 0x72, 0x73, 0x8, 
    0x6, 0x1, 0x2, 0x73, 0x78, 0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x5, 0x14, 
    0xb, 0x2, 0x75, 0x76, 0x8, 0x6, 0x1, 0x2, 0x76, 0x78, 0x3, 0x2, 0x2, 
    0x2, 0x77, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x77, 0x5f, 0x3, 0x2, 0x2, 0x2, 
    0x77, 0x62, 0x3, 0x2, 0x2, 0x2, 0x77, 0x65, 0x3, 0x2, 0x2, 0x2, 0x77, 
    0x68, 0x3, 0x2, 0x2, 0x2, 0x77, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x77, 0x6e, 
    0x3, 0x2, 0x2, 0x2, 0x77, 0x71, 0x3, 0x2, 0x2, 0x2, 0x77, 0x74, 0x3, 
    0x2, 0x2, 0x2, 0x78, 0xb, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7b, 0x7, 0x7, 
    0x2, 0x2, 0x7a, 0x7c, 0x5, 0x6, 0x4, 0x2, 0x7b, 0x7a, 0x3, 0x2, 0x2, 
    0x2, 0x7b, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7d, 0x3, 0x2, 0x2, 0x2, 
    0x7d, 0x7e, 0x7, 0x8, 0x2, 0x2, 0x7e, 0xd, 0x3, 0x2, 0x2, 0x2, 0x7f, 
    0x83, 0x7, 0x9, 0x2, 0x2, 0x80, 0x82, 0x5, 0x4, 0x3, 0x2, 0x81, 0x80, 
    0x3, 0x2, 0x2, 0x2, 0x82, 0x85, 0x3, 0x2, 0x2, 0x2, 0x83, 0x81, 0x3, 
    0x2, 0x2, 0x2, 0x83, 0x84, 0x3, 0x2, 0x2, 0x2, 0x84, 0x86, 0x3, 0x2, 
    0x2, 0x2, 0x85, 0x83, 0x3, 0x2, 0x2, 0x2, 0x86, 0x87, 0x7, 0xa, 0x2, 
    0x2, 0x87, 0xf, 0x3, 0x2, 0x2, 0x2, 0x88, 0x8a, 0x7, 0xb, 0x2, 0x2, 
    0x89, 0x8b, 0x5, 0x6, 0x4, 0x2, 0x8a, 0x89, 0x3, 0x2, 0x2, 0x2, 0x8a, 
    0x8b, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x94, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8e, 
    0x7, 0xe, 0x2, 0x2, 0x8d, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 0x3, 
    0x2, 0x2, 0x2, 0x8f, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 0x3, 0x2, 
    0x2, 0x2, 0x90, 0x91, 0x3, 0x2, 0x2, 0x2, 0x91, 0x93, 0x5, 0x6, 0x4, 
    0x2, 0x92, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x93, 0x96, 0x3, 0x2, 0x2, 0x2, 
    0x94, 0x92, 0x3, 0x2, 0x2, 0x2, 0x94, 0x95, 0x3, 0x2, 0x2, 0x2, 0x95, 
    0x97, 0x3, 0x2, 0x2, 0x2, 0x96, 0x94, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 
    0x7, 0xc, 0x2, 0x2, 0x98, 0x11, 0x3, 0x2, 0x2, 0x2, 0x99, 0xa7, 0x7, 
    0x18, 0x2, 0x2, 0x9a, 0x9c, 0x7, 0x7, 0x2, 0x2, 0x9b, 0x9d, 0x7, 0x18, 
    0x2, 0x2, 0x9c, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x9d, 0x3, 0x2, 0x2, 
    0x2, 0x9d, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x9f, 0x7, 0xe, 0x2, 0x2, 
    0x9f, 0xa1, 0x7, 0x18, 0x2, 0x2, 0xa0, 0x9e, 0x3, 0x2, 0x2, 0x2, 0xa1, 
    0xa4, 0x3, 0x2, 0x2, 0x2, 0xa2, 0xa0, 0x3, 0x2, 0x2, 0x2, 0xa2, 0xa3, 
    0x3, 0x2, 0x2, 0x2, 0xa3, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa2, 0x3, 
    0x2, 0x2, 0x2, 0xa5, 0xa7, 0x7, 0x8, 0x2, 0x2, 0xa6, 0x99, 0x3, 0x2, 
    0x2, 0x2, 0xa6, 0x9a, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 
    0x2, 0xa8, 0xa9, 0x7, 0xf, 0x2, 0x2, 0xa9, 0xad, 0x7, 0x9, 0x2, 0x2, 
    0xaa, 0xac, 0x5, 0x4, 0x3, 0x2, 0xab, 0xaa, 0x3, 0x2, 0x2, 0x2, 0xac, 
    0xaf, 0x3, 0x2, 0x2, 0x2, 0xad, 0xab, 0x3, 0x2, 0x2, 0x2, 0xad, 0xae, 
    0x3, 0x2, 0x2, 0x2, 0xae, 0xb0, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xad, 0x3, 
    0x2, 0x2, 0x2, 0xb0, 0xb1, 0x7, 0xa, 0x2, 0x2, 0xb1, 0x13, 0x3, 0x2, 
    0x2, 0x2, 0xb2, 0xb3, 0x7, 0x18, 0x2, 0x2, 0xb3, 0x15, 0x3, 0x2, 0x2, 
    0x2, 0xb4, 0xb5, 0x7, 0x13, 0x2, 0x2, 0xb5, 0xbd, 0x8, 0xc, 0x1, 0x2, 
    0xb6, 0xb7, 0x7, 0x14, 0x2, 0x2, 0xb7, 0xbd, 0x8, 0xc, 0x1, 0x2, 0xb8, 
    0xb9, 0x7, 0x15, 0x2, 0x2, 0xb9, 0xbd, 0x8, 0xc, 0x1, 0x2, 0xba, 0xbb, 
    0x7, 0x16, 0x2, 0x2, 0xbb, 0xbd, 0x8, 0xc, 0x1, 0x2, 0xbc, 0xb4, 0x3, 
    0x2, 0x2, 0x2, 0xbc, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xb8, 0x3, 0x2, 
    0x2, 0x2, 0xbc, 0xba, 0x3, 0x2, 0x2, 0x2, 0xbd, 0x17, 0x3, 0x2, 0x2, 
    0x2, 0xbe, 0xbf, 0x7, 0x17, 0x2, 0x2, 0xbf, 0x19, 0x3, 0x2, 0x2, 0x2, 
    0xc0, 0xc1, 0x7, 0x10, 0x2, 0x2, 0xc1, 0xc5, 0x8, 0xe, 0x1, 0x2, 0xc2, 
    0xc3, 0x7, 0x11, 0x2, 0x2, 0xc3, 0xc5, 0x8, 0xe, 0x1, 0x2, 0xc4, 0xc0, 
    0x3, 0x2, 0x2, 0x2, 0xc4, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc5, 0x1b, 0x3, 
    0x2, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x12, 0x2, 0x2, 0xc7, 0x1d, 0x3, 0x2, 
    0x2, 0x2, 0x14, 0x21, 0x27, 0x4a, 0x4c, 0x51, 0x57, 0x77, 0x7b, 0x83, 
    0x8a, 0x8f, 0x94, 0x9c, 0xa2, 0xa6, 0xad, 0xbc, 0xc4, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

PoolParser::Initializer PoolParser::_init;
