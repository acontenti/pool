use std::fmt::{Debug, Display, Formatter};
use std::fs;

use pest::error::Error;
use pest::iterators::{Pair, Pairs};
use pest::Parser;
use serde::{Deserialize, Serialize};

#[derive(Parser)]
#[grammar = "pool.pest"] // relative to src
pub struct PoolParser;

#[repr(C)]
pub struct Pool {
	file: String,
}

#[derive(Serialize, Deserialize, Debug)]
pub enum AstNode {
	Call {
		caller: Box<AstNode>,
		method: String,
		callee: Box<AstNode>,
	},
	Tuple(Vec<AstNode>),
	Array(Vec<AstNode>),
	Block(Vec<AstNode>),
	Fun {
		args: Box<AstNode>,
		block: Box<AstNode>,
	},
	Var(Vec<String>),
	Ident(String),
	Str(String),
	Integer(i64),
	Decimal(f64),
	Bool(bool),
	Null(),
	Empty(),
}

#[allow(unused_must_use)]
impl Display for AstNode {
	fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), std::fmt::Error> {
		match self {
			AstNode::Decimal(x) => {
				write!(f, "{}", x);
			}
			AstNode::Integer(x) => {
				write!(f, "{}", x);
			}
			AstNode::Str(x) => {
				write!(f, "{}", x);
			}
			AstNode::Bool(x) => {
				write!(f, "\"{}\"", x);
			}
			AstNode::Ident(x) => {
				write!(f, "{}", x);
			}
			AstNode::Call { caller, method, callee } => {
				write!(f, "{} {} {}", caller, method, callee);
			}
			AstNode::Fun { args, block } => {
				write!(f, "({}) : {}", args, block);
			}
			AstNode::Array(vec) => {
				write!(f, "[");
				for x in vec {
					write!(f, "{},", x);
				}
				write!(f, "]");
			}
			AstNode::Tuple(vec) => {
				write!(f, "(");
				for x in vec {
					write!(f, "{},", x);
				}
				write!(f, ")");
			}
			AstNode::Block(vec) => {
				write!(f, "{{");
				for x in vec {
					writeln!(f, "{};", x);
				}
				write!(f, "}}");
			}
			AstNode::Var(vec) => {
				write!(f, "{}", vec[0]);
				for x in vec.iter().skip(1) {
					write!(f, ".{}", x);
				}
				writeln!(f);
			}
			AstNode::Empty() => {
				write!(f, "");
			}
			AstNode::Null() => {
				write!(f, "null");
			}
		};
		Ok(())
	}
}

impl Pool {
	pub fn new(file: &str) -> Pool {
		Pool {
			file: file.to_string(),
		}
	}

	pub fn parse(&mut self) -> Result<AstNode, Error<Rule>> {
		let unparsed_file = fs::read_to_string(&self.file).expect("cannot read file");
		let file: Pairs<Rule> =
			PoolParser::parse(Rule::pool, &unparsed_file).unwrap_or_else(|e| panic!("{}", e));
		let mut statements = Vec::new();
		for statement in file {
			match statement.as_rule() {
				Rule::statement => {
					statements.push(self.parse_statement(statement));
				}
				Rule::EOI => {}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		Ok(AstNode::Block(statements))
	}

	fn parse_statement(&mut self, pair: Pair<Rule>) -> AstNode {
		let inner = pair.into_inner().next();
		return if inner.is_none() {
			AstNode::Empty()
		} else {
			let statement = inner.unwrap();
			match statement.as_rule() {
				Rule::call => {
					self.parse_call(statement)
				}
				Rule::term => {
					self.parse_term(statement)
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		};
	}

	fn parse_call(&mut self, call: Pair<Rule>) -> AstNode {
		let mut inner = call.into_inner();
		let caller = self.parse_term(inner.next().unwrap());
		let method = inner.next().unwrap().as_span().as_str().to_string();
		let callee = self.parse_term(inner.next().unwrap());
		AstNode::Call {
			caller: Box::new(caller),
			method,
			callee: Box::new(callee),
		}
	}

	fn parse_args(&mut self, pair: Pair<Rule>) -> AstNode {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::identifier => {
					vec.push(self.parse_identifier(statement));
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return AstNode::Tuple(vec);
	}

	fn parse_fun(&mut self, call: Pair<Rule>) -> AstNode {
		let mut inner = call.into_inner();
		let args = self.parse_args(inner.next().unwrap());
		let block = self.parse_block(inner.next().unwrap());
		AstNode::Fun {
			args: Box::new(args),
			block: Box::new(block),
		}
	}

	fn parse_term(&mut self, pair: Pair<Rule>) -> AstNode {
		let option = pair.into_inner().next();
		if option.is_none() {
			return AstNode::Empty();
		}
		let term = option.unwrap();
		match term.as_rule() {
			Rule::term => {
				return self.parse_term(term);
			}
			Rule::null => {
				return AstNode::Null();
			}
			Rule::variable => {
				return self.parse_variable(term);
			}
			Rule::bool => {
				let bool = term.as_span().as_str() == "true";
				return AstNode::Bool(bool);
			}
			Rule::string => {
				let str = term
					.into_inner()
					.next()
					.unwrap()
					.as_span()
					.as_str()
					.to_string();
				return AstNode::Str(str);
			}
			Rule::num => {
				return self.parse_num(term);
			}
			Rule::block => {
				return self.parse_block(term);
			}
			Rule::array => {
				return self.parse_array(term);
			}
			Rule::tuple => {
				return self.parse_tuple(term);
			}
			Rule::par => {
				return self.parse_term(term);
			}
			Rule::call => {
				return self.parse_call(term);
			}
			Rule::fun => {
				return self.parse_fun(term);
			}
			_ => {
				println!("{}", term);
				panic!()
			}
		}
	}

	fn parse_variable(&mut self, pair: Pair<Rule>) -> AstNode {
		let mut vec = vec![];
		for identifier in pair.into_inner() {
			match identifier.as_rule() {
				Rule::identifier => {
					vec.push(identifier.as_span().as_str().to_string());
				}
				_ => {
					println!("{}", identifier);
					panic!()
				}
			}
		}
		return AstNode::Var(vec);
	}

	fn parse_identifier(&mut self, pair: Pair<Rule>) -> AstNode {
		let id = pair.as_span().as_str().to_string();
		return AstNode::Ident(id);
	}

	fn parse_tuple(&mut self, pair: Pair<Rule>) -> AstNode {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::term => {
					vec.push(self.parse_term(statement));
				}
				Rule::call => {
					vec.push(self.parse_call(statement));
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return AstNode::Tuple(vec);
	}

	fn parse_array(&mut self, pair: Pair<Rule>) -> AstNode {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::term => {
					vec.push(self.parse_term(statement));
				}
				Rule::call => {
					vec.push(self.parse_call(statement));
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return AstNode::Array(vec);
	}

	fn parse_block(&mut self, pair: Pair<Rule>) -> AstNode {
		let mut statements = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::statement => {
					statements.push(self.parse_statement(statement));
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return AstNode::Block(statements);
	}

	fn parse_num(&mut self, pair: Pair<Rule>) -> AstNode {
		let term = pair.into_inner().next().unwrap();
		match term.as_rule() {
			Rule::integer => {
				let result = term
					.as_span()
					.as_str()
					.parse::<i64>()
					.expect("Integer parse failed");
				return AstNode::Integer(result);
			}
			Rule::decimal => {
				let result = term
					.as_span()
					.as_str()
					.parse::<f64>()
					.expect("Decimal parse failed");
				return AstNode::Decimal(result);
			}
			_ => {
				println!("{}", term);
				panic!()
			}
		}
	}
}
