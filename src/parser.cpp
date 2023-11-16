#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

#include "parser.hpp"
#include "pool_private.hpp"
#include <poolstd.hpp>
#include "natives.hpp"
#include "util/strings.hpp"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/LLVMContext.h>

using namespace antlr4;
namespace pool {
	class ParserImpl : public Parser {
		size_t intFunIdx = 0;
		llvm::IRBuilder<> builder;

		Location tptl(Token *start, Token *end = nullptr) {
			if (!start) return Location::UNKNOWN;
			if (!end) end = start;
			return Location(Location::Point{
					start->getLine(),
					start->getCharPositionInLine(),
					start->getStartIndex(),
					start->getStopIndex()
			}, Location::Point{
					end->getLine(),
					end->getCharPositionInLine(),
					end->getStartIndex(),
					end->getStopIndex()
			}, poolInstance);
		}

		llvm::Value *parseNum(PoolParser::NumContext *ast, llvm::Value *contextObject) {
			llvm::Value *value;
			string constructorName;
			switch (ast->type) {
				case PoolParser::NumContext::DEC: {
					const auto &token = ast->DECIMAL_INTEGER_LITERAL()->getSymbol();
					value = builder.getInt(llvm::APInt(64, token->getText(), 10));
					constructorName = "$Integer_$new";
					break;
				}
				case PoolParser::NumContext::HEX: {
					const auto &token = ast->HEX_INTEGER_LITERAL()->getSymbol();
					value = builder.getInt(llvm::APInt(64, token->getText().substr(2), 16));
					constructorName = "$Integer_$new";
					break;
				}
				case PoolParser::NumContext::BIN: {
					const auto &token = ast->BIN_INTEGER_LITERAL()->getSymbol();
					value = builder.getInt(llvm::APInt(64, token->getText().substr(2), 2));
					constructorName = "$Integer_$new";
					break;
				}
				case PoolParser::NumContext::FLT: {
					const auto &token = ast->FLOAT_LITERAL()->getSymbol();
					value = llvm::ConstantFP::get(*llvm_context, llvm::APFloat(stod(token->getText())));
					constructorName = "$Decimal_$new";
					break;
				}
				default:
					throw compile_error("invalid number literal", tptl(ast->start, ast->stop));
			}
			auto *classNewFunction = Natives::get()->findFunction(constructorName);
			auto *classNew = builder.CreateCall(classNewFunction, {value, contextObject});
			return classNew;
		}

		llvm::Value *parseString(PoolParser::StringContext *ast, llvm::Value *contextObject) {
			const auto &token = ast->STRING_LITERAL()->getSymbol();
			const auto &text = token->getText();
			const auto &unescaped = unescapeString(text.substr(1, text.size() - 2));
			auto *value = builder.CreateGlobalStringPtr(llvm::StringRef(unescaped));
			auto *classNewFunction = Natives::get()->findFunction("$String_$new");
			auto *classNew = builder.CreateCall(classNewFunction, {value, contextObject});
			return classNew;
		}

		llvm::Value *parseNative(tree::TerminalNode *ast) {
			const auto &token = ast->getSymbol();
			const auto &id = token->getText();
			auto *native = Natives::get()->find(id);
			if (native) {
				return native;
			} else throw compile_error("Symbol '" + id + "' is not defined", tptl(token));
		}

		llvm::Value *parseIdentifier(antlr4::Token *token, llvm::Value *contextObject, bool local = false) {
			const auto &id = token->getText();
			auto *objectGetFunction = Natives::get()->findFunction(local ? "$Object_getLocal" : "$Object_get");
			auto *objectGet = builder.CreateCall(objectGetFunction, {contextObject, builder.CreateGlobalStringPtr(id)});
			return objectGet;
		}

		llvm::Value *parseAccess(PoolParser::AccessContext *ast, llvm::Value *contextObject) {
			switch (ast->type) {
				case PoolParser::AccessContext::G:
				case PoolParser::AccessContext::L: {
					const auto &caller = parseCall(ast->callee, contextObject);
					auto *identifier = parseIdentifier(ast->ID()->getSymbol(), caller, ast->type == PoolParser::AccessContext::L);
					return identifier;
				}
				case PoolParser::AccessContext::I: {
					auto *identifier = parseIdentifier(ast->ID()->getSymbol(), contextObject);
					return identifier;
				}
				default:
					throw compile_error("invalid access", tptl(ast->start, ast->stop));
			}
		}

		llvm::Value *parseAssignment(PoolParser::AssignmentContext *ast, llvm::Value *contextObject) {
			auto assignee = parseAccess(ast->assignee, contextObject);
			auto value = parseCall(ast->value, contextObject);
			auto *setFunction = Natives::get()->findFunction("$Object_set");
			auto *constant = ast->type == PoolParser::AssignmentContext::C ? builder.getTrue() : builder.getFalse();
			auto *set = builder.CreateCall(setFunction, {assignee, builder.CreateGlobalStringPtr(ast->assignee->ID()->getText()), value, constant});
			return set;
		}

		llvm::Value *parseBlock(PoolParser::BlockContext *ast, llvm::Value *contextObject) {
			auto *function = parseStatements(ast->statement());
			auto *classNewFunction = Natives::get()->findFunction("$Function_$new");
			auto *nullptrValue = llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(*llvm_context));
			auto *classNew = builder.CreateCall(classNewFunction, {nullptrValue, builder.getInt64(0), function, contextObject});
			return classNew;
		}

		llvm::Value *parseFunction(PoolParser::FunContext *ast, llvm::Value *contextObject) {
			auto paramsAst = ast->param();
			auto *mallocFunction = Natives::get()->findFunction("malloc");
			auto *parameterType = llvm::StructType::create(*llvm_context, {
					llvm::PointerType::getInt8PtrTy(*llvm_context),
					llvm::PointerType::getUnqual(*llvm_context),
					llvm::Type::getInt1Ty(*llvm_context)
			}, "Parameter");
			auto *paramsArrayType = llvm::ArrayType::get(parameterType, paramsAst.size());
			auto *paramsMemory = builder.CreateCall(mallocFunction, {llvm::ConstantExpr::getSizeOf(paramsArrayType)});
			auto *paramsArray = builder.CreateBitCast(paramsMemory, parameterType->getPointerTo());
			size_t i = 0;
			for (const auto &param: paramsAst) {
				llvm::Constant *type;
				if (param->type) {
					auto *id = parseIdentifier(param->type, contextObject);
					type = reinterpret_cast<llvm::Constant *>(id);
				} else {
					type = reinterpret_cast<llvm::Constant *>(Natives::get()->find("$ObjectClass"));
				}
				auto *parameterValue = llvm::ConstantStruct::get(parameterType, {
						builder.CreateGlobalStringPtr(param->name->getText()),
						type,
						builder.getInt1(param->DOTS() != nullptr)
				});
				auto *ptr = builder.CreateGEP(parameterType, paramsArray, {builder.getInt64(i++)});
				builder.CreateStore(parameterValue, ptr);
			}
			auto *function = parseStatements(ast->statement());
			auto *classNewFunction = Natives::get()->findFunction("$Function_$new");
			auto *classNew = builder.CreateCall(classNewFunction, {paramsArray, builder.getInt64(paramsAst.size()), function, contextObject});
			return classNew;
		}

		pair<llvm::Value *, llvm::Value*> parseArgs(const vector<PoolParser::ArgContext *> &ast, llvm::Value *contextObject) {
			if (!ast.empty()) {
				auto *mallocFunction = Natives::get()->findFunction("malloc");
				auto *arrayExpanderFunction = Natives::get()->findFunction("$Array_$expand");
				auto *arrayLengthFunction = Natives::get()->findFunction("$Array_length");
				// calculate the size of the array (it must take into account the expansion of the arguments)
				auto *sizeType = llvm::Type::getInt64Ty(*llvm_context);
				auto *sizeCounter = builder.CreateAlloca(sizeType);
				builder.CreateStore(builder.getInt64(0), sizeCounter);
				vector<llvm::Value *> sizes;
				for (const auto &arg: ast) {
					llvm::Value *inc;
					auto *value = parseCall(arg->call(), contextObject);
					if (arg->DOTS()) {
						inc = builder.CreateCall(arrayLengthFunction, {value});
					} else {
						inc = builder.getInt64(1);
					}
					sizes.push_back(inc);
					builder.CreateStore(builder.CreateAdd(builder.CreateLoad(sizeType, sizeCounter), inc), sizeCounter);
				}
				auto *arrayElementType = llvm::PointerType::getUnqual(*llvm_context);
				auto *mallocSize = builder.CreateMul(builder.CreateLoad(sizeType, sizeCounter), llvm::ConstantExpr::getSizeOf(arrayElementType));
				auto *array = builder.CreateCall(mallocFunction, {mallocSize});
				auto *i = builder.CreateAlloca(sizeType);
				builder.CreateStore(builder.getInt64(0), i);
				auto sizesIter = sizes.begin();
				for (const auto &arg: ast) {
					auto *value = parseCall(arg->call(), contextObject);
					auto *size = *sizesIter++;
					if (arg->DOTS()) {
						auto *expValues = builder.CreateCall(arrayExpanderFunction, {value});
						auto *expPtr = builder.CreateBitCast(expValues, arrayElementType->getPointerTo());
						auto *destPtr = builder.CreateGEP(arrayElementType, array, {builder.CreateLoad(sizeType, i)});
						builder.CreateMemCpy(destPtr, llvm::MaybeAlign(8), expPtr, llvm::MaybeAlign(8), size);
					} else {
						auto *ptr = builder.CreateGEP(arrayElementType, array, {builder.CreateLoad(sizeType, i)});
						builder.CreateStore(value, ptr);
					}
					builder.CreateStore(builder.CreateAdd(builder.CreateLoad(sizeType, i), size), i);
				}
				return {array, builder.CreateLoad(sizeType, sizeCounter)};
			} else {
				auto *nullptrValue = llvm::ConstantPointerNull::get(llvm::PointerType::getUnqual(*llvm_context));
				return {nullptrValue, builder.getInt64(0)};
			}
		}

		llvm::Value *parseArray(PoolParser::ArrayContext *ast, llvm::Value *contextObject) {
			const auto &[args, size] = parseArgs(ast->arg(), contextObject);
			auto *classNewFunction = Natives::get()->findFunction("$Array_$new");
			auto *classNew = builder.CreateCall(classNewFunction, {size, args, contextObject});
			return classNew;
		}

		llvm::Value *parsePar(PoolParser::ParContext *ast, llvm::Value *contextObject) {
			if (const auto &exp = ast->expression()) {
				return parseExpression(exp, contextObject);
			} else {
				return Natives::get()->find("$Null");
			}
		}

		llvm::Value *parseExpression(PoolParser::ExpressionContext *ast, llvm::Value *contextObject) {
			if (ast->a) {
				return parseAssignment(ast->a, contextObject);
			} else if (ast->c) {
				return parseCall(ast->c, contextObject);
			} else throw compile_error("invalid expression", tptl(ast->start, ast->stop));
		}

		llvm::Value *parseTerm(PoolParser::TermContext *ast, llvm::Value *contextObject) {
			switch (ast->type) {
				case PoolParser::TermContext::NUM:
					return parseNum(ast->num(), contextObject);
				case PoolParser::TermContext::STR:
					return parseString(ast->string(), contextObject);
				case PoolParser::TermContext::FUN:
					return parseFunction(ast->fun(), contextObject);
				case PoolParser::TermContext::PAR:
					return parsePar(ast->par(), contextObject);
				case PoolParser::TermContext::BLK:
					return parseBlock(ast->block(), contextObject);
				case PoolParser::TermContext::ARR:
					return parseArray(ast->array(), contextObject);
				case PoolParser::TermContext::NSM:
					return parseNative(ast->NATIVE_SYMBOL());
				case PoolParser::TermContext::IDT:
					return parseIdentifier(ast->ID()->getSymbol(), contextObject);
				default:
					throw compile_error("invalid term", tptl(ast->start, ast->stop));
			}
		}

		llvm::Value *parseCall(PoolParser::CallContext *ast, llvm::Value *contextObject) {
			switch (ast->type) {
				case PoolParser::CallContext::T: {
					return parseTerm(ast->term(), contextObject);
				}
				case PoolParser::CallContext::A: {
					const auto &caller = parseCall(ast->callee, contextObject);
					const auto &[args, size] = parseArgs(ast->a->arg(), contextObject);
					auto *callFunction = Natives::get()->findFunction("$Function_call");
					auto *call = builder.CreateCall(callFunction, {caller, args, size});
					return call;
				}
				case PoolParser::CallContext::IA:
				case PoolParser::CallContext::ILA: {
					const auto &caller = parseCall(ast->callee, contextObject);
					auto *identifier = parseIdentifier(ast->ID()->getSymbol(), caller, ast->type == PoolParser::CallContext::ILA);
					if (ast->a) {
						const auto &[args, size] = parseArgs(ast->a->arg(), contextObject);
						auto *callFunction = Natives::get()->findFunction("$Function_call");
						auto *call = builder.CreateCall(callFunction, {identifier, args, size});
						return call;
					} else {
						return identifier;
					}
				}
				default:
					throw compile_error("invalid call", tptl(ast->start, ast->stop));
			}
		}

		llvm::Function *parseStatements(const vector<PoolParser::StatementContext *> &statements) {
			auto *previousBlock = builder.GetInsertBlock();
			auto *functionType = llvm::FunctionType::get(llvm::PointerType::getUnqual(*llvm_context), {llvm::PointerType::getUnqual(*llvm_context)}, false);
			auto *function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "_fun_" + to_string(intFunIdx++), *module);
			auto *ctxArg = function->getArg(0);
			ctxArg->setName("context");
			auto *entry = llvm::BasicBlock::Create(*llvm_context, "entry", function);
			builder.SetInsertPoint(entry);
			llvm::Value *result = nullptr;
			for (const auto &statement: statements) {
				if (const auto &exp = statement->expression()) {
					result = parseExpression(exp, ctxArg);
				} else {
					result = Natives::get()->find("$Null");
				}
			}
			builder.CreateRet(result);
			builder.SetInsertPoint(previousBlock);
			return function;
		}

	public:
		explicit ParserImpl(const shared_ptr<PoolInstanceImpl> &poolInstance)
				: Parser(poolInstance), builder(*this->llvm_context) {}

		void parseProgram(PoolParser::ProgramContext *ast) override {
			auto *fun = parseStatements(ast->statement());
			auto *mainType = llvm::FunctionType::get(llvm::PointerType::getUnqual(*llvm_context), false);
			auto *mainFunction = llvm::Function::Create(mainType, llvm::Function::ExternalLinkage, "_start", *module);
			auto *entry = llvm::BasicBlock::Create(*llvm_context, "entry", mainFunction);
			builder.SetInsertPoint(entry);
			// create the module
			auto *moduleNewFunction = Natives::get()->findFunction("$Module_$new");
			auto *moduleName = builder.CreateGlobalStringPtr(llvm::StringRef(module->getName()));
			auto *moduleNew = builder.CreateCall(moduleNewFunction, {moduleName, fun, Natives::get()->findConstant("$ModuleClass")});
			// execute the module
			auto *moduleExecuteFunction = Natives::get()->findFunction("$Module_execute");
			builder.CreateCall(moduleExecuteFunction, {moduleNew});
			// return the module
			builder.CreateRet(moduleNew);
		}
	};

	shared_ptr<pool::Parser> pool::Parser::create(const shared_ptr<PoolInstanceImpl> &poolInstance) {
		return make_shared<ParserImpl>(poolInstance);
	}

	Parser::Parser(const shared_ptr<PoolInstanceImpl> &poolInstance)
			: poolInstance(poolInstance),
			  llvm_context(make_unique<llvm::LLVMContext>()),
			  module(make_unique<llvm::Module>(poolInstance->path, *this->llvm_context)) {}
}
#pragma clang diagnostic pop
