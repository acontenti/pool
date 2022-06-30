
// Generated from PoolLexer.g4 by ANTLR 4.10.1


#include "PoolLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct PoolLexerStaticData final {
  PoolLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  PoolLexerStaticData(const PoolLexerStaticData&) = delete;
  PoolLexerStaticData(PoolLexerStaticData&&) = delete;
  PoolLexerStaticData& operator=(const PoolLexerStaticData&) = delete;
  PoolLexerStaticData& operator=(PoolLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag poollexerLexerOnceFlag;
PoolLexerStaticData *poollexerLexerStaticData = nullptr;

void poollexerLexerInitialize() {
  assert(poollexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<PoolLexerStaticData>(
    std::vector<std::string>{
      "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", "LSB", 
      "RSB", "DOT", "COMMA", "EQ", "CEQ", "DOTDOT", "COLON", "HASH", "AT", 
      "DOTS", "HEX_DIGIT", "BIN_DIGIT", "DIGIT", "DECIMAL_INTEGER_LITERAL", 
      "HEX_INTEGER_LITERAL", "BIN_INTEGER_LITERAL", "FLOAT_LITERAL", "ESCAPE", 
      "STRING_LITERAL", "NATIVE_SYMBOL", "ID", "SYMBOL"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "", "", "", "';'", "'('", "')'", "'{'", "'}'", "'['", "']'", "'.'", 
      "','", "':='", "'::='", "'..'", "':'", "'#'", "'@'", "'...'"
    },
    std::vector<std::string>{
      "", "WS", "COMMENT", "LINE_COMMENT", "SEMI", "LP", "RP", "LB", "RB", 
      "LSB", "RSB", "DOT", "COMMA", "EQ", "CEQ", "DOTDOT", "COLON", "HASH", 
      "AT", "DOTS", "DECIMAL_INTEGER_LITERAL", "HEX_INTEGER_LITERAL", "BIN_INTEGER_LITERAL", 
      "FLOAT_LITERAL", "STRING_LITERAL", "NATIVE_SYMBOL", "ID"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,26,233,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,1,0,4,0,65,8,0,11,0,12,0,66,1,0,1,0,1,1,1,1,
  	1,1,1,1,5,1,75,8,1,10,1,12,1,78,9,1,1,1,1,1,1,1,3,1,83,8,1,1,1,1,1,1,
  	2,1,2,1,2,1,2,5,2,91,8,2,10,2,12,2,94,9,2,1,2,1,2,1,3,1,3,1,4,1,4,1,5,
  	1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,12,1,12,1,12,
  	1,13,1,13,1,13,1,13,1,14,1,14,1,14,1,15,1,15,1,16,1,16,1,17,1,17,1,18,
  	1,18,1,18,1,18,1,19,1,19,1,20,1,20,1,21,1,21,1,22,3,22,143,8,22,1,22,
  	4,22,146,8,22,11,22,12,22,147,1,23,3,23,151,8,23,1,23,1,23,1,23,5,23,
  	156,8,23,10,23,12,23,159,9,23,1,24,3,24,162,8,24,1,24,1,24,1,24,5,24,
  	167,8,24,10,24,12,24,170,9,24,1,25,3,25,173,8,25,1,25,5,25,176,8,25,10,
  	25,12,25,179,9,25,1,25,3,25,182,8,25,1,25,4,25,185,8,25,11,25,12,25,186,
  	1,26,1,26,1,26,1,26,1,26,1,26,1,26,1,26,3,26,197,8,26,1,27,1,27,1,27,
  	5,27,202,8,27,10,27,12,27,205,9,27,1,27,1,27,1,28,1,28,1,28,1,28,1,28,
  	5,28,214,8,28,10,28,12,28,217,9,28,1,28,1,28,1,29,1,29,3,29,223,8,29,
  	1,29,1,29,5,29,227,8,29,10,29,12,29,230,9,29,1,30,1,30,1,76,0,31,1,1,
  	3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,
  	15,31,16,33,17,35,18,37,19,39,0,41,0,43,0,45,20,47,21,49,22,51,23,53,
  	0,55,24,57,25,59,26,61,0,1,0,13,3,0,9,10,13,13,32,32,2,0,10,10,13,13,
  	3,0,48,57,65,70,97,102,1,0,48,49,1,0,48,57,2,0,43,43,45,45,2,0,88,88,
  	120,120,2,0,66,66,98,98,8,0,34,34,47,47,92,92,98,98,102,102,110,110,114,
  	114,116,116,4,0,10,10,13,13,34,34,92,92,2,0,65,90,97,122,3,0,48,57,65,
  	90,97,122,9,0,33,33,37,38,42,43,45,45,47,47,60,63,94,95,124,124,126,126,
  	248,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,
  	1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,
  	0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,
  	0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,
  	1,0,0,0,0,51,1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,1,64,1,0,
  	0,0,3,70,1,0,0,0,5,86,1,0,0,0,7,97,1,0,0,0,9,99,1,0,0,0,11,101,1,0,0,
  	0,13,103,1,0,0,0,15,105,1,0,0,0,17,107,1,0,0,0,19,109,1,0,0,0,21,111,
  	1,0,0,0,23,113,1,0,0,0,25,115,1,0,0,0,27,118,1,0,0,0,29,122,1,0,0,0,31,
  	125,1,0,0,0,33,127,1,0,0,0,35,129,1,0,0,0,37,131,1,0,0,0,39,135,1,0,0,
  	0,41,137,1,0,0,0,43,139,1,0,0,0,45,142,1,0,0,0,47,150,1,0,0,0,49,161,
  	1,0,0,0,51,172,1,0,0,0,53,188,1,0,0,0,55,198,1,0,0,0,57,208,1,0,0,0,59,
  	222,1,0,0,0,61,231,1,0,0,0,63,65,7,0,0,0,64,63,1,0,0,0,65,66,1,0,0,0,
  	66,64,1,0,0,0,66,67,1,0,0,0,67,68,1,0,0,0,68,69,6,0,0,0,69,2,1,0,0,0,
  	70,71,5,47,0,0,71,72,5,42,0,0,72,76,1,0,0,0,73,75,9,0,0,0,74,73,1,0,0,
  	0,75,78,1,0,0,0,76,77,1,0,0,0,76,74,1,0,0,0,77,82,1,0,0,0,78,76,1,0,0,
  	0,79,80,5,42,0,0,80,83,5,47,0,0,81,83,5,0,0,1,82,79,1,0,0,0,82,81,1,0,
  	0,0,83,84,1,0,0,0,84,85,6,1,0,0,85,4,1,0,0,0,86,87,5,47,0,0,87,88,5,47,
  	0,0,88,92,1,0,0,0,89,91,8,1,0,0,90,89,1,0,0,0,91,94,1,0,0,0,92,90,1,0,
  	0,0,92,93,1,0,0,0,93,95,1,0,0,0,94,92,1,0,0,0,95,96,6,2,0,0,96,6,1,0,
  	0,0,97,98,5,59,0,0,98,8,1,0,0,0,99,100,5,40,0,0,100,10,1,0,0,0,101,102,
  	5,41,0,0,102,12,1,0,0,0,103,104,5,123,0,0,104,14,1,0,0,0,105,106,5,125,
  	0,0,106,16,1,0,0,0,107,108,5,91,0,0,108,18,1,0,0,0,109,110,5,93,0,0,110,
  	20,1,0,0,0,111,112,5,46,0,0,112,22,1,0,0,0,113,114,5,44,0,0,114,24,1,
  	0,0,0,115,116,5,58,0,0,116,117,5,61,0,0,117,26,1,0,0,0,118,119,5,58,0,
  	0,119,120,5,58,0,0,120,121,5,61,0,0,121,28,1,0,0,0,122,123,5,46,0,0,123,
  	124,5,46,0,0,124,30,1,0,0,0,125,126,5,58,0,0,126,32,1,0,0,0,127,128,5,
  	35,0,0,128,34,1,0,0,0,129,130,5,64,0,0,130,36,1,0,0,0,131,132,5,46,0,
  	0,132,133,5,46,0,0,133,134,5,46,0,0,134,38,1,0,0,0,135,136,7,2,0,0,136,
  	40,1,0,0,0,137,138,7,3,0,0,138,42,1,0,0,0,139,140,7,4,0,0,140,44,1,0,
  	0,0,141,143,7,5,0,0,142,141,1,0,0,0,142,143,1,0,0,0,143,145,1,0,0,0,144,
  	146,3,43,21,0,145,144,1,0,0,0,146,147,1,0,0,0,147,145,1,0,0,0,147,148,
  	1,0,0,0,148,46,1,0,0,0,149,151,7,5,0,0,150,149,1,0,0,0,150,151,1,0,0,
  	0,151,152,1,0,0,0,152,153,5,48,0,0,153,157,7,6,0,0,154,156,3,39,19,0,
  	155,154,1,0,0,0,156,159,1,0,0,0,157,155,1,0,0,0,157,158,1,0,0,0,158,48,
  	1,0,0,0,159,157,1,0,0,0,160,162,7,5,0,0,161,160,1,0,0,0,161,162,1,0,0,
  	0,162,163,1,0,0,0,163,164,5,48,0,0,164,168,7,7,0,0,165,167,3,41,20,0,
  	166,165,1,0,0,0,167,170,1,0,0,0,168,166,1,0,0,0,168,169,1,0,0,0,169,50,
  	1,0,0,0,170,168,1,0,0,0,171,173,7,5,0,0,172,171,1,0,0,0,172,173,1,0,0,
  	0,173,181,1,0,0,0,174,176,3,43,21,0,175,174,1,0,0,0,176,179,1,0,0,0,177,
  	175,1,0,0,0,177,178,1,0,0,0,178,180,1,0,0,0,179,177,1,0,0,0,180,182,5,
  	46,0,0,181,177,1,0,0,0,181,182,1,0,0,0,182,184,1,0,0,0,183,185,3,43,21,
  	0,184,183,1,0,0,0,185,186,1,0,0,0,186,184,1,0,0,0,186,187,1,0,0,0,187,
  	52,1,0,0,0,188,196,5,92,0,0,189,197,7,8,0,0,190,191,5,117,0,0,191,192,
  	3,39,19,0,192,193,3,39,19,0,193,194,3,39,19,0,194,195,3,39,19,0,195,197,
  	1,0,0,0,196,189,1,0,0,0,196,190,1,0,0,0,197,54,1,0,0,0,198,203,5,34,0,
  	0,199,202,8,9,0,0,200,202,3,53,26,0,201,199,1,0,0,0,201,200,1,0,0,0,202,
  	205,1,0,0,0,203,201,1,0,0,0,203,204,1,0,0,0,204,206,1,0,0,0,205,203,1,
  	0,0,0,206,207,5,34,0,0,207,56,1,0,0,0,208,209,5,96,0,0,209,215,3,59,29,
  	0,210,211,3,21,10,0,211,212,3,59,29,0,212,214,1,0,0,0,213,210,1,0,0,0,
  	214,217,1,0,0,0,215,213,1,0,0,0,215,216,1,0,0,0,216,218,1,0,0,0,217,215,
  	1,0,0,0,218,219,5,96,0,0,219,58,1,0,0,0,220,223,7,10,0,0,221,223,3,61,
  	30,0,222,220,1,0,0,0,222,221,1,0,0,0,223,228,1,0,0,0,224,227,7,11,0,0,
  	225,227,3,61,30,0,226,224,1,0,0,0,226,225,1,0,0,0,227,230,1,0,0,0,228,
  	226,1,0,0,0,228,229,1,0,0,0,229,60,1,0,0,0,230,228,1,0,0,0,231,232,7,
  	12,0,0,232,62,1,0,0,0,22,0,66,76,82,92,142,147,150,157,161,168,172,177,
  	181,186,196,201,203,215,222,226,228,1,0,1,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  poollexerLexerStaticData = staticData.release();
}

}

PoolLexer::PoolLexer(CharStream *input) : Lexer(input) {
  PoolLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *poollexerLexerStaticData->atn, poollexerLexerStaticData->decisionToDFA, poollexerLexerStaticData->sharedContextCache);
}

PoolLexer::~PoolLexer() {
  delete _interpreter;
}

std::string PoolLexer::getGrammarFileName() const {
  return "PoolLexer.g4";
}

const std::vector<std::string>& PoolLexer::getRuleNames() const {
  return poollexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& PoolLexer::getChannelNames() const {
  return poollexerLexerStaticData->channelNames;
}

const std::vector<std::string>& PoolLexer::getModeNames() const {
  return poollexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& PoolLexer::getVocabulary() const {
  return poollexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView PoolLexer::getSerializedATN() const {
  return poollexerLexerStaticData->serializedATN;
}

const atn::ATN& PoolLexer::getATN() const {
  return *poollexerLexerStaticData->atn;
}




void PoolLexer::initialize() {
  std::call_once(poollexerLexerOnceFlag, poollexerLexerInitialize);
}
