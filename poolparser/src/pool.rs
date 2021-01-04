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

#[derive(Serialize, Deserialize, Debug, Clone)]
pub enum AstNode {
	Call {
		caller: Box<AstNode>,
		method: String,
		callee: Box<AstNode>,
	},
	Invocation {
		caller: Box<AstNode>,
		args: Vec<AstNode>,
	},
	Tuple(Vec<AstNode>),
	Array(Vec<AstNode>),
	Block {
		params: Vec<String>,
		calls: Vec<AstNode>,
	},
	Op(String),
	Identifier(String),
	Str(String),
	Integer(i64),
	Decimal(f64),
	Bool(bool),
	Null(),
	Void(),
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
			AstNode::Call {
				caller,
				method,
				callee,
			} => {
				write!(f, "{} {} {}", caller, method, callee);
			}
			AstNode::Invocation { caller, args } => {
				write!(f, "{} ", caller);
				write!(f, "(");
				for x in args {
					write!(f, "{},", x);
				}
				write!(f, ")");
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
			AstNode::Block { params, calls } => {
				write!(f, "({}) : ", params.join(","));
				write!(f, "{{");
				for x in calls {
					writeln!(f, "{};", x);
				}
				write!(f, "}}");
			}
			AstNode::Op(op) => {
				write!(f, "{}", op);
			}
			AstNode::Identifier(id) => {
				write!(f, "{}", id);
			}
			AstNode::Void() => {
				write!(f, "void");
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
					statements.push(Pool::parse_statement(statement));
				}
				Rule::EOI => {}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		Ok(AstNode::Block {
			params: vec![],
			calls: statements,
		})
	}

	fn parse_statement(pair: Pair<Rule>) -> AstNode {
		let inner = pair.into_inner().next();
		return if inner.is_none() {
			AstNode::Void()
		} else {
			let statement = inner.unwrap();
			Pool::parse_expression(statement)
		};
	}

	fn parse_expression(statement: Pair<Rule>) -> AstNode {
		return match statement.as_rule() {
			Rule::call => Pool::parse_call(statement),
			_ => {
				println!("{}", statement);
				panic!()
			}
		};
	}

	fn parse_call(call: Pair<Rule>) -> AstNode {
		let mut inner = call.into_inner();
		let pair = inner.next().unwrap();
		let (caller, method) = if pair.clone().into_inner().next().is_some() {
			let option = inner.next();
			if option.is_none() {
				return Pool::parse_invocation(pair);
			}
			(Pool::parse_invocation(pair), option.unwrap().as_span().as_str().to_string())
		} else {
			(AstNode::Void(), pair.as_span().as_str().to_string())
		};
		let option = inner.next();
		let mut callee = AstNode::Void();
		if option.is_some() {
			callee = Pool::parse_invocation(option.unwrap());
		}
		return AstNode::Call {
			caller: Box::new(caller),
			method,
			callee: Box::new(callee),
		};
	}

	fn parse_invocation(call: Pair<Rule>) -> AstNode {
		let mut inner = call.into_inner();
		let caller = Pool::parse_access(inner.next().unwrap());
		let option = inner.next();
		return if option.is_some() {
			let args = Pool::parse_args(option.unwrap());
			AstNode::Invocation {
				caller: Box::new(caller),
				args,
			}
		} else {
			caller
		};
	}

	fn parse_params(pair: Pair<Rule>) -> Vec<String> {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::identifier => {
					vec.push(statement.as_span().as_str()[1..].to_string());
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return vec;
	}

	fn parse_args(pair: Pair<Rule>) -> Vec<AstNode> {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			vec.push(Pool::parse_expression(statement))
		}
		return vec;
	}

	fn parse_fun(call: Pair<Rule>) -> AstNode {
		let mut inner = call.into_inner();
		let params = Pool::parse_params(inner.next().unwrap());
		let block = Pool::parse_block(inner.next().unwrap());
		AstNode::Block {
			params,
			calls: block,
		}
	}

	fn parse_term(pair: Pair<Rule>) -> AstNode {
		let option = pair.into_inner().next();
		if option.is_none() {
			return AstNode::Void();
		}
		let term = option.unwrap();
		match term.as_rule() {
			Rule::term => Pool::parse_term(term),
			Rule::null => AstNode::Null(),
			Rule::identifier => Pool::parse_identifier(term),
			Rule::bool => AstNode::Bool(term.as_span().as_str() == "true"),
			Rule::string => Pool::parse_string(term),
			Rule::num => Pool::parse_num(term),
			Rule::block => AstNode::Block {
				params: vec![],
				calls: Pool::parse_block(term),
			},
			Rule::array => Pool::parse_array(term),
			Rule::tuple => Pool::parse_tuple(term),
			Rule::par => Pool::parse_term(term),
			Rule::call => Pool::parse_call(term),
			Rule::fun => Pool::parse_fun(term),
			Rule::access => Pool::parse_access(term),
			Rule::invocation => Pool::parse_invocation(term),
			_ => {
				println!("{}", term);
				panic!()
			}
		}
	}

	fn unescape(s: &str) -> String {
		let mut result = String::with_capacity(s.len());
		let mut chars = s.chars();
		while let Some(ch) = chars.next() {
			result.push(if ch != '\\' {
				ch
			} else {
				match chars.next() {
					Some('u') => {
						let value = chars.by_ref().take(4).fold(0, |acc, c| acc * 16 + c.to_digit(16).unwrap());
						std::char::from_u32(value).unwrap()
					}
					Some('b') => '\x08',
					Some('f') => '\x0c',
					Some('n') => '\n',
					Some('r') => '\r',
					Some('t') => '\t',
					Some(ch) => ch,
					_ => panic!("Malformed escape"),
				}
			})
		}
		return result;
	}

	fn parse_string(term: Pair<Rule>) -> AstNode {
		let str = term.into_inner().next().unwrap().as_span().as_str();
		return AstNode::Str(Pool::unescape(str));
	}

	fn parse_access(pair: Pair<Rule>) -> AstNode {
		let mut inner = pair.into_inner();
		let caller = Pool::parse_term(inner.next().unwrap());
		let mut vec = vec![];
		for statement in inner {
			vec.push(Pool::parse_identifier(statement));
		}
		return if vec.is_empty() {
			caller
		} else {
			let mut iter = vec.iter().cloned();
			let start = iter.next().unwrap();
			iter.fold(
				AstNode::Call {
					caller: Box::new(caller),
					method: ".".to_string(),
					callee: Box::new(start),
				},
				|a, b| AstNode::Call {
					caller: Box::new(a),
					method: ".".to_string(),
					callee: Box::new(b),
				},
			)
		};
	}

	fn parse_identifier(pair: Pair<Rule>) -> AstNode {
		let id = pair.as_span().as_str()[1..].to_string();
		return AstNode::Identifier(id);
	}

	fn parse_tuple(pair: Pair<Rule>) -> AstNode {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			vec.push(Pool::parse_expression(statement));
		}
		return AstNode::Tuple(vec);
	}

	fn parse_array(pair: Pair<Rule>) -> AstNode {
		let mut vec = vec![];
		for statement in pair.into_inner() {
			vec.push(Pool::parse_expression(statement));
		}
		return AstNode::Array(vec);
	}

	fn parse_block(pair: Pair<Rule>) -> Vec<AstNode> {
		let mut statements = vec![];
		for statement in pair.into_inner() {
			match statement.as_rule() {
				Rule::statement => {
					statements.push(Pool::parse_statement(statement));
				}
				_ => {
					println!("{}", statement);
					panic!()
				}
			}
		}
		return statements;
	}

	fn parse_num(pair: Pair<Rule>) -> AstNode {
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
