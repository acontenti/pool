extern crate pest;
#[macro_use]
extern crate pest_derive;

use std::env;

mod pool;

pub fn main() {
	use pool::Pool;
	let args: Vec<String> = env::args().collect();
	if args.len() == 2 {
		let mut pool = Pool::new(&args[1]);
		let root = pool.parse().expect("parse error");
		let serialized = serde_json::to_string_pretty(&root).unwrap();
		println!("{}", serialized);
	}
}
