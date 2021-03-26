
// Generated from PoolParser.g4 by ANTLR 4.9.2



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


PoolParser::ProgramContext* PoolParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, PoolParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(33);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(30);
      statement();
      setState(35);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(36);
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

PoolParser::ExpressionContext* PoolParser::StatementContext::expression() {
  return getRuleContext<PoolParser::ExpressionContext>(0);
}


size_t PoolParser::StatementContext::getRuleIndex() const {
  return PoolParser::RuleStatement;
}


PoolParser::StatementContext* PoolParser::statement() {
  StatementContext *_localctx = _tracker.createInstance<StatementContext>(_ctx, getState());
  enterRule(_localctx, 2, PoolParser::RuleStatement);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(39);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(38);
      expression();
    }
    setState(41);
    match(PoolParser::SEMI);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpressionContext ------------------------------------------------------------------

PoolParser::ExpressionContext::ExpressionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PoolParser::AssignmentContext* PoolParser::ExpressionContext::assignment() {
  return getRuleContext<PoolParser::AssignmentContext>(0);
}

PoolParser::CallContext* PoolParser::ExpressionContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}


size_t PoolParser::ExpressionContext::getRuleIndex() const {
  return PoolParser::RuleExpression;
}


PoolParser::ExpressionContext* PoolParser::expression() {
  ExpressionContext *_localctx = _tracker.createInstance<ExpressionContext>(_ctx, getState());
  enterRule(_localctx, 4, PoolParser::RuleExpression);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(45);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(43);
      dynamic_cast<ExpressionContext *>(_localctx)->a = assignment();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(44);
      dynamic_cast<ExpressionContext *>(_localctx)->c = call(0);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignmentContext ------------------------------------------------------------------

PoolParser::AssignmentContext::AssignmentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::AssignmentContext::EQ() {
  return getToken(PoolParser::EQ, 0);
}

PoolParser::AccessContext* PoolParser::AssignmentContext::access() {
  return getRuleContext<PoolParser::AccessContext>(0);
}

PoolParser::CallContext* PoolParser::AssignmentContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}

tree::TerminalNode* PoolParser::AssignmentContext::CEQ() {
  return getToken(PoolParser::CEQ, 0);
}


size_t PoolParser::AssignmentContext::getRuleIndex() const {
  return PoolParser::RuleAssignment;
}


PoolParser::AssignmentContext* PoolParser::assignment() {
  AssignmentContext *_localctx = _tracker.createInstance<AssignmentContext>(_ctx, getState());
  enterRule(_localctx, 6, PoolParser::RuleAssignment);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(57);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(47);
      dynamic_cast<AssignmentContext *>(_localctx)->assignee = access();
      setState(48);
      match(PoolParser::EQ);
      setState(49);
      dynamic_cast<AssignmentContext *>(_localctx)->value = call(0);
      dynamic_cast<AssignmentContext *>(_localctx)->type = AssignmentContext::V;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(52);
      dynamic_cast<AssignmentContext *>(_localctx)->assignee = access();
      setState(53);
      match(PoolParser::CEQ);
      setState(54);
      dynamic_cast<AssignmentContext *>(_localctx)->value = call(0);
      dynamic_cast<AssignmentContext *>(_localctx)->type = AssignmentContext::C;
      break;
    }

    default:
      break;
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

tree::TerminalNode* PoolParser::AccessContext::DOT() {
  return getToken(PoolParser::DOT, 0);
}

tree::TerminalNode* PoolParser::AccessContext::ID() {
  return getToken(PoolParser::ID, 0);
}

PoolParser::CallContext* PoolParser::AccessContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}

tree::TerminalNode* PoolParser::AccessContext::SQ() {
  return getToken(PoolParser::SQ, 0);
}


size_t PoolParser::AccessContext::getRuleIndex() const {
  return PoolParser::RuleAccess;
}


PoolParser::AccessContext* PoolParser::access() {
  AccessContext *_localctx = _tracker.createInstance<AccessContext>(_ctx, getState());
  enterRule(_localctx, 8, PoolParser::RuleAccess);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(71);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(59);
      dynamic_cast<AccessContext *>(_localctx)->callee = call(0);
      setState(60);
      match(PoolParser::DOT);
      setState(61);
      match(PoolParser::ID);
      dynamic_cast<AccessContext *>(_localctx)->type = AccessContext::G;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(64);
      dynamic_cast<AccessContext *>(_localctx)->callee = call(0);
      setState(65);
      match(PoolParser::SQ);
      setState(66);
      match(PoolParser::ID);
      dynamic_cast<AccessContext *>(_localctx)->type = AccessContext::L;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(69);
      match(PoolParser::ID);
      dynamic_cast<AccessContext *>(_localctx)->type = AccessContext::I;
      break;
    }

    default:
      break;
    }
   
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

tree::TerminalNode* PoolParser::CallContext::DOT() {
  return getToken(PoolParser::DOT, 0);
}

tree::TerminalNode* PoolParser::CallContext::ID() {
  return getToken(PoolParser::ID, 0);
}

PoolParser::CallContext* PoolParser::CallContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}

PoolParser::ArgsContext* PoolParser::CallContext::args() {
  return getRuleContext<PoolParser::ArgsContext>(0);
}


size_t PoolParser::CallContext::getRuleIndex() const {
  return PoolParser::RuleCall;
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
  size_t startState = 10;
  enterRecursionRule(_localctx, 10, PoolParser::RuleCall, precedence);

    

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(74);
    term();
    dynamic_cast<CallContext *>(_localctx)->type = CallContext::T;
    _ctx->stop = _input->LT(-1);
    setState(90);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(88);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(77);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(78);
          match(PoolParser::DOT);
          setState(79);
          match(PoolParser::ID);
          setState(81);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
          case 1: {
            setState(80);
            dynamic_cast<CallContext *>(_localctx)->a = args();
            break;
          }

          default:
            break;
          }
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::IA;
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(84);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(85);
          dynamic_cast<CallContext *>(_localctx)->a = args();
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::A;
          break;
        }

        default:
          break;
        } 
      }
      setState(92);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
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

std::vector<PoolParser::ArgContext *> PoolParser::ArgsContext::arg() {
  return getRuleContexts<PoolParser::ArgContext>();
}

PoolParser::ArgContext* PoolParser::ArgsContext::arg(size_t i) {
  return getRuleContext<PoolParser::ArgContext>(i);
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


PoolParser::ArgsContext* PoolParser::args() {
  ArgsContext *_localctx = _tracker.createInstance<ArgsContext>(_ctx, getState());
  enterRule(_localctx, 12, PoolParser::RuleArgs);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    match(PoolParser::LP);
    setState(95);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::DOTS)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(94);
      arg();
    }
    setState(101);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(97);
      match(PoolParser::COMMA);
      setState(98);
      arg();
      setState(103);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(104);
    match(PoolParser::RP);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgContext ------------------------------------------------------------------

PoolParser::ArgContext::ArgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

PoolParser::CallContext* PoolParser::ArgContext::call() {
  return getRuleContext<PoolParser::CallContext>(0);
}

tree::TerminalNode* PoolParser::ArgContext::DOTS() {
  return getToken(PoolParser::DOTS, 0);
}


size_t PoolParser::ArgContext::getRuleIndex() const {
  return PoolParser::RuleArg;
}


PoolParser::ArgContext* PoolParser::arg() {
  ArgContext *_localctx = _tracker.createInstance<ArgContext>(_ctx, getState());
  enterRule(_localctx, 14, PoolParser::RuleArg);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(107);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::DOTS) {
      setState(106);
      match(PoolParser::DOTS);
    }
    setState(109);
    call(0);
   
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

PoolParser::NumContext* PoolParser::TermContext::num() {
  return getRuleContext<PoolParser::NumContext>(0);
}

PoolParser::StringContext* PoolParser::TermContext::string() {
  return getRuleContext<PoolParser::StringContext>(0);
}

PoolParser::FunContext* PoolParser::TermContext::fun() {
  return getRuleContext<PoolParser::FunContext>(0);
}

PoolParser::ParContext* PoolParser::TermContext::par() {
  return getRuleContext<PoolParser::ParContext>(0);
}

PoolParser::BlockContext* PoolParser::TermContext::block() {
  return getRuleContext<PoolParser::BlockContext>(0);
}

tree::TerminalNode* PoolParser::TermContext::NATIVE_SYMBOL() {
  return getToken(PoolParser::NATIVE_SYMBOL, 0);
}

tree::TerminalNode* PoolParser::TermContext::ID() {
  return getToken(PoolParser::ID, 0);
}


size_t PoolParser::TermContext::getRuleIndex() const {
  return PoolParser::RuleTerm;
}


PoolParser::TermContext* PoolParser::term() {
  TermContext *_localctx = _tracker.createInstance<TermContext>(_ctx, getState());
  enterRule(_localctx, 16, PoolParser::RuleTerm);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(130);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(111);
      num();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NUM;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(114);
      string();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::STR;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(117);
      fun();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::FUN;
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(120);
      par();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::PAR;
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(123);
      block();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::BLK;
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(126);
      match(PoolParser::NATIVE_SYMBOL);
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NSM;
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(128);
      match(PoolParser::ID);
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::IDT;
      break;
    }

    default:
      break;
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

PoolParser::ExpressionContext* PoolParser::ParContext::expression() {
  return getRuleContext<PoolParser::ExpressionContext>(0);
}

tree::TerminalNode* PoolParser::ParContext::RP() {
  return getToken(PoolParser::RP, 0);
}


size_t PoolParser::ParContext::getRuleIndex() const {
  return PoolParser::RulePar;
}


PoolParser::ParContext* PoolParser::par() {
  ParContext *_localctx = _tracker.createInstance<ParContext>(_ctx, getState());
  enterRule(_localctx, 18, PoolParser::RulePar);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(132);
    match(PoolParser::LP);
    setState(133);
    expression();
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


PoolParser::BlockContext* PoolParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 20, PoolParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(PoolParser::LB);
    setState(140);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(137);
      statement();
      setState(142);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(143);
    match(PoolParser::RB);
   
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

tree::TerminalNode* PoolParser::FunContext::LP() {
  return getToken(PoolParser::LP, 0);
}

tree::TerminalNode* PoolParser::FunContext::RP() {
  return getToken(PoolParser::RP, 0);
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

std::vector<PoolParser::ParamContext *> PoolParser::FunContext::param() {
  return getRuleContexts<PoolParser::ParamContext>();
}

PoolParser::ParamContext* PoolParser::FunContext::param(size_t i) {
  return getRuleContext<PoolParser::ParamContext>(i);
}

std::vector<tree::TerminalNode *> PoolParser::FunContext::COMMA() {
  return getTokens(PoolParser::COMMA);
}

tree::TerminalNode* PoolParser::FunContext::COMMA(size_t i) {
  return getToken(PoolParser::COMMA, i);
}

std::vector<PoolParser::StatementContext *> PoolParser::FunContext::statement() {
  return getRuleContexts<PoolParser::StatementContext>();
}

PoolParser::StatementContext* PoolParser::FunContext::statement(size_t i) {
  return getRuleContext<PoolParser::StatementContext>(i);
}


size_t PoolParser::FunContext::getRuleIndex() const {
  return PoolParser::RuleFun;
}


PoolParser::FunContext* PoolParser::fun() {
  FunContext *_localctx = _tracker.createInstance<FunContext>(_ctx, getState());
  enterRule(_localctx, 22, PoolParser::RuleFun);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(145);
    match(PoolParser::LP);
    setState(147);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::DOTS

    || _la == PoolParser::ID) {
      setState(146);
      param();
    }
    setState(153);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(149);
      match(PoolParser::COMMA);
      setState(150);
      param();
      setState(155);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(156);
    match(PoolParser::RP);
    setState(157);
    match(PoolParser::COLON);
    setState(158);
    match(PoolParser::LB);
    setState(162);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(159);
      statement();
      setState(164);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(165);
    match(PoolParser::RB);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParamContext ------------------------------------------------------------------

PoolParser::ParamContext::ParamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* PoolParser::ParamContext::ID() {
  return getToken(PoolParser::ID, 0);
}

tree::TerminalNode* PoolParser::ParamContext::DOTS() {
  return getToken(PoolParser::DOTS, 0);
}


size_t PoolParser::ParamContext::getRuleIndex() const {
  return PoolParser::RuleParam;
}


PoolParser::ParamContext* PoolParser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 24, PoolParser::RuleParam);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(168);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::DOTS) {
      setState(167);
      match(PoolParser::DOTS);
    }
    setState(170);
    match(PoolParser::ID);
   
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


PoolParser::NumContext* PoolParser::num() {
  NumContext *_localctx = _tracker.createInstance<NumContext>(_ctx, getState());
  enterRule(_localctx, 26, PoolParser::RuleNum);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(180);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::DECIMAL_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(172);
        match(PoolParser::DECIMAL_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::DEC;
        break;
      }

      case PoolParser::HEX_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(174);
        match(PoolParser::HEX_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::HEX;
        break;
      }

      case PoolParser::BIN_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(176);
        match(PoolParser::BIN_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::BIN;
        break;
      }

      case PoolParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(178);
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


PoolParser::StringContext* PoolParser::string() {
  StringContext *_localctx = _tracker.createInstance<StringContext>(_ctx, getState());
  enterRule(_localctx, 28, PoolParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(182);
    match(PoolParser::STRING_LITERAL);
   
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
    case 5: return callSempred(dynamic_cast<CallContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool PoolParser::callSempred(CallContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);

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
  "program", "statement", "expression", "assignment", "access", "call", 
  "args", "arg", "term", "par", "block", "fun", "param", "num", "string"
};

std::vector<std::string> PoolParser::_literalNames = {
  "", "", "", "", "';'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
  "','", "':='", "'::='", "'::'", "':'", "'#'", "'@'", "'\u003F'", "'...'"
};

std::vector<std::string> PoolParser::_symbolicNames = {
  "", "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", "LSB", 
  "RSB", "DOT", "COMMA", "EQ", "CEQ", "SQ", "COLON", "HASH", "AT", "QM", 
  "DOTS", "DECIMAL_INTEGER_LITERAL", "HEX_INTEGER_LITERAL", "BIN_INTEGER_LITERAL", 
  "FLOAT_LITERAL", "STRING_LITERAL", "NATIVE_SYMBOL", "ID"
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

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x1d, 0xbb, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x3, 0x2, 
       0x7, 0x2, 0x22, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 0x25, 0xb, 0x2, 0x3, 
       0x2, 0x3, 0x2, 0x3, 0x3, 0x5, 0x3, 0x2a, 0xa, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x30, 0xa, 0x4, 0x3, 0x5, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
       0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x3c, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x4a, 0xa, 0x6, 0x3, 
       0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x5, 0x7, 0x54, 0xa, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x5b, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 
       0x5e, 0xb, 0x7, 0x3, 0x8, 0x3, 0x8, 0x5, 0x8, 0x62, 0xa, 0x8, 0x3, 
       0x8, 0x3, 0x8, 0x7, 0x8, 0x66, 0xa, 0x8, 0xc, 0x8, 0xe, 0x8, 0x69, 
       0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x5, 0x9, 0x6e, 0xa, 0x9, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
       0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x5, 0xa, 0x85, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 
       0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x8d, 0xa, 0xc, 0xc, 
       0xc, 0xe, 0xc, 0x90, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 
       0xd, 0x5, 0xd, 0x96, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0x9a, 
       0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0x9d, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 
       0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xa3, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 
       0xa6, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x5, 0xe, 0xab, 0xa, 
       0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
       0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0xb7, 0xa, 0xf, 
       0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x2, 0x3, 0xc, 0x11, 0x2, 0x4, 0x6, 
       0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 
       0x2, 0x2, 0x2, 0xc5, 0x2, 0x23, 0x3, 0x2, 0x2, 0x2, 0x4, 0x29, 0x3, 
       0x2, 0x2, 0x2, 0x6, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x8, 0x3b, 0x3, 0x2, 
       0x2, 0x2, 0xa, 0x49, 0x3, 0x2, 0x2, 0x2, 0xc, 0x4b, 0x3, 0x2, 0x2, 
       0x2, 0xe, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x10, 0x6d, 0x3, 0x2, 0x2, 0x2, 
       0x12, 0x84, 0x3, 0x2, 0x2, 0x2, 0x14, 0x86, 0x3, 0x2, 0x2, 0x2, 0x16, 
       0x8a, 0x3, 0x2, 0x2, 0x2, 0x18, 0x93, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xaa, 
       0x3, 0x2, 0x2, 0x2, 0x1c, 0xb6, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xb8, 0x3, 
       0x2, 0x2, 0x2, 0x20, 0x22, 0x5, 0x4, 0x3, 0x2, 0x21, 0x20, 0x3, 0x2, 
       0x2, 0x2, 0x22, 0x25, 0x3, 0x2, 0x2, 0x2, 0x23, 0x21, 0x3, 0x2, 0x2, 
       0x2, 0x23, 0x24, 0x3, 0x2, 0x2, 0x2, 0x24, 0x26, 0x3, 0x2, 0x2, 0x2, 
       0x25, 0x23, 0x3, 0x2, 0x2, 0x2, 0x26, 0x27, 0x7, 0x2, 0x2, 0x3, 0x27, 
       0x3, 0x3, 0x2, 0x2, 0x2, 0x28, 0x2a, 0x5, 0x6, 0x4, 0x2, 0x29, 0x28, 
       0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x3, 
       0x2, 0x2, 0x2, 0x2b, 0x2c, 0x7, 0x6, 0x2, 0x2, 0x2c, 0x5, 0x3, 0x2, 
       0x2, 0x2, 0x2d, 0x30, 0x5, 0x8, 0x5, 0x2, 0x2e, 0x30, 0x5, 0xc, 0x7, 
       0x2, 0x2f, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x2e, 0x3, 0x2, 0x2, 0x2, 
       0x30, 0x7, 0x3, 0x2, 0x2, 0x2, 0x31, 0x32, 0x5, 0xa, 0x6, 0x2, 0x32, 
       0x33, 0x7, 0xf, 0x2, 0x2, 0x33, 0x34, 0x5, 0xc, 0x7, 0x2, 0x34, 0x35, 
       0x8, 0x5, 0x1, 0x2, 0x35, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x36, 0x37, 0x5, 
       0xa, 0x6, 0x2, 0x37, 0x38, 0x7, 0x10, 0x2, 0x2, 0x38, 0x39, 0x5, 
       0xc, 0x7, 0x2, 0x39, 0x3a, 0x8, 0x5, 0x1, 0x2, 0x3a, 0x3c, 0x3, 0x2, 
       0x2, 0x2, 0x3b, 0x31, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x36, 0x3, 0x2, 0x2, 
       0x2, 0x3c, 0x9, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x5, 0xc, 0x7, 0x2, 
       0x3e, 0x3f, 0x7, 0xd, 0x2, 0x2, 0x3f, 0x40, 0x7, 0x1d, 0x2, 0x2, 
       0x40, 0x41, 0x8, 0x6, 0x1, 0x2, 0x41, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x42, 
       0x43, 0x5, 0xc, 0x7, 0x2, 0x43, 0x44, 0x7, 0x11, 0x2, 0x2, 0x44, 
       0x45, 0x7, 0x1d, 0x2, 0x2, 0x45, 0x46, 0x8, 0x6, 0x1, 0x2, 0x46, 
       0x4a, 0x3, 0x2, 0x2, 0x2, 0x47, 0x48, 0x7, 0x1d, 0x2, 0x2, 0x48, 
       0x4a, 0x8, 0x6, 0x1, 0x2, 0x49, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x49, 0x42, 
       0x3, 0x2, 0x2, 0x2, 0x49, 0x47, 0x3, 0x2, 0x2, 0x2, 0x4a, 0xb, 0x3, 
       0x2, 0x2, 0x2, 0x4b, 0x4c, 0x8, 0x7, 0x1, 0x2, 0x4c, 0x4d, 0x5, 0x12, 
       0xa, 0x2, 0x4d, 0x4e, 0x8, 0x7, 0x1, 0x2, 0x4e, 0x5c, 0x3, 0x2, 0x2, 
       0x2, 0x4f, 0x50, 0xc, 0x5, 0x2, 0x2, 0x50, 0x51, 0x7, 0xd, 0x2, 0x2, 
       0x51, 0x53, 0x7, 0x1d, 0x2, 0x2, 0x52, 0x54, 0x5, 0xe, 0x8, 0x2, 
       0x53, 0x52, 0x3, 0x2, 0x2, 0x2, 0x53, 0x54, 0x3, 0x2, 0x2, 0x2, 0x54, 
       0x55, 0x3, 0x2, 0x2, 0x2, 0x55, 0x5b, 0x8, 0x7, 0x1, 0x2, 0x56, 0x57, 
       0xc, 0x4, 0x2, 0x2, 0x57, 0x58, 0x5, 0xe, 0x8, 0x2, 0x58, 0x59, 0x8, 
       0x7, 0x1, 0x2, 0x59, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x4f, 0x3, 0x2, 
       0x2, 0x2, 0x5a, 0x56, 0x3, 0x2, 0x2, 0x2, 0x5b, 0x5e, 0x3, 0x2, 0x2, 
       0x2, 0x5c, 0x5a, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 0x3, 0x2, 0x2, 0x2, 
       0x5d, 0xd, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x5f, 
       0x61, 0x7, 0x7, 0x2, 0x2, 0x60, 0x62, 0x5, 0x10, 0x9, 0x2, 0x61, 
       0x60, 0x3, 0x2, 0x2, 0x2, 0x61, 0x62, 0x3, 0x2, 0x2, 0x2, 0x62, 0x67, 
       0x3, 0x2, 0x2, 0x2, 0x63, 0x64, 0x7, 0xe, 0x2, 0x2, 0x64, 0x66, 0x5, 
       0x10, 0x9, 0x2, 0x65, 0x63, 0x3, 0x2, 0x2, 0x2, 0x66, 0x69, 0x3, 
       0x2, 0x2, 0x2, 0x67, 0x65, 0x3, 0x2, 0x2, 0x2, 0x67, 0x68, 0x3, 0x2, 
       0x2, 0x2, 0x68, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x69, 0x67, 0x3, 0x2, 0x2, 
       0x2, 0x6a, 0x6b, 0x7, 0x8, 0x2, 0x2, 0x6b, 0xf, 0x3, 0x2, 0x2, 0x2, 
       0x6c, 0x6e, 0x7, 0x16, 0x2, 0x2, 0x6d, 0x6c, 0x3, 0x2, 0x2, 0x2, 
       0x6d, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x6f, 
       0x70, 0x5, 0xc, 0x7, 0x2, 0x70, 0x11, 0x3, 0x2, 0x2, 0x2, 0x71, 0x72, 
       0x5, 0x1c, 0xf, 0x2, 0x72, 0x73, 0x8, 0xa, 0x1, 0x2, 0x73, 0x85, 
       0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x5, 0x1e, 0x10, 0x2, 0x75, 0x76, 
       0x8, 0xa, 0x1, 0x2, 0x76, 0x85, 0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 0x5, 
       0x18, 0xd, 0x2, 0x78, 0x79, 0x8, 0xa, 0x1, 0x2, 0x79, 0x85, 0x3, 
       0x2, 0x2, 0x2, 0x7a, 0x7b, 0x5, 0x14, 0xb, 0x2, 0x7b, 0x7c, 0x8, 
       0xa, 0x1, 0x2, 0x7c, 0x85, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x5, 0x16, 
       0xc, 0x2, 0x7e, 0x7f, 0x8, 0xa, 0x1, 0x2, 0x7f, 0x85, 0x3, 0x2, 0x2, 
       0x2, 0x80, 0x81, 0x7, 0x1c, 0x2, 0x2, 0x81, 0x85, 0x8, 0xa, 0x1, 
       0x2, 0x82, 0x83, 0x7, 0x1d, 0x2, 0x2, 0x83, 0x85, 0x8, 0xa, 0x1, 
       0x2, 0x84, 0x71, 0x3, 0x2, 0x2, 0x2, 0x84, 0x74, 0x3, 0x2, 0x2, 0x2, 
       0x84, 0x77, 0x3, 0x2, 0x2, 0x2, 0x84, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x84, 
       0x7d, 0x3, 0x2, 0x2, 0x2, 0x84, 0x80, 0x3, 0x2, 0x2, 0x2, 0x84, 0x82, 
       0x3, 0x2, 0x2, 0x2, 0x85, 0x13, 0x3, 0x2, 0x2, 0x2, 0x86, 0x87, 0x7, 
       0x7, 0x2, 0x2, 0x87, 0x88, 0x5, 0x6, 0x4, 0x2, 0x88, 0x89, 0x7, 0x8, 
       0x2, 0x2, 0x89, 0x15, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8e, 0x7, 0x9, 0x2, 
       0x2, 0x8b, 0x8d, 0x5, 0x4, 0x3, 0x2, 0x8c, 0x8b, 0x3, 0x2, 0x2, 0x2, 
       0x8d, 0x90, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8e, 
       0x8f, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x91, 0x3, 0x2, 0x2, 0x2, 0x90, 0x8e, 
       0x3, 0x2, 0x2, 0x2, 0x91, 0x92, 0x7, 0xa, 0x2, 0x2, 0x92, 0x17, 0x3, 
       0x2, 0x2, 0x2, 0x93, 0x95, 0x7, 0x7, 0x2, 0x2, 0x94, 0x96, 0x5, 0x1a, 
       0xe, 0x2, 0x95, 0x94, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x3, 0x2, 0x2, 
       0x2, 0x96, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 0x7, 0xe, 0x2, 0x2, 
       0x98, 0x9a, 0x5, 0x1a, 0xe, 0x2, 0x99, 0x97, 0x3, 0x2, 0x2, 0x2, 
       0x9a, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x99, 0x3, 0x2, 0x2, 0x2, 0x9b, 
       0x9c, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x9e, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9b, 
       0x3, 0x2, 0x2, 0x2, 0x9e, 0x9f, 0x7, 0x8, 0x2, 0x2, 0x9f, 0xa0, 0x7, 
       0x12, 0x2, 0x2, 0xa0, 0xa4, 0x7, 0x9, 0x2, 0x2, 0xa1, 0xa3, 0x5, 
       0x4, 0x3, 0x2, 0xa2, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa6, 0x3, 0x2, 
       0x2, 0x2, 0xa4, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa5, 0x3, 0x2, 0x2, 
       0x2, 0xa5, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa4, 0x3, 0x2, 0x2, 0x2, 
       0xa7, 0xa8, 0x7, 0xa, 0x2, 0x2, 0xa8, 0x19, 0x3, 0x2, 0x2, 0x2, 0xa9, 
       0xab, 0x7, 0x16, 0x2, 0x2, 0xaa, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xaa, 
       0xab, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 
       0x7, 0x1d, 0x2, 0x2, 0xad, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xae, 0xaf, 
       0x7, 0x17, 0x2, 0x2, 0xaf, 0xb7, 0x8, 0xf, 0x1, 0x2, 0xb0, 0xb1, 
       0x7, 0x18, 0x2, 0x2, 0xb1, 0xb7, 0x8, 0xf, 0x1, 0x2, 0xb2, 0xb3, 
       0x7, 0x19, 0x2, 0x2, 0xb3, 0xb7, 0x8, 0xf, 0x1, 0x2, 0xb4, 0xb5, 
       0x7, 0x1a, 0x2, 0x2, 0xb5, 0xb7, 0x8, 0xf, 0x1, 0x2, 0xb6, 0xae, 
       0x3, 0x2, 0x2, 0x2, 0xb6, 0xb0, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb2, 0x3, 
       0x2, 0x2, 0x2, 0xb6, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb7, 0x1d, 0x3, 0x2, 
       0x2, 0x2, 0xb8, 0xb9, 0x7, 0x1b, 0x2, 0x2, 0xb9, 0x1f, 0x3, 0x2, 
       0x2, 0x2, 0x14, 0x23, 0x29, 0x2f, 0x3b, 0x49, 0x53, 0x5a, 0x5c, 0x61, 
       0x67, 0x6d, 0x84, 0x8e, 0x95, 0x9b, 0xa4, 0xaa, 0xb6, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

PoolParser::Initializer PoolParser::_init;
