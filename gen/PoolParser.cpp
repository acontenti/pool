
// Generated from PoolParser.g4 by ANTLR 4.10.1



#include "PoolParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct PoolParserStaticData final {
  PoolParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PoolParserStaticData(const PoolParserStaticData&) = delete;
  PoolParserStaticData(PoolParserStaticData&&) = delete;
  PoolParserStaticData& operator=(const PoolParserStaticData&) = delete;
  PoolParserStaticData& operator=(PoolParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag poolparserParserOnceFlag;
PoolParserStaticData *poolparserParserStaticData = nullptr;

void poolparserParserInitialize() {
  assert(poolparserParserStaticData == nullptr);
  auto staticData = std::make_unique<PoolParserStaticData>(
    std::vector<std::string>{
      "program", "statement", "expression", "assignment", "access", "call", 
      "args", "arg", "term", "par", "block", "array", "fun", "param", "num", 
      "string"
    },
    std::vector<std::string>{
      "", "", "", "", "';'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
      "','", "':='", "'::='", "'..'", "':'", "'#'", "'@'", "'\\u003F'", 
      "'...'"
    },
    std::vector<std::string>{
      "", "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", 
      "LSB", "RSB", "DOT", "COMMA", "EQ", "CEQ", "DOTDOT", "COLON", "HASH", 
      "AT", "QM", "DOTS", "DECIMAL_INTEGER_LITERAL", "HEX_INTEGER_LITERAL", 
      "BIN_INTEGER_LITERAL", "FLOAT_LITERAL", "STRING_LITERAL", "NATIVE_SYMBOL", 
      "ID"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,27,213,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,7,
  	14,2,15,7,15,1,0,5,0,34,8,0,10,0,12,0,37,9,0,1,0,1,0,1,1,3,1,42,8,1,1,
  	1,1,1,1,2,1,2,3,2,48,8,2,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,3,3,
  	60,8,3,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,1,4,3,4,74,8,4,1,5,
  	1,5,1,5,1,5,1,5,1,5,1,5,1,5,3,5,84,8,5,1,5,1,5,1,5,1,5,1,5,3,5,91,8,5,
  	1,5,1,5,1,5,1,5,1,5,5,5,98,8,5,10,5,12,5,101,9,5,1,6,1,6,3,6,105,8,6,
  	1,6,1,6,5,6,109,8,6,10,6,12,6,112,9,6,1,6,1,6,1,7,3,7,117,8,7,1,7,1,7,
  	1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,8,1,
  	8,1,8,1,8,1,8,1,8,3,8,143,8,8,1,9,1,9,1,9,1,9,1,10,1,10,5,10,151,8,10,
  	10,10,12,10,154,9,10,1,10,1,10,1,11,1,11,3,11,160,8,11,1,11,1,11,5,11,
  	164,8,11,10,11,12,11,167,9,11,1,11,1,11,1,12,1,12,3,12,173,8,12,1,12,
  	1,12,5,12,177,8,12,10,12,12,12,180,9,12,1,12,1,12,1,12,1,12,5,12,186,
  	8,12,10,12,12,12,189,9,12,1,12,1,12,1,13,3,13,194,8,13,1,13,1,13,1,13,
  	3,13,199,8,13,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,3,14,209,8,14,1,
  	15,1,15,1,15,0,1,10,16,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,0,0,
  	228,0,35,1,0,0,0,2,41,1,0,0,0,4,47,1,0,0,0,6,59,1,0,0,0,8,73,1,0,0,0,
  	10,75,1,0,0,0,12,102,1,0,0,0,14,116,1,0,0,0,16,142,1,0,0,0,18,144,1,0,
  	0,0,20,148,1,0,0,0,22,157,1,0,0,0,24,170,1,0,0,0,26,193,1,0,0,0,28,208,
  	1,0,0,0,30,210,1,0,0,0,32,34,3,2,1,0,33,32,1,0,0,0,34,37,1,0,0,0,35,33,
  	1,0,0,0,35,36,1,0,0,0,36,38,1,0,0,0,37,35,1,0,0,0,38,39,5,0,0,1,39,1,
  	1,0,0,0,40,42,3,4,2,0,41,40,1,0,0,0,41,42,1,0,0,0,42,43,1,0,0,0,43,44,
  	5,4,0,0,44,3,1,0,0,0,45,48,3,6,3,0,46,48,3,10,5,0,47,45,1,0,0,0,47,46,
  	1,0,0,0,48,5,1,0,0,0,49,50,3,8,4,0,50,51,5,13,0,0,51,52,3,10,5,0,52,53,
  	6,3,-1,0,53,60,1,0,0,0,54,55,3,8,4,0,55,56,5,14,0,0,56,57,3,10,5,0,57,
  	58,6,3,-1,0,58,60,1,0,0,0,59,49,1,0,0,0,59,54,1,0,0,0,60,7,1,0,0,0,61,
  	62,3,10,5,0,62,63,5,11,0,0,63,64,5,27,0,0,64,65,6,4,-1,0,65,74,1,0,0,
  	0,66,67,3,10,5,0,67,68,5,15,0,0,68,69,5,27,0,0,69,70,6,4,-1,0,70,74,1,
  	0,0,0,71,72,5,27,0,0,72,74,6,4,-1,0,73,61,1,0,0,0,73,66,1,0,0,0,73,71,
  	1,0,0,0,74,9,1,0,0,0,75,76,6,5,-1,0,76,77,3,16,8,0,77,78,6,5,-1,0,78,
  	99,1,0,0,0,79,80,10,4,0,0,80,81,5,11,0,0,81,83,5,27,0,0,82,84,3,12,6,
  	0,83,82,1,0,0,0,83,84,1,0,0,0,84,85,1,0,0,0,85,98,6,5,-1,0,86,87,10,3,
  	0,0,87,88,5,15,0,0,88,90,5,27,0,0,89,91,3,12,6,0,90,89,1,0,0,0,90,91,
  	1,0,0,0,91,92,1,0,0,0,92,98,6,5,-1,0,93,94,10,2,0,0,94,95,3,12,6,0,95,
  	96,6,5,-1,0,96,98,1,0,0,0,97,79,1,0,0,0,97,86,1,0,0,0,97,93,1,0,0,0,98,
  	101,1,0,0,0,99,97,1,0,0,0,99,100,1,0,0,0,100,11,1,0,0,0,101,99,1,0,0,
  	0,102,104,5,5,0,0,103,105,3,14,7,0,104,103,1,0,0,0,104,105,1,0,0,0,105,
  	110,1,0,0,0,106,107,5,12,0,0,107,109,3,14,7,0,108,106,1,0,0,0,109,112,
  	1,0,0,0,110,108,1,0,0,0,110,111,1,0,0,0,111,113,1,0,0,0,112,110,1,0,0,
  	0,113,114,5,6,0,0,114,13,1,0,0,0,115,117,5,20,0,0,116,115,1,0,0,0,116,
  	117,1,0,0,0,117,118,1,0,0,0,118,119,3,10,5,0,119,15,1,0,0,0,120,121,3,
  	28,14,0,121,122,6,8,-1,0,122,143,1,0,0,0,123,124,3,30,15,0,124,125,6,
  	8,-1,0,125,143,1,0,0,0,126,127,3,24,12,0,127,128,6,8,-1,0,128,143,1,0,
  	0,0,129,130,3,18,9,0,130,131,6,8,-1,0,131,143,1,0,0,0,132,133,3,20,10,
  	0,133,134,6,8,-1,0,134,143,1,0,0,0,135,136,3,22,11,0,136,137,6,8,-1,0,
  	137,143,1,0,0,0,138,139,5,26,0,0,139,143,6,8,-1,0,140,141,5,27,0,0,141,
  	143,6,8,-1,0,142,120,1,0,0,0,142,123,1,0,0,0,142,126,1,0,0,0,142,129,
  	1,0,0,0,142,132,1,0,0,0,142,135,1,0,0,0,142,138,1,0,0,0,142,140,1,0,0,
  	0,143,17,1,0,0,0,144,145,5,5,0,0,145,146,3,4,2,0,146,147,5,6,0,0,147,
  	19,1,0,0,0,148,152,5,7,0,0,149,151,3,2,1,0,150,149,1,0,0,0,151,154,1,
  	0,0,0,152,150,1,0,0,0,152,153,1,0,0,0,153,155,1,0,0,0,154,152,1,0,0,0,
  	155,156,5,8,0,0,156,21,1,0,0,0,157,159,5,9,0,0,158,160,3,14,7,0,159,158,
  	1,0,0,0,159,160,1,0,0,0,160,165,1,0,0,0,161,162,5,12,0,0,162,164,3,14,
  	7,0,163,161,1,0,0,0,164,167,1,0,0,0,165,163,1,0,0,0,165,166,1,0,0,0,166,
  	168,1,0,0,0,167,165,1,0,0,0,168,169,5,10,0,0,169,23,1,0,0,0,170,172,5,
  	5,0,0,171,173,3,26,13,0,172,171,1,0,0,0,172,173,1,0,0,0,173,178,1,0,0,
  	0,174,175,5,12,0,0,175,177,3,26,13,0,176,174,1,0,0,0,177,180,1,0,0,0,
  	178,176,1,0,0,0,178,179,1,0,0,0,179,181,1,0,0,0,180,178,1,0,0,0,181,182,
  	5,6,0,0,182,183,5,16,0,0,183,187,5,7,0,0,184,186,3,2,1,0,185,184,1,0,
  	0,0,186,189,1,0,0,0,187,185,1,0,0,0,187,188,1,0,0,0,188,190,1,0,0,0,189,
  	187,1,0,0,0,190,191,5,8,0,0,191,25,1,0,0,0,192,194,5,20,0,0,193,192,1,
  	0,0,0,193,194,1,0,0,0,194,195,1,0,0,0,195,198,5,27,0,0,196,197,5,16,0,
  	0,197,199,5,27,0,0,198,196,1,0,0,0,198,199,1,0,0,0,199,27,1,0,0,0,200,
  	201,5,21,0,0,201,209,6,14,-1,0,202,203,5,22,0,0,203,209,6,14,-1,0,204,
  	205,5,23,0,0,205,209,6,14,-1,0,206,207,5,24,0,0,207,209,6,14,-1,0,208,
  	200,1,0,0,0,208,202,1,0,0,0,208,204,1,0,0,0,208,206,1,0,0,0,209,29,1,
  	0,0,0,210,211,5,25,0,0,211,31,1,0,0,0,22,35,41,47,59,73,83,90,97,99,104,
  	110,116,142,152,159,165,172,178,187,193,198,208
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  poolparserParserStaticData = staticData.release();
}

}

PoolParser::PoolParser(TokenStream *input) : PoolParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

PoolParser::PoolParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  PoolParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *poolparserParserStaticData->atn, poolparserParserStaticData->decisionToDFA, poolparserParserStaticData->sharedContextCache, options);
}

PoolParser::~PoolParser() {
  delete _interpreter;
}

const atn::ATN& PoolParser::getATN() const {
  return *poolparserParserStaticData->atn;
}

std::string PoolParser::getGrammarFileName() const {
  return "PoolParser.g4";
}

const std::vector<std::string>& PoolParser::getRuleNames() const {
  return poolparserParserStaticData->ruleNames;
}

const dfa::Vocabulary& PoolParser::getVocabulary() const {
  return poolparserParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView PoolParser::getSerializedATN() const {
  return poolparserParserStaticData->serializedATN;
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
      antlrcpp::downCast<ExpressionContext *>(_localctx)->a = assignment();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(46);
      antlrcpp::downCast<ExpressionContext *>(_localctx)->c = call(0);
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
      antlrcpp::downCast<AssignmentContext *>(_localctx)->assignee = access();
      setState(50);
      match(PoolParser::EQ);
      setState(51);
      antlrcpp::downCast<AssignmentContext *>(_localctx)->value = call(0);
      antlrcpp::downCast<AssignmentContext *>(_localctx)->type = AssignmentContext::V;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(54);
      antlrcpp::downCast<AssignmentContext *>(_localctx)->assignee = access();
      setState(55);
      match(PoolParser::CEQ);
      setState(56);
      antlrcpp::downCast<AssignmentContext *>(_localctx)->value = call(0);
      antlrcpp::downCast<AssignmentContext *>(_localctx)->type = AssignmentContext::C;
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
      antlrcpp::downCast<AccessContext *>(_localctx)->callee = call(0);
      setState(62);
      match(PoolParser::DOT);
      setState(63);
      match(PoolParser::ID);
      antlrcpp::downCast<AccessContext *>(_localctx)->type = AccessContext::G;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(66);
      antlrcpp::downCast<AccessContext *>(_localctx)->callee = call(0);
      setState(67);
      match(PoolParser::DOTDOT);
      setState(68);
      match(PoolParser::ID);
      antlrcpp::downCast<AccessContext *>(_localctx)->type = AccessContext::L;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(71);
      match(PoolParser::ID);
      antlrcpp::downCast<AccessContext *>(_localctx)->type = AccessContext::I;
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
    antlrcpp::downCast<CallContext *>(_localctx)->type = CallContext::T;
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
            antlrcpp::downCast<CallContext *>(_localctx)->a = args();
            break;
          }

          default:
            break;
          }
          antlrcpp::downCast<CallContext *>(_localctx)->type = CallContext::IA;
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
            antlrcpp::downCast<CallContext *>(_localctx)->a = args();
            break;
          }

          default:
            break;
          }
          antlrcpp::downCast<CallContext *>(_localctx)->type = CallContext::ILA;
          break;
        }

        case 3: {
          _localctx = _tracker.createInstance<CallContext>(parentContext, parentState);
          _localctx->callee = previousContext;
          pushNewRecursionContext(_localctx, startState, RuleCall);
          setState(93);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(94);
          antlrcpp::downCast<CallContext *>(_localctx)->a = args();
          antlrcpp::downCast<CallContext *>(_localctx)->type = CallContext::A;
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
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::NUM;
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(123);
      string();
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::STR;
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(126);
      fun();
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::FUN;
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(129);
      par();
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::PAR;
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(132);
      block();
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::BLK;
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(135);
      array();
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::ARR;
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(138);
      match(PoolParser::NATIVE_SYMBOL);
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::NSM;
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(140);
      match(PoolParser::ID);
      antlrcpp::downCast<TermContext *>(_localctx)->type = TermContext::IDT;
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
    antlrcpp::downCast<ParamContext *>(_localctx)->name = match(PoolParser::ID);
    setState(198);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == PoolParser::COLON) {
      setState(196);
      match(PoolParser::COLON);
      setState(197);
      antlrcpp::downCast<ParamContext *>(_localctx)->type = match(PoolParser::ID);
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
        antlrcpp::downCast<NumContext *>(_localctx)->type = NumContext::DEC;
        break;
      }

      case PoolParser::HEX_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 2);
        setState(202);
        match(PoolParser::HEX_INTEGER_LITERAL);
        antlrcpp::downCast<NumContext *>(_localctx)->type = NumContext::HEX;
        break;
      }

      case PoolParser::BIN_INTEGER_LITERAL: {
        enterOuterAlt(_localctx, 3);
        setState(204);
        match(PoolParser::BIN_INTEGER_LITERAL);
        antlrcpp::downCast<NumContext *>(_localctx)->type = NumContext::BIN;
        break;
      }

      case PoolParser::FLOAT_LITERAL: {
        enterOuterAlt(_localctx, 4);
        setState(206);
        match(PoolParser::FLOAT_LITERAL);
        antlrcpp::downCast<NumContext *>(_localctx)->type = NumContext::FLT;
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
    case 5: return callSempred(antlrcpp::downCast<CallContext *>(context), predicateIndex);

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

void PoolParser::initialize() {
  std::call_once(poolparserParserOnceFlag, poolparserParserInitialize);
}
