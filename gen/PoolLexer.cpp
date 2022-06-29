
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
      "QM", "DOTS", "HEX_DIGIT", "BIN_DIGIT", "DIGIT", "DECIMAL_INTEGER_LITERAL", 
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
  	4,0,27,225,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,1,0,4,0,67,8,0,11,0,12,0,68,1,0,1,
  	0,1,1,1,1,1,1,1,1,5,1,77,8,1,10,1,12,1,80,9,1,1,1,1,1,1,1,3,1,85,8,1,
  	1,1,1,1,1,2,1,2,1,2,1,2,5,2,93,8,2,10,2,12,2,96,9,2,1,2,1,2,1,3,1,3,1,
  	4,1,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,11,1,11,1,12,
  	1,12,1,12,1,13,1,13,1,13,1,13,1,14,1,14,1,14,1,15,1,15,1,16,1,16,1,17,
  	1,17,1,18,1,18,1,19,1,19,1,19,1,19,1,20,1,20,1,21,1,21,1,22,1,22,1,23,
  	4,23,147,8,23,11,23,12,23,148,1,24,1,24,1,24,5,24,154,8,24,10,24,12,24,
  	157,9,24,1,25,1,25,1,25,5,25,162,8,25,10,25,12,25,165,9,25,1,26,5,26,
  	168,8,26,10,26,12,26,171,9,26,1,26,3,26,174,8,26,1,26,4,26,177,8,26,11,
  	26,12,26,178,1,27,1,27,1,27,1,27,1,27,1,27,1,27,1,27,3,27,189,8,27,1,
  	28,1,28,1,28,5,28,194,8,28,10,28,12,28,197,9,28,1,28,1,28,1,29,1,29,1,
  	29,1,29,1,29,5,29,206,8,29,10,29,12,29,209,9,29,1,29,1,29,1,30,1,30,3,
  	30,215,8,30,1,30,1,30,5,30,219,8,30,10,30,12,30,222,9,30,1,31,1,31,1,
  	78,0,32,1,1,3,2,5,3,7,4,9,5,11,6,13,7,15,8,17,9,19,10,21,11,23,12,25,
  	13,27,14,29,15,31,16,33,17,35,18,37,19,39,20,41,0,43,0,45,0,47,21,49,
  	22,51,23,53,24,55,0,57,25,59,26,61,27,63,0,1,0,12,3,0,9,10,13,13,32,32,
  	2,0,10,10,13,13,3,0,48,57,65,70,97,102,1,0,48,49,1,0,48,57,2,0,88,88,
  	120,120,2,0,66,66,98,98,8,0,34,34,47,47,92,92,98,98,102,102,110,110,114,
  	114,116,116,4,0,10,10,13,13,34,34,92,92,2,0,65,90,97,122,3,0,48,57,65,
  	90,97,122,9,0,33,33,37,38,42,43,45,45,47,47,60,62,94,95,124,124,126,126,
  	236,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,1,0,0,0,0,9,1,0,0,0,0,11,
  	1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,
  	0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,
  	0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,0,0,0,0,47,1,0,0,0,0,49,
  	1,0,0,0,0,51,1,0,0,0,0,53,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,61,1,0,
  	0,0,1,66,1,0,0,0,3,72,1,0,0,0,5,88,1,0,0,0,7,99,1,0,0,0,9,101,1,0,0,0,
  	11,103,1,0,0,0,13,105,1,0,0,0,15,107,1,0,0,0,17,109,1,0,0,0,19,111,1,
  	0,0,0,21,113,1,0,0,0,23,115,1,0,0,0,25,117,1,0,0,0,27,120,1,0,0,0,29,
  	124,1,0,0,0,31,127,1,0,0,0,33,129,1,0,0,0,35,131,1,0,0,0,37,133,1,0,0,
  	0,39,135,1,0,0,0,41,139,1,0,0,0,43,141,1,0,0,0,45,143,1,0,0,0,47,146,
  	1,0,0,0,49,150,1,0,0,0,51,158,1,0,0,0,53,173,1,0,0,0,55,180,1,0,0,0,57,
  	190,1,0,0,0,59,200,1,0,0,0,61,214,1,0,0,0,63,223,1,0,0,0,65,67,7,0,0,
  	0,66,65,1,0,0,0,67,68,1,0,0,0,68,66,1,0,0,0,68,69,1,0,0,0,69,70,1,0,0,
  	0,70,71,6,0,0,0,71,2,1,0,0,0,72,73,5,47,0,0,73,74,5,42,0,0,74,78,1,0,
  	0,0,75,77,9,0,0,0,76,75,1,0,0,0,77,80,1,0,0,0,78,79,1,0,0,0,78,76,1,0,
  	0,0,79,84,1,0,0,0,80,78,1,0,0,0,81,82,5,42,0,0,82,85,5,47,0,0,83,85,5,
  	0,0,1,84,81,1,0,0,0,84,83,1,0,0,0,85,86,1,0,0,0,86,87,6,1,0,0,87,4,1,
  	0,0,0,88,89,5,47,0,0,89,90,5,47,0,0,90,94,1,0,0,0,91,93,8,1,0,0,92,91,
  	1,0,0,0,93,96,1,0,0,0,94,92,1,0,0,0,94,95,1,0,0,0,95,97,1,0,0,0,96,94,
  	1,0,0,0,97,98,6,2,0,0,98,6,1,0,0,0,99,100,5,59,0,0,100,8,1,0,0,0,101,
  	102,5,40,0,0,102,10,1,0,0,0,103,104,5,41,0,0,104,12,1,0,0,0,105,106,5,
  	123,0,0,106,14,1,0,0,0,107,108,5,125,0,0,108,16,1,0,0,0,109,110,5,91,
  	0,0,110,18,1,0,0,0,111,112,5,93,0,0,112,20,1,0,0,0,113,114,5,46,0,0,114,
  	22,1,0,0,0,115,116,5,44,0,0,116,24,1,0,0,0,117,118,5,58,0,0,118,119,5,
  	61,0,0,119,26,1,0,0,0,120,121,5,58,0,0,121,122,5,58,0,0,122,123,5,61,
  	0,0,123,28,1,0,0,0,124,125,5,46,0,0,125,126,5,46,0,0,126,30,1,0,0,0,127,
  	128,5,58,0,0,128,32,1,0,0,0,129,130,5,35,0,0,130,34,1,0,0,0,131,132,5,
  	64,0,0,132,36,1,0,0,0,133,134,5,63,0,0,134,38,1,0,0,0,135,136,5,46,0,
  	0,136,137,5,46,0,0,137,138,5,46,0,0,138,40,1,0,0,0,139,140,7,2,0,0,140,
  	42,1,0,0,0,141,142,7,3,0,0,142,44,1,0,0,0,143,144,7,4,0,0,144,46,1,0,
  	0,0,145,147,3,45,22,0,146,145,1,0,0,0,147,148,1,0,0,0,148,146,1,0,0,0,
  	148,149,1,0,0,0,149,48,1,0,0,0,150,151,5,48,0,0,151,155,7,5,0,0,152,154,
  	3,41,20,0,153,152,1,0,0,0,154,157,1,0,0,0,155,153,1,0,0,0,155,156,1,0,
  	0,0,156,50,1,0,0,0,157,155,1,0,0,0,158,159,5,48,0,0,159,163,7,6,0,0,160,
  	162,3,43,21,0,161,160,1,0,0,0,162,165,1,0,0,0,163,161,1,0,0,0,163,164,
  	1,0,0,0,164,52,1,0,0,0,165,163,1,0,0,0,166,168,3,45,22,0,167,166,1,0,
  	0,0,168,171,1,0,0,0,169,167,1,0,0,0,169,170,1,0,0,0,170,172,1,0,0,0,171,
  	169,1,0,0,0,172,174,5,46,0,0,173,169,1,0,0,0,173,174,1,0,0,0,174,176,
  	1,0,0,0,175,177,3,45,22,0,176,175,1,0,0,0,177,178,1,0,0,0,178,176,1,0,
  	0,0,178,179,1,0,0,0,179,54,1,0,0,0,180,188,5,92,0,0,181,189,7,7,0,0,182,
  	183,5,117,0,0,183,184,3,41,20,0,184,185,3,41,20,0,185,186,3,41,20,0,186,
  	187,3,41,20,0,187,189,1,0,0,0,188,181,1,0,0,0,188,182,1,0,0,0,189,56,
  	1,0,0,0,190,195,5,34,0,0,191,194,8,8,0,0,192,194,3,55,27,0,193,191,1,
  	0,0,0,193,192,1,0,0,0,194,197,1,0,0,0,195,193,1,0,0,0,195,196,1,0,0,0,
  	196,198,1,0,0,0,197,195,1,0,0,0,198,199,5,34,0,0,199,58,1,0,0,0,200,201,
  	5,96,0,0,201,207,3,61,30,0,202,203,3,21,10,0,203,204,3,61,30,0,204,206,
  	1,0,0,0,205,202,1,0,0,0,206,209,1,0,0,0,207,205,1,0,0,0,207,208,1,0,0,
  	0,208,210,1,0,0,0,209,207,1,0,0,0,210,211,5,96,0,0,211,60,1,0,0,0,212,
  	215,7,9,0,0,213,215,3,63,31,0,214,212,1,0,0,0,214,213,1,0,0,0,215,220,
  	1,0,0,0,216,219,7,10,0,0,217,219,3,63,31,0,218,216,1,0,0,0,218,217,1,
  	0,0,0,219,222,1,0,0,0,220,218,1,0,0,0,220,221,1,0,0,0,221,62,1,0,0,0,
  	222,220,1,0,0,0,223,224,7,11,0,0,224,64,1,0,0,0,18,0,68,78,84,94,148,
  	155,163,169,173,178,188,193,195,207,214,218,220,1,0,1,0
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
