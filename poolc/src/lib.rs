extern crate pest;
#[macro_use]
extern crate pest_derive;

use std::ffi::{CStr, CString};
use std::os::raw::c_char;

mod pool;

#[no_mangle]
pub extern fn generate_json_ast(file: *const c_char) -> *const c_char {
	use pool::Pool;
	let c_str = unsafe { CStr::from_ptr(file) };
	let mut pool = Pool::new(c_str.to_str().expect("Invalid input"));
	let root = pool.parse().expect("parse error");
	let serialized = serde_json::to_string_pretty(&root).unwrap();
	println!("{}", serialized);
	return CString::new(serialized + "\0").unwrap().into_raw();
}
