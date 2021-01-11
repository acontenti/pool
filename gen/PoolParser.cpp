
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


//----------------- PoolContext ------------------------------------------------------------------

PoolParser::PoolContext::PoolContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<PoolParser::StatementContext *> PoolParser::PoolContext::statement() {
  return getRuleContexts<PoolParser::StatementContext>();
}

PoolParser::StatementContext* PoolParser::PoolContext::statement(size_t i) {
  return getRuleContext<PoolParser::StatementContext>(i);
}


size_t PoolParser::PoolContext::getRuleIndex() const {
  return PoolParser::RulePool;
}

void PoolParser::PoolContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPool(this);
}

void PoolParser::PoolContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPool(this);
}

PoolParser::PoolContext* PoolParser::pool() {
  PoolContext *_localctx = _tracker.createInstance<PoolContext>(_ctx, getState());
  enterRule(_localctx, 0, PoolParser::RulePool);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(36);
      statement();
      setState(41);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
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
    setState(43);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(42);
      call();
    }
    setState(45);
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

tree::TerminalNode* PoolParser::CallContext::OPERATOR() {
  return getToken(PoolParser::OPERATOR, 0);
}

std::vector<PoolParser::InvocationContext *> PoolParser::CallContext::invocation() {
  return getRuleContexts<PoolParser::InvocationContext>();
}

PoolParser::InvocationContext* PoolParser::CallContext::invocation(size_t i) {
  return getRuleContext<PoolParser::InvocationContext>(i);
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
  CallContext *_localctx = _tracker.createInstance<CallContext>(_ctx, getState());
  enterRule(_localctx, 4, PoolParser::RuleCall);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(62);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(48);
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
        setState(47);
        dynamic_cast<CallContext *>(_localctx)->caller = invocation();
      }
      setState(50);
      dynamic_cast<CallContext *>(_localctx)->op = match(PoolParser::OPERATOR);
      setState(51);
      dynamic_cast<CallContext *>(_localctx)->callee = invocation();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(52);
      dynamic_cast<CallContext *>(_localctx)->caller = invocation();
      setState(53);
      dynamic_cast<CallContext *>(_localctx)->op = match(PoolParser::OPERATOR);
      setState(55);
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
        setState(54);
        dynamic_cast<CallContext *>(_localctx)->callee = invocation();
      }
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(57);
      dynamic_cast<CallContext *>(_localctx)->caller = invocation();
      setState(60);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == PoolParser::OPERATOR) {
        setState(58);
        dynamic_cast<CallContext *>(_localctx)->op = match(PoolParser::OPERATOR);
        setState(59);
        dynamic_cast<CallContext *>(_localctx)->callee = invocation();
      }
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

//----------------- InvocationContext ------------------------------------------------------------------

PoolParser::InvocationContext::InvocationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PoolParser::AccessContext* PoolParser::InvocationContext::access() {
  return getRuleContext<PoolParser::AccessContext>(0);
}

PoolParser::ArgsContext* PoolParser::InvocationContext::args() {
  return getRuleContext<PoolParser::ArgsContext>(0);
}


size_t PoolParser::InvocationContext::getRuleIndex() const {
  return PoolParser::RuleInvocation;
}

void PoolParser::InvocationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInvocation(this);
}

void PoolParser::InvocationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInvocation(this);
}

PoolParser::InvocationContext* PoolParser::invocation() {
  InvocationContext *_localctx = _tracker.createInstance<InvocationContext>(_ctx, getState());
  enterRule(_localctx, 6, PoolParser::RuleInvocation);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    access();
    setState(66);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::LP) {
      setState(65);
      args();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AccessContext ------------------------------------------------------------------

PoolParser::AccessContext::AccessContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PoolParser::TermContext* PoolParser::AccessContext::term() {
  return getRuleContext<PoolParser::TermContext>(0);
}

std::vector<tree::TerminalNode *> PoolParser::AccessContext::DOT() {
  return getTokens(PoolParser::DOT);
}

tree::TerminalNode* PoolParser::AccessContext::DOT(size_t i) {
  return getToken(PoolParser::DOT, i);
}

std::vector<tree::TerminalNode *> PoolParser::AccessContext::IDENTIFIER() {
  return getTokens(PoolParser::IDENTIFIER);
}

tree::TerminalNode* PoolParser::AccessContext::IDENTIFIER(size_t i) {
  return getToken(PoolParser::IDENTIFIER, i);
}


size_t PoolParser::AccessContext::getRuleIndex() const {
  return PoolParser::RuleAccess;
}

void PoolParser::AccessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAccess(this);
}

void PoolParser::AccessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAccess(this);
}

PoolParser::AccessContext* PoolParser::access() {
  AccessContext *_localctx = _tracker.createInstance<AccessContext>(_ctx, getState());
  enterRule(_localctx, 8, PoolParser::RuleAccess);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    term();
    setState(73);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::DOT) {
      setState(69);
      match(PoolParser::DOT);
      setState(70);
      match(PoolParser::IDENTIFIER);
      setState(75);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
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

PoolParser::TupleContext* PoolParser::TermContext::tuple() {
  return getRuleContext<PoolParser::TupleContext>(0);
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
  enterRule(_localctx, 10, PoolParser::RuleTerm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(106);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(76);
      null();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NIL;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(79);
      num();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NUM;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(82);
      boolean();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::BLN;
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(85);
      string();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::STR;
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(88);
      fun();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::FUN;
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(91);
      tuple();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::TPL;
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(94);
      array();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::ARR;
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(97);
      par();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::PAR;
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(100);
      block();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::BLK;
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);
      setState(103);
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
  enterRule(_localctx, 12, PoolParser::RulePar);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(108);
    match(PoolParser::LP);
    setState(110);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(109);
      call();
    }
    setState(112);
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
  enterRule(_localctx, 14, PoolParser::RuleBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(114);
    match(PoolParser::LB);
    setState(118);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(115);
      statement();
      setState(120);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(121);
    match(PoolParser::RB);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TupleContext ------------------------------------------------------------------

PoolParser::TupleContext::TupleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::TupleContext::LP() {
  return getToken(PoolParser::LP, 0);
}

tree::TerminalNode* PoolParser::TupleContext::RP() {
  return getToken(PoolParser::RP, 0);
}

std::vector<PoolParser::CallContext *> PoolParser::TupleContext::call() {
  return getRuleContexts<PoolParser::CallContext>();
}

PoolParser::CallContext* PoolParser::TupleContext::call(size_t i) {
  return getRuleContext<PoolParser::CallContext>(i);
}

std::vector<tree::TerminalNode *> PoolParser::TupleContext::COMMA() {
  return getTokens(PoolParser::COMMA);
}

tree::TerminalNode* PoolParser::TupleContext::COMMA(size_t i) {
  return getToken(PoolParser::COMMA, i);
}


size_t PoolParser::TupleContext::getRuleIndex() const {
  return PoolParser::RuleTuple;
}

void PoolParser::TupleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTuple(this);
}

void PoolParser::TupleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTuple(this);
}

PoolParser::TupleContext* PoolParser::tuple() {
  TupleContext *_localctx = _tracker.createInstance<TupleContext>(_ctx, getState());
  enterRule(_localctx, 16, PoolParser::RuleTuple);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(123);
    match(PoolParser::LP);
    setState(127); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(124);
              call();
              setState(125);
              match(PoolParser::COMMA);
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(129); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
    setState(132);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(131);
      call();
    }
    setState(134);
    match(PoolParser::RP);
   
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
  enterRule(_localctx, 18, PoolParser::RuleArray);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(PoolParser::LSB);
    setState(138);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(137);
      call();
    }
    setState(148);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(141); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(140);
        match(PoolParser::COMMA);
        setState(143); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while (_la == PoolParser::COMMA);
      setState(145);
      call();
      setState(150);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(151);
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

PoolParser::ParamsContext* PoolParser::FunContext::params() {
  return getRuleContext<PoolParser::ParamsContext>(0);
}

tree::TerminalNode* PoolParser::FunContext::COLON() {
  return getToken(PoolParser::COLON, 0);
}

PoolParser::BlockContext* PoolParser::FunContext::block() {
  return getRuleContext<PoolParser::BlockContext>(0);
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
  enterRule(_localctx, 20, PoolParser::RuleFun);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(153);
    params();
    setState(154);
    match(PoolParser::COLON);
    setState(155);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamsContext ------------------------------------------------------------------

PoolParser::ParamsContext::ParamsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> PoolParser::ParamsContext::IDENTIFIER() {
  return getTokens(PoolParser::IDENTIFIER);
}

tree::TerminalNode* PoolParser::ParamsContext::IDENTIFIER(size_t i) {
  return getToken(PoolParser::IDENTIFIER, i);
}

tree::TerminalNode* PoolParser::ParamsContext::LP() {
  return getToken(PoolParser::LP, 0);
}

tree::TerminalNode* PoolParser::ParamsContext::RP() {
  return getToken(PoolParser::RP, 0);
}

std::vector<tree::TerminalNode *> PoolParser::ParamsContext::COMMA() {
  return getTokens(PoolParser::COMMA);
}

tree::TerminalNode* PoolParser::ParamsContext::COMMA(size_t i) {
  return getToken(PoolParser::COMMA, i);
}


size_t PoolParser::ParamsContext::getRuleIndex() const {
  return PoolParser::RuleParams;
}

void PoolParser::ParamsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParams(this);
}

void PoolParser::ParamsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<PoolParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParams(this);
}

PoolParser::ParamsContext* PoolParser::params() {
  ParamsContext *_localctx = _tracker.createInstance<ParamsContext>(_ctx, getState());
  enterRule(_localctx, 22, PoolParser::RuleParams);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(170);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::IDENTIFIER: {
        enterOuterAlt(_localctx, 1);
        setState(157);
        match(PoolParser::IDENTIFIER);
        break;
      }

      case PoolParser::LP: {
        enterOuterAlt(_localctx, 2);
        setState(158);
        match(PoolParser::LP);
        setState(160);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == PoolParser::IDENTIFIER) {
          setState(159);
          match(PoolParser::IDENTIFIER);
        }
        setState(166);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == PoolParser::COMMA) {
          setState(162);
          match(PoolParser::COMMA);
          setState(163);
          match(PoolParser::IDENTIFIER);
          setState(168);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(169);
        match(PoolParser::RP);
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
  enterRule(_localctx, 24, PoolParser::RuleArgs);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(172);
    match(PoolParser::LP);
    setState(174);
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
      | (1ULL << PoolParser::IDENTIFIER)
      | (1ULL << PoolParser::OPERATOR))) != 0)) {
      setState(173);
      call();
    }
    setState(180);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(176);
      match(PoolParser::COMMA);
      setState(177);
      call();
      setState(182);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(183);
    match(PoolParser::RP);
   
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
  enterRule(_localctx, 26, PoolParser::RuleId);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(185);
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
  enterRule(_localctx, 28, PoolParser::RuleNum);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(195);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::DECIMAL_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(187);
        match(PoolParser::DECIMAL_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::DEC;
        break;
      }

      case PoolParser::HEX_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(189);
        match(PoolParser::HEX_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::HEX;
        break;
      }

      case PoolParser::BIN_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(191);
        match(PoolParser::BIN_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::BIN;
        break;
      }

      case PoolParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(193);
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
  enterRule(_localctx, 30, PoolParser::RuleString);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(197);
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
  enterRule(_localctx, 32, PoolParser::RuleBoolean);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(203);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::TRUE: {
        setState(199);
        match(PoolParser::TRUE);
        dynamic_cast<BooleanContext *>(_localctx)->value =  true;
        break;
      }

      case PoolParser::FALSE: {
        setState(201);
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
  enterRule(_localctx, 34, PoolParser::RuleNull);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(205);
    match(PoolParser::NULL_);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> PoolParser::_decisionToDFA;
atn::PredictionContextCache PoolParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN PoolParser::_atn;
std::vector<uint16_t> PoolParser::_serializedATN;

std::vector<std::string> PoolParser::_ruleNames = {
  "pool", "statement", "call", "invocation", "access", "term", "par", "block", 
  "tuple", "array", "fun", "params", "args", "id", "num", "string", "boolean", 
  "null"
};

std::vector<std::string> PoolParser::_literalNames = {
  "", "", "", "", "';'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
  "','", "':'", "'true'", "'false'", "'null'"
};

std::vector<std::string> PoolParser::_symbolicNames = {
  "", "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", "LSB", 
  "RSB", "DOT", "COMMA", "COLON", "TRUE", "FALSE", "NULL_", "DECIMAL_INTEGER_LITERAL", 
  "HEX_INTEGER_LITERAL", "BIN_INTEGER_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", 
  "IDENTIFIER", "OPERATOR", "IDENT_PART", "SYMBOL"
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
    0x3, 0x1b, 0xd2, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x4, 
    0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 0x12, 
    0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x3, 0x2, 0x7, 0x2, 0x28, 0xa, 0x2, 
    0xc, 0x2, 0xe, 0x2, 0x2b, 0xb, 0x2, 0x3, 0x3, 0x5, 0x3, 0x2e, 0xa, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x5, 0x4, 0x33, 0xa, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x3a, 0xa, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x3f, 0xa, 0x4, 0x5, 0x4, 0x41, 0xa, 0x4, 
    0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x45, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x7, 0x6, 0x4a, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0x4d, 0xb, 0x6, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x5, 0x7, 0x6d, 0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 
    0x71, 0xa, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x77, 
    0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x7a, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x6, 0xa, 0x82, 0xa, 0xa, 0xd, 0xa, 
    0xe, 0xa, 0x83, 0x3, 0xa, 0x5, 0xa, 0x87, 0xa, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0x8d, 0xa, 0xb, 0x3, 0xb, 0x6, 0xb, 0x90, 
    0xa, 0xb, 0xd, 0xb, 0xe, 0xb, 0x91, 0x3, 0xb, 0x7, 0xb, 0x95, 0xa, 0xb, 
    0xc, 0xb, 0xe, 0xb, 0x98, 0xb, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 
    0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x5, 0xd, 0xa3, 
    0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xa7, 0xa, 0xd, 0xc, 0xd, 0xe, 
    0xd, 0xaa, 0xb, 0xd, 0x3, 0xd, 0x5, 0xd, 0xad, 0xa, 0xd, 0x3, 0xe, 0x3, 
    0xe, 0x5, 0xe, 0xb1, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0xb5, 0xa, 
    0xe, 0xc, 0xe, 0xe, 0xe, 0xb8, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xc6, 0xa, 0x10, 0x3, 0x11, 0x3, 
    0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 0xce, 0xa, 
    0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x2, 0x2, 0x14, 0x2, 0x4, 0x6, 
    0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 
    0x20, 0x22, 0x24, 0x2, 0x2, 0x2, 0xe1, 0x2, 0x29, 0x3, 0x2, 0x2, 0x2, 
    0x4, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x6, 0x40, 0x3, 0x2, 0x2, 0x2, 0x8, 0x42, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0x46, 0x3, 0x2, 0x2, 0x2, 0xc, 0x6c, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x10, 0x74, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x14, 0x8a, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x18, 0xac, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0xae, 0x3, 0x2, 0x2, 0x2, 0x1c, 0xbb, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xc5, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0xc7, 0x3, 0x2, 0x2, 0x2, 0x22, 0xcd, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0xcf, 0x3, 0x2, 0x2, 0x2, 0x26, 0x28, 0x5, 0x4, 
    0x3, 0x2, 0x27, 0x26, 0x3, 0x2, 0x2, 0x2, 0x28, 0x2b, 0x3, 0x2, 0x2, 
    0x2, 0x29, 0x27, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x3, 0x2, 0x2, 0x2, 
    0x2a, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x29, 0x3, 0x2, 0x2, 0x2, 0x2c, 
    0x2e, 0x5, 0x6, 0x4, 0x2, 0x2d, 0x2c, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2e, 
    0x3, 0x2, 0x2, 0x2, 0x2e, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x30, 0x7, 
    0x6, 0x2, 0x2, 0x30, 0x5, 0x3, 0x2, 0x2, 0x2, 0x31, 0x33, 0x5, 0x8, 
    0x5, 0x2, 0x32, 0x31, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x3, 0x2, 0x2, 
    0x2, 0x33, 0x34, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 0x7, 0x19, 0x2, 0x2, 
    0x35, 0x41, 0x5, 0x8, 0x5, 0x2, 0x36, 0x37, 0x5, 0x8, 0x5, 0x2, 0x37, 
    0x39, 0x7, 0x19, 0x2, 0x2, 0x38, 0x3a, 0x5, 0x8, 0x5, 0x2, 0x39, 0x38, 
    0x3, 0x2, 0x2, 0x2, 0x39, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x41, 0x3, 
    0x2, 0x2, 0x2, 0x3b, 0x3e, 0x5, 0x8, 0x5, 0x2, 0x3c, 0x3d, 0x7, 0x19, 
    0x2, 0x2, 0x3d, 0x3f, 0x5, 0x8, 0x5, 0x2, 0x3e, 0x3c, 0x3, 0x2, 0x2, 
    0x2, 0x3e, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x41, 0x3, 0x2, 0x2, 0x2, 
    0x40, 0x32, 0x3, 0x2, 0x2, 0x2, 0x40, 0x36, 0x3, 0x2, 0x2, 0x2, 0x40, 
    0x3b, 0x3, 0x2, 0x2, 0x2, 0x41, 0x7, 0x3, 0x2, 0x2, 0x2, 0x42, 0x44, 
    0x5, 0xa, 0x6, 0x2, 0x43, 0x45, 0x5, 0x1a, 0xe, 0x2, 0x44, 0x43, 0x3, 
    0x2, 0x2, 0x2, 0x44, 0x45, 0x3, 0x2, 0x2, 0x2, 0x45, 0x9, 0x3, 0x2, 
    0x2, 0x2, 0x46, 0x4b, 0x5, 0xc, 0x7, 0x2, 0x47, 0x48, 0x7, 0xd, 0x2, 
    0x2, 0x48, 0x4a, 0x7, 0x18, 0x2, 0x2, 0x49, 0x47, 0x3, 0x2, 0x2, 0x2, 
    0x4a, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x49, 0x3, 0x2, 0x2, 0x2, 0x4b, 
    0x4c, 0x3, 0x2, 0x2, 0x2, 0x4c, 0xb, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4b, 
    0x3, 0x2, 0x2, 0x2, 0x4e, 0x4f, 0x5, 0x24, 0x13, 0x2, 0x4f, 0x50, 0x8, 
    0x7, 0x1, 0x2, 0x50, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x51, 0x52, 0x5, 0x1e, 
    0x10, 0x2, 0x52, 0x53, 0x8, 0x7, 0x1, 0x2, 0x53, 0x6d, 0x3, 0x2, 0x2, 
    0x2, 0x54, 0x55, 0x5, 0x22, 0x12, 0x2, 0x55, 0x56, 0x8, 0x7, 0x1, 0x2, 
    0x56, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x57, 0x58, 0x5, 0x20, 0x11, 0x2, 0x58, 
    0x59, 0x8, 0x7, 0x1, 0x2, 0x59, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 
    0x5, 0x16, 0xc, 0x2, 0x5b, 0x5c, 0x8, 0x7, 0x1, 0x2, 0x5c, 0x6d, 0x3, 
    0x2, 0x2, 0x2, 0x5d, 0x5e, 0x5, 0x12, 0xa, 0x2, 0x5e, 0x5f, 0x8, 0x7, 
    0x1, 0x2, 0x5f, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x60, 0x61, 0x5, 0x14, 0xb, 
    0x2, 0x61, 0x62, 0x8, 0x7, 0x1, 0x2, 0x62, 0x6d, 0x3, 0x2, 0x2, 0x2, 
    0x63, 0x64, 0x5, 0xe, 0x8, 0x2, 0x64, 0x65, 0x8, 0x7, 0x1, 0x2, 0x65, 
    0x6d, 0x3, 0x2, 0x2, 0x2, 0x66, 0x67, 0x5, 0x10, 0x9, 0x2, 0x67, 0x68, 
    0x8, 0x7, 0x1, 0x2, 0x68, 0x6d, 0x3, 0x2, 0x2, 0x2, 0x69, 0x6a, 0x5, 
    0x1c, 0xf, 0x2, 0x6a, 0x6b, 0x8, 0x7, 0x1, 0x2, 0x6b, 0x6d, 0x3, 0x2, 
    0x2, 0x2, 0x6c, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x51, 0x3, 0x2, 0x2, 
    0x2, 0x6c, 0x54, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x57, 0x3, 0x2, 0x2, 0x2, 
    0x6c, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x6c, 
    0x60, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x63, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x66, 
    0x3, 0x2, 0x2, 0x2, 0x6c, 0x69, 0x3, 0x2, 0x2, 0x2, 0x6d, 0xd, 0x3, 
    0x2, 0x2, 0x2, 0x6e, 0x70, 0x7, 0x7, 0x2, 0x2, 0x6f, 0x71, 0x5, 0x6, 
    0x4, 0x2, 0x70, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 0x3, 0x2, 0x2, 
    0x2, 0x71, 0x72, 0x3, 0x2, 0x2, 0x2, 0x72, 0x73, 0x7, 0x8, 0x2, 0x2, 
    0x73, 0xf, 0x3, 0x2, 0x2, 0x2, 0x74, 0x78, 0x7, 0x9, 0x2, 0x2, 0x75, 
    0x77, 0x5, 0x4, 0x3, 0x2, 0x76, 0x75, 0x3, 0x2, 0x2, 0x2, 0x77, 0x7a, 
    0x3, 0x2, 0x2, 0x2, 0x78, 0x76, 0x3, 0x2, 0x2, 0x2, 0x78, 0x79, 0x3, 
    0x2, 0x2, 0x2, 0x79, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x78, 0x3, 0x2, 
    0x2, 0x2, 0x7b, 0x7c, 0x7, 0xa, 0x2, 0x2, 0x7c, 0x11, 0x3, 0x2, 0x2, 
    0x2, 0x7d, 0x81, 0x7, 0x7, 0x2, 0x2, 0x7e, 0x7f, 0x5, 0x6, 0x4, 0x2, 
    0x7f, 0x80, 0x7, 0xe, 0x2, 0x2, 0x80, 0x82, 0x3, 0x2, 0x2, 0x2, 0x81, 
    0x7e, 0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x3, 0x2, 0x2, 0x2, 0x83, 0x81, 
    0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 0x3, 0x2, 0x2, 0x2, 0x84, 0x86, 0x3, 
    0x2, 0x2, 0x2, 0x85, 0x87, 0x5, 0x6, 0x4, 0x2, 0x86, 0x85, 0x3, 0x2, 
    0x2, 0x2, 0x86, 0x87, 0x3, 0x2, 0x2, 0x2, 0x87, 0x88, 0x3, 0x2, 0x2, 
    0x2, 0x88, 0x89, 0x7, 0x8, 0x2, 0x2, 0x89, 0x13, 0x3, 0x2, 0x2, 0x2, 
    0x8a, 0x8c, 0x7, 0xb, 0x2, 0x2, 0x8b, 0x8d, 0x5, 0x6, 0x4, 0x2, 0x8c, 
    0x8b, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x96, 
    0x3, 0x2, 0x2, 0x2, 0x8e, 0x90, 0x7, 0xe, 0x2, 0x2, 0x8f, 0x8e, 0x3, 
    0x2, 0x2, 0x2, 0x90, 0x91, 0x3, 0x2, 0x2, 0x2, 0x91, 0x8f, 0x3, 0x2, 
    0x2, 0x2, 0x91, 0x92, 0x3, 0x2, 0x2, 0x2, 0x92, 0x93, 0x3, 0x2, 0x2, 
    0x2, 0x93, 0x95, 0x5, 0x6, 0x4, 0x2, 0x94, 0x8f, 0x3, 0x2, 0x2, 0x2, 
    0x95, 0x98, 0x3, 0x2, 0x2, 0x2, 0x96, 0x94, 0x3, 0x2, 0x2, 0x2, 0x96, 
    0x97, 0x3, 0x2, 0x2, 0x2, 0x97, 0x99, 0x3, 0x2, 0x2, 0x2, 0x98, 0x96, 
    0x3, 0x2, 0x2, 0x2, 0x99, 0x9a, 0x7, 0xc, 0x2, 0x2, 0x9a, 0x15, 0x3, 
    0x2, 0x2, 0x2, 0x9b, 0x9c, 0x5, 0x18, 0xd, 0x2, 0x9c, 0x9d, 0x7, 0xf, 
    0x2, 0x2, 0x9d, 0x9e, 0x5, 0x10, 0x9, 0x2, 0x9e, 0x17, 0x3, 0x2, 0x2, 
    0x2, 0x9f, 0xad, 0x7, 0x18, 0x2, 0x2, 0xa0, 0xa2, 0x7, 0x7, 0x2, 0x2, 
    0xa1, 0xa3, 0x7, 0x18, 0x2, 0x2, 0xa2, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa2, 
    0xa3, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 
    0x7, 0xe, 0x2, 0x2, 0xa5, 0xa7, 0x7, 0x18, 0x2, 0x2, 0xa6, 0xa4, 0x3, 
    0x2, 0x2, 0x2, 0xa7, 0xaa, 0x3, 0x2, 0x2, 0x2, 0xa8, 0xa6, 0x3, 0x2, 
    0x2, 0x2, 0xa8, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xab, 0x3, 0x2, 0x2, 
    0x2, 0xaa, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xab, 0xad, 0x7, 0x8, 0x2, 0x2, 
    0xac, 0x9f, 0x3, 0x2, 0x2, 0x2, 0xac, 0xa0, 0x3, 0x2, 0x2, 0x2, 0xad, 
    0x19, 0x3, 0x2, 0x2, 0x2, 0xae, 0xb0, 0x7, 0x7, 0x2, 0x2, 0xaf, 0xb1, 
    0x5, 0x6, 0x4, 0x2, 0xb0, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb1, 0x3, 
    0x2, 0x2, 0x2, 0xb1, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xb3, 0x7, 0xe, 
    0x2, 0x2, 0xb3, 0xb5, 0x5, 0x6, 0x4, 0x2, 0xb4, 0xb2, 0x3, 0x2, 0x2, 
    0x2, 0xb5, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb4, 0x3, 0x2, 0x2, 0x2, 
    0xb6, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xb8, 
    0xb6, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xba, 0x7, 0x8, 0x2, 0x2, 0xba, 0x1b, 
    0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0x7, 0x18, 0x2, 0x2, 0xbc, 0x1d, 0x3, 
    0x2, 0x2, 0x2, 0xbd, 0xbe, 0x7, 0x13, 0x2, 0x2, 0xbe, 0xc6, 0x8, 0x10, 
    0x1, 0x2, 0xbf, 0xc0, 0x7, 0x14, 0x2, 0x2, 0xc0, 0xc6, 0x8, 0x10, 0x1, 
    0x2, 0xc1, 0xc2, 0x7, 0x15, 0x2, 0x2, 0xc2, 0xc6, 0x8, 0x10, 0x1, 0x2, 
    0xc3, 0xc4, 0x7, 0x16, 0x2, 0x2, 0xc4, 0xc6, 0x8, 0x10, 0x1, 0x2, 0xc5, 
    0xbd, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc1, 
    0x3, 0x2, 0x2, 0x2, 0xc5, 0xc3, 0x3, 0x2, 0x2, 0x2, 0xc6, 0x1f, 0x3, 
    0x2, 0x2, 0x2, 0xc7, 0xc8, 0x7, 0x17, 0x2, 0x2, 0xc8, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0xc9, 0xca, 0x7, 0x10, 0x2, 0x2, 0xca, 0xce, 0x8, 0x12, 0x1, 
    0x2, 0xcb, 0xcc, 0x7, 0x11, 0x2, 0x2, 0xcc, 0xce, 0x8, 0x12, 0x1, 0x2, 
    0xcd, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xce, 
    0x23, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 0x7, 0x12, 0x2, 0x2, 0xd0, 0x25, 
    0x3, 0x2, 0x2, 0x2, 0x19, 0x29, 0x2d, 0x32, 0x39, 0x3e, 0x40, 0x44, 
    0x4b, 0x6c, 0x70, 0x78, 0x83, 0x86, 0x8c, 0x91, 0x96, 0xa2, 0xa8, 0xac, 
    0xb0, 0xb6, 0xc5, 0xcd, 
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
