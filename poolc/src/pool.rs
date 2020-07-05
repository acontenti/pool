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
	Ident(String),
	Str(String),
	Integer(i64),
	Decimal(f64),
	Bool(bool),
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
			AstNode::Call {
				caller,
				method,
				callee,
			} => {
				write!(f, "{} {} {}", caller, method, callee);
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
			AstNode::Empty() => {
				write!(f, "");
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
		let mut calls = Vec::new();
		for statement in file {
			match statement.as_rule() {
				Rule::call => {
					calls.push(self.parse_call(statement));
				}
				_ => {}
			}
		}
		Ok(AstNode::Block(calls))
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

	fn parse_term(&mut self, pair: Pair<Rule>) -> AstNode {
		let option = pair.into_inner().next();
		if option.is_none() {
			return AstNode::Empty();
		}
		let term = option.unwrap();
		match term.as_rule() {
			Rule::identifier => {
				let id = term.as_span().as_str().to_string();
				return AstNode::Ident(id);
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
			_ => {
				println!("{}", term);
				panic!()
			}
		}
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
		let mut calls = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::call => {
					calls.push(self.parse_call(statement));
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return AstNode::Block(calls);
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
