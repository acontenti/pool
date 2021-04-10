
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
    setState(35);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(32);
      statement();
      setState(37);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(38);
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
    setState(41);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(40);
      expression();
    }
    setState(43);
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
    setState(47);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(45);
      dynamic_cast<ExpressionContext *>(_localctx)->a = assignment();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(46);
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
    setState(59);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(49);
      dynamic_cast<AssignmentContext *>(_localctx)->assignee = access();
      setState(50);
      match(PoolParser::EQ);
      setState(51);
      dynamic_cast<AssignmentContext *>(_localctx)->value = call(0);
      dynamic_cast<AssignmentContext *>(_localctx)->type = AssignmentContext::V;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(54);
      dynamic_cast<AssignmentContext *>(_localctx)->assignee = access();
      setState(55);
      match(PoolParser::CEQ);
      setState(56);
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

tree::TerminalNode* PoolParser::AccessContext::DOTDOT() {
  return getToken(PoolParser::DOTDOT, 0);
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
    setState(73);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(61);
      dynamic_cast<AccessContext *>(_localctx)->callee = call(0);
      setState(62);
      match(PoolParser::DOT);
      setState(63);
      match(PoolParser::ID);
      dynamic_cast<AccessContext *>(_localctx)->type = AccessContext::G;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(66);
      dynamic_cast<AccessContext *>(_localctx)->callee = call(0);
      setState(67);
      match(PoolParser::DOTDOT);
      setState(68);
      match(PoolParser::ID);
      dynamic_cast<AccessContext *>(_localctx)->type = AccessContext::L;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(71);
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

tree::TerminalNode* PoolParser::CallContext::DOTDOT() {
  return getToken(PoolParser::DOTDOT, 0);
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
    setState(76);
    term();
    dynamic_cast<CallContext *>(_localctx)->type = CallContext::T;
    _ctx->stop = _input->LT(-1);
    setState(99);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(97);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(79);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(80);
          match(PoolParser::DOT);
          setState(81);
          match(PoolParser::ID);
          setState(83);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
          case 1: {
            setState(82);
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
          setState(86);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(87);
          match(PoolParser::DOTDOT);
          setState(88);
          match(PoolParser::ID);
          setState(90);
          _errHandler->sync(this);

          switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
          case 1: {
            setState(89);
            dynamic_cast<CallContext *>(_localctx)->a = args();
            break;
          }

          default:
            break;
          }
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::ILA;
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(93);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(94);
          dynamic_cast<CallContext *>(_localctx)->a = args();
          dynamic_cast<CallContext *>(_localctx)->type = CallContext::A;
          break;
        }

        default:
          break;
        } 
      }
      setState(101);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
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
    setState(102);
    match(PoolParser::LP);
    setState(104);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::DOTS)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(103);
      arg();
    }
    setState(110);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(106);
      match(PoolParser::COMMA);
      setState(107);
      arg();
      setState(112);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(113);
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
    setState(116);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::DOTS) {
      setState(115);
      match(PoolParser::DOTS);
    }
    setState(118);
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

PoolParser::ArrayContext* PoolParser::TermContext::array() {
  return getRuleContext<PoolParser::ArrayContext>(0);
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
    setState(142);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(120);
      num();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NUM;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(123);
      string();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::STR;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(126);
      fun();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::FUN;
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(129);
      par();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::PAR;
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(132);
      block();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::BLK;
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(135);
      array();
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::ARR;
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(138);
      match(PoolParser::NATIVE_SYMBOL);
      dynamic_cast<TermContext *>(_localctx)->type = TermContext::NSM;
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(140);
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
    setState(144);
    match(PoolParser::LP);
    setState(145);
    expression();
    setState(146);
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
    setState(148);
    match(PoolParser::LB);
    setState(152);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(149);
      statement();
      setState(154);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(155);
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

std::vector<PoolParser::ArgContext *> PoolParser::ArrayContext::arg() {
  return getRuleContexts<PoolParser::ArgContext>();
}

PoolParser::ArgContext* PoolParser::ArrayContext::arg(size_t i) {
  return getRuleContext<PoolParser::ArgContext>(i);
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


PoolParser::ArrayContext* PoolParser::array() {
  ArrayContext *_localctx = _tracker.createInstance<ArrayContext>(_ctx, getState());
  enterRule(_localctx, 22, PoolParser::RuleArray);
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
    setState(157);
    match(PoolParser::LSB);
    setState(159);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::DOTS)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(158);
      arg();
    }
    setState(165);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(161);
      match(PoolParser::COMMA);
      setState(162);
      arg();
      setState(167);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(168);
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
  enterRule(_localctx, 24, PoolParser::RuleFun);
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
    setState(170);
    match(PoolParser::LP);
    setState(172);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::DOTS

    || _la == PoolParser::ID) {
      setState(171);
      param();
    }
    setState(178);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == PoolParser::COMMA) {
      setState(174);
      match(PoolParser::COMMA);
      setState(175);
      param();
      setState(180);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(181);
    match(PoolParser::RP);
    setState(182);
    match(PoolParser::COLON);
    setState(183);
    match(PoolParser::LB);
    setState(187);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << PoolParser::SEMI)
      | (1ULL << PoolParser::LP)
      | (1ULL << PoolParser::LB)
      | (1ULL << PoolParser::LSB)
      | (1ULL << PoolParser::DECIMAL_INTEGER_LITERAL)
      | (1ULL << PoolParser::HEX_INTEGER_LITERAL)
      | (1ULL << PoolParser::BIN_INTEGER_LITERAL)
      | (1ULL << PoolParser::FLOAT_LITERAL)
      | (1ULL << PoolParser::STRING_LITERAL)
      | (1ULL << PoolParser::NATIVE_SYMBOL)
      | (1ULL << PoolParser::ID))) != 0)) {
      setState(184);
      statement();
      setState(189);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(190);
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

std::vector<tree::TerminalNode *> PoolParser::ParamContext::ID() {
  return getTokens(PoolParser::ID);
}

tree::TerminalNode* PoolParser::ParamContext::ID(size_t i) {
  return getToken(PoolParser::ID, i);
}

tree::TerminalNode* PoolParser::ParamContext::DOTS() {
  return getToken(PoolParser::DOTS, 0);
}

tree::TerminalNode* PoolParser::ParamContext::COLON() {
  return getToken(PoolParser::COLON, 0);
}


size_t PoolParser::ParamContext::getRuleIndex() const {
  return PoolParser::RuleParam;
}


PoolParser::ParamContext* PoolParser::param() {
  ParamContext *_localctx = _tracker.createInstance<ParamContext>(_ctx, getState());
  enterRule(_localctx, 26, PoolParser::RuleParam);
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
    setState(193);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::DOTS) {
      setState(192);
      match(PoolParser::DOTS);
    }
    setState(195);
    dynamic_cast<ParamContext *>(_localctx)->name = match(PoolParser::ID);
    setState(198);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::COLON) {
      setState(196);
      match(PoolParser::COLON);
      setState(197);
      dynamic_cast<ParamContext *>(_localctx)->type = match(PoolParser::ID);
    }
   
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
  enterRule(_localctx, 28, PoolParser::RuleNum);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(208);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case PoolParser::DECIMAL_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 1);
        setState(200);
        match(PoolParser::DECIMAL_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::DEC;
        break;
      }

      case PoolParser::HEX_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(202);
        match(PoolParser::HEX_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::HEX;
        break;
      }

      case PoolParser::BIN_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(204);
        match(PoolParser::BIN_INTEGER_LITERAL);
        dynamic_cast<NumContext *>(_localctx)->type = NumContext::BIN;
        break;
      }

      case PoolParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(206);
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
  enterRule(_localctx, 30, PoolParser::RuleString);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(210);
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
    case 0: return precpred(_ctx, 4);
    case 1: return precpred(_ctx, 3);
    case 2: return precpred(_ctx, 2);

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
  "args", "arg", "term", "par", "block", "array", "fun", "param", "num", 
  "string"
};

std::vector<std::string> PoolParser::_literalNames = {
  "", "", "", "", "';'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
  "','", "':='", "'::='", "'..'", "':'", "'#'", "'@'", "'\u003F'", "'...'"
};

std::vector<std::string> PoolParser::_symbolicNames = {
  "", "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", "LSB", 
  "RSB", "DOT", "COMMA", "EQ", "CEQ", "DOTDOT", "COLON", "HASH", "AT", "QM", 
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
       0x3, 0x1d, 0xd7, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x3, 0x2, 0x7, 0x2, 0x24, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 
       0x27, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x5, 0x3, 0x2c, 0xa, 
       0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x32, 0xa, 
       0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
       0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x3e, 0xa, 0x5, 
       0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 
       0x4c, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x56, 0xa, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x5d, 0xa, 0x7, 
       0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x64, 
       0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 0x67, 0xb, 0x7, 0x3, 0x8, 0x3, 0x8, 
       0x5, 0x8, 0x6b, 0xa, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x6f, 0xa, 
       0x8, 0xc, 0x8, 0xe, 0x8, 0x72, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 
       0x9, 0x5, 0x9, 0x77, 0xa, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
       0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
       0x3, 0xa, 0x5, 0xa, 0x91, 0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 
       0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x7, 0xc, 0x99, 0xa, 0xc, 0xc, 0xc, 
       0xe, 0xc, 0x9c, 0xb, 0xc, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 
       0x5, 0xd, 0xa2, 0xa, 0xd, 0x3, 0xd, 0x3, 0xd, 0x7, 0xd, 0xa6, 0xa, 
       0xd, 0xc, 0xd, 0xe, 0xd, 0xa9, 0xb, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 
       0xe, 0x3, 0xe, 0x5, 0xe, 0xaf, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 
       0xe, 0xb3, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0xb6, 0xb, 0xe, 0x3, 0xe, 
       0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0xbc, 0xa, 0xe, 0xc, 0xe, 
       0xe, 0xe, 0xbf, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x5, 0xf, 
       0xc4, 0xa, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0xc9, 0xa, 
       0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
       0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xd3, 0xa, 0x10, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x2, 0x3, 0xc, 0x12, 0x2, 0x4, 0x6, 0x8, 0xa, 
       0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x2, 
       0x2, 0x2, 0xe6, 0x2, 0x25, 0x3, 0x2, 0x2, 0x2, 0x4, 0x2b, 0x3, 0x2, 
       0x2, 0x2, 0x6, 0x31, 0x3, 0x2, 0x2, 0x2, 0x8, 0x3d, 0x3, 0x2, 0x2, 
       0x2, 0xa, 0x4b, 0x3, 0x2, 0x2, 0x2, 0xc, 0x4d, 0x3, 0x2, 0x2, 0x2, 
       0xe, 0x68, 0x3, 0x2, 0x2, 0x2, 0x10, 0x76, 0x3, 0x2, 0x2, 0x2, 0x12, 
       0x90, 0x3, 0x2, 0x2, 0x2, 0x14, 0x92, 0x3, 0x2, 0x2, 0x2, 0x16, 0x96, 
       0x3, 0x2, 0x2, 0x2, 0x18, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xac, 0x3, 
       0x2, 0x2, 0x2, 0x1c, 0xc3, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xd2, 0x3, 0x2, 
       0x2, 0x2, 0x20, 0xd4, 0x3, 0x2, 0x2, 0x2, 0x22, 0x24, 0x5, 0x4, 0x3, 
       0x2, 0x23, 0x22, 0x3, 0x2, 0x2, 0x2, 0x24, 0x27, 0x3, 0x2, 0x2, 0x2, 
       0x25, 0x23, 0x3, 0x2, 0x2, 0x2, 0x25, 0x26, 0x3, 0x2, 0x2, 0x2, 0x26, 
       0x28, 0x3, 0x2, 0x2, 0x2, 0x27, 0x25, 0x3, 0x2, 0x2, 0x2, 0x28, 0x29, 
       0x7, 0x2, 0x2, 0x3, 0x29, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2c, 0x5, 
       0x6, 0x4, 0x2, 0x2b, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2b, 0x2c, 0x3, 0x2, 
       0x2, 0x2, 0x2c, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x2d, 0x2e, 0x7, 0x6, 0x2, 
       0x2, 0x2e, 0x5, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x32, 0x5, 0x8, 0x5, 0x2, 
       0x30, 0x32, 0x5, 0xc, 0x7, 0x2, 0x31, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x31, 
       0x30, 0x3, 0x2, 0x2, 0x2, 0x32, 0x7, 0x3, 0x2, 0x2, 0x2, 0x33, 0x34, 
       0x5, 0xa, 0x6, 0x2, 0x34, 0x35, 0x7, 0xf, 0x2, 0x2, 0x35, 0x36, 0x5, 
       0xc, 0x7, 0x2, 0x36, 0x37, 0x8, 0x5, 0x1, 0x2, 0x37, 0x3e, 0x3, 0x2, 
       0x2, 0x2, 0x38, 0x39, 0x5, 0xa, 0x6, 0x2, 0x39, 0x3a, 0x7, 0x10, 
       0x2, 0x2, 0x3a, 0x3b, 0x5, 0xc, 0x7, 0x2, 0x3b, 0x3c, 0x8, 0x5, 0x1, 
       0x2, 0x3c, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x33, 0x3, 0x2, 0x2, 0x2, 
       0x3d, 0x38, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x9, 0x3, 0x2, 0x2, 0x2, 0x3f, 
       0x40, 0x5, 0xc, 0x7, 0x2, 0x40, 0x41, 0x7, 0xd, 0x2, 0x2, 0x41, 0x42, 
       0x7, 0x1d, 0x2, 0x2, 0x42, 0x43, 0x8, 0x6, 0x1, 0x2, 0x43, 0x4c, 
       0x3, 0x2, 0x2, 0x2, 0x44, 0x45, 0x5, 0xc, 0x7, 0x2, 0x45, 0x46, 0x7, 
       0x11, 0x2, 0x2, 0x46, 0x47, 0x7, 0x1d, 0x2, 0x2, 0x47, 0x48, 0x8, 
       0x6, 0x1, 0x2, 0x48, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4a, 0x7, 0x1d, 
       0x2, 0x2, 0x4a, 0x4c, 0x8, 0x6, 0x1, 0x2, 0x4b, 0x3f, 0x3, 0x2, 0x2, 
       0x2, 0x4b, 0x44, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x49, 0x3, 0x2, 0x2, 0x2, 
       0x4c, 0xb, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x8, 0x7, 0x1, 0x2, 0x4e, 
       0x4f, 0x5, 0x12, 0xa, 0x2, 0x4f, 0x50, 0x8, 0x7, 0x1, 0x2, 0x50, 
       0x65, 0x3, 0x2, 0x2, 0x2, 0x51, 0x52, 0xc, 0x6, 0x2, 0x2, 0x52, 0x53, 
       0x7, 0xd, 0x2, 0x2, 0x53, 0x55, 0x7, 0x1d, 0x2, 0x2, 0x54, 0x56, 
       0x5, 0xe, 0x8, 0x2, 0x55, 0x54, 0x3, 0x2, 0x2, 0x2, 0x55, 0x56, 0x3, 
       0x2, 0x2, 0x2, 0x56, 0x57, 0x3, 0x2, 0x2, 0x2, 0x57, 0x64, 0x8, 0x7, 
       0x1, 0x2, 0x58, 0x59, 0xc, 0x5, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x11, 
       0x2, 0x2, 0x5a, 0x5c, 0x7, 0x1d, 0x2, 0x2, 0x5b, 0x5d, 0x5, 0xe, 
       0x8, 0x2, 0x5c, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 0x3, 0x2, 0x2, 
       0x2, 0x5d, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x64, 0x8, 0x7, 0x1, 0x2, 
       0x5f, 0x60, 0xc, 0x4, 0x2, 0x2, 0x60, 0x61, 0x5, 0xe, 0x8, 0x2, 0x61, 
       0x62, 0x8, 0x7, 0x1, 0x2, 0x62, 0x64, 0x3, 0x2, 0x2, 0x2, 0x63, 0x51, 
       0x3, 0x2, 0x2, 0x2, 0x63, 0x58, 0x3, 0x2, 0x2, 0x2, 0x63, 0x5f, 0x3, 
       0x2, 0x2, 0x2, 0x64, 0x67, 0x3, 0x2, 0x2, 0x2, 0x65, 0x63, 0x3, 0x2, 
       0x2, 0x2, 0x65, 0x66, 0x3, 0x2, 0x2, 0x2, 0x66, 0xd, 0x3, 0x2, 0x2, 
       0x2, 0x67, 0x65, 0x3, 0x2, 0x2, 0x2, 0x68, 0x6a, 0x7, 0x7, 0x2, 0x2, 
       0x69, 0x6b, 0x5, 0x10, 0x9, 0x2, 0x6a, 0x69, 0x3, 0x2, 0x2, 0x2, 
       0x6a, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x70, 0x3, 0x2, 0x2, 0x2, 0x6c, 
       0x6d, 0x7, 0xe, 0x2, 0x2, 0x6d, 0x6f, 0x5, 0x10, 0x9, 0x2, 0x6e, 
       0x6c, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x72, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6e, 
       0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 0x3, 0x2, 0x2, 0x2, 0x71, 0x73, 0x3, 
       0x2, 0x2, 0x2, 0x72, 0x70, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x7, 0x8, 
       0x2, 0x2, 0x74, 0xf, 0x3, 0x2, 0x2, 0x2, 0x75, 0x77, 0x7, 0x16, 0x2, 
       0x2, 0x76, 0x75, 0x3, 0x2, 0x2, 0x2, 0x76, 0x77, 0x3, 0x2, 0x2, 0x2, 
       0x77, 0x78, 0x3, 0x2, 0x2, 0x2, 0x78, 0x79, 0x5, 0xc, 0x7, 0x2, 0x79, 
       0x11, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7b, 0x5, 0x1e, 0x10, 0x2, 0x7b, 
       0x7c, 0x8, 0xa, 0x1, 0x2, 0x7c, 0x91, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 
       0x5, 0x20, 0x11, 0x2, 0x7e, 0x7f, 0x8, 0xa, 0x1, 0x2, 0x7f, 0x91, 
       0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 0x5, 0x1a, 0xe, 0x2, 0x81, 0x82, 
       0x8, 0xa, 0x1, 0x2, 0x82, 0x91, 0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 0x5, 
       0x14, 0xb, 0x2, 0x84, 0x85, 0x8, 0xa, 0x1, 0x2, 0x85, 0x91, 0x3, 
       0x2, 0x2, 0x2, 0x86, 0x87, 0x5, 0x16, 0xc, 0x2, 0x87, 0x88, 0x8, 
       0xa, 0x1, 0x2, 0x88, 0x91, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8a, 0x5, 0x18, 
       0xd, 0x2, 0x8a, 0x8b, 0x8, 0xa, 0x1, 0x2, 0x8b, 0x91, 0x3, 0x2, 0x2, 
       0x2, 0x8c, 0x8d, 0x7, 0x1c, 0x2, 0x2, 0x8d, 0x91, 0x8, 0xa, 0x1, 
       0x2, 0x8e, 0x8f, 0x7, 0x1d, 0x2, 0x2, 0x8f, 0x91, 0x8, 0xa, 0x1, 
       0x2, 0x90, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x90, 0x7d, 0x3, 0x2, 0x2, 0x2, 
       0x90, 0x80, 0x3, 0x2, 0x2, 0x2, 0x90, 0x83, 0x3, 0x2, 0x2, 0x2, 0x90, 
       0x86, 0x3, 0x2, 0x2, 0x2, 0x90, 0x89, 0x3, 0x2, 0x2, 0x2, 0x90, 0x8c, 
       0x3, 0x2, 0x2, 0x2, 0x90, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x91, 0x13, 0x3, 
       0x2, 0x2, 0x2, 0x92, 0x93, 0x7, 0x7, 0x2, 0x2, 0x93, 0x94, 0x5, 0x6, 
       0x4, 0x2, 0x94, 0x95, 0x7, 0x8, 0x2, 0x2, 0x95, 0x15, 0x3, 0x2, 0x2, 
       0x2, 0x96, 0x9a, 0x7, 0x9, 0x2, 0x2, 0x97, 0x99, 0x5, 0x4, 0x3, 0x2, 
       0x98, 0x97, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9c, 0x3, 0x2, 0x2, 0x2, 0x9a, 
       0x98, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x9d, 
       0x3, 0x2, 0x2, 0x2, 0x9c, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x9d, 0x9e, 0x7, 
       0xa, 0x2, 0x2, 0x9e, 0x17, 0x3, 0x2, 0x2, 0x2, 0x9f, 0xa1, 0x7, 0xb, 
       0x2, 0x2, 0xa0, 0xa2, 0x5, 0x10, 0x9, 0x2, 0xa1, 0xa0, 0x3, 0x2, 
       0x2, 0x2, 0xa1, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa2, 0xa7, 0x3, 0x2, 0x2, 
       0x2, 0xa3, 0xa4, 0x7, 0xe, 0x2, 0x2, 0xa4, 0xa6, 0x5, 0x10, 0x9, 
       0x2, 0xa5, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa9, 0x3, 0x2, 0x2, 0x2, 
       0xa7, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa8, 
       0xaa, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xab, 
       0x7, 0xc, 0x2, 0x2, 0xab, 0x19, 0x3, 0x2, 0x2, 0x2, 0xac, 0xae, 0x7, 
       0x7, 0x2, 0x2, 0xad, 0xaf, 0x5, 0x1c, 0xf, 0x2, 0xae, 0xad, 0x3, 
       0x2, 0x2, 0x2, 0xae, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xaf, 0xb4, 0x3, 0x2, 
       0x2, 0x2, 0xb0, 0xb1, 0x7, 0xe, 0x2, 0x2, 0xb1, 0xb3, 0x5, 0x1c, 
       0xf, 0x2, 0xb2, 0xb0, 0x3, 0x2, 0x2, 0x2, 0xb3, 0xb6, 0x3, 0x2, 0x2, 
       0x2, 0xb4, 0xb2, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb5, 0x3, 0x2, 0x2, 0x2, 
       0xb5, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb6, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb7, 
       0xb8, 0x7, 0x8, 0x2, 0x2, 0xb8, 0xb9, 0x7, 0x12, 0x2, 0x2, 0xb9, 
       0xbd, 0x7, 0x9, 0x2, 0x2, 0xba, 0xbc, 0x5, 0x4, 0x3, 0x2, 0xbb, 0xba, 
       0x3, 0x2, 0x2, 0x2, 0xbc, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xbd, 0xbb, 0x3, 
       0x2, 0x2, 0x2, 0xbd, 0xbe, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xc0, 0x3, 0x2, 
       0x2, 0x2, 0xbf, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x7, 0xa, 0x2, 
       0x2, 0xc1, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc4, 0x7, 0x16, 0x2, 
       0x2, 0xc3, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc3, 0xc4, 0x3, 0x2, 0x2, 0x2, 
       0xc4, 0xc5, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc8, 0x7, 0x1d, 0x2, 0x2, 
       0xc6, 0xc7, 0x7, 0x12, 0x2, 0x2, 0xc7, 0xc9, 0x7, 0x1d, 0x2, 0x2, 
       0xc8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x3, 0x2, 0x2, 0x2, 0xc9, 
       0x1d, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcb, 0x7, 0x17, 0x2, 0x2, 0xcb, 
       0xd3, 0x8, 0x10, 0x1, 0x2, 0xcc, 0xcd, 0x7, 0x18, 0x2, 0x2, 0xcd, 
       0xd3, 0x8, 0x10, 0x1, 0x2, 0xce, 0xcf, 0x7, 0x19, 0x2, 0x2, 0xcf, 
       0xd3, 0x8, 0x10, 0x1, 0x2, 0xd0, 0xd1, 0x7, 0x1a, 0x2, 0x2, 0xd1, 
       0xd3, 0x8, 0x10, 0x1, 0x2, 0xd2, 0xca, 0x3, 0x2, 0x2, 0x2, 0xd2, 
       0xcc, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xce, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd0, 
       0x3, 0x2, 0x2, 0x2, 0xd3, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd5, 0x7, 
       0x1b, 0x2, 0x2, 0xd5, 0x21, 0x3, 0x2, 0x2, 0x2, 0x18, 0x25, 0x2b, 
       0x31, 0x3d, 0x4b, 0x55, 0x5c, 0x63, 0x65, 0x6a, 0x70, 0x76, 0x90, 
       0x9a, 0xa1, 0xa7, 0xae, 0xb4, 0xbd, 0xc3, 0xc8, 0xd2, 
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
