extern crate libc;

use libc::{c_char};
use std::ffi::{CString, CStr};
use std::str;

//C signatures
extern "C" {
    pub fn m_init();
    pub fn m_close();
    pub fn m_stem(input: *const c_char) -> *mut c_char;
    pub fn m_full_stem(input: *const c_char) -> *mut c_char;
}

///Will be called automatically, just provided for completeness
pub fn init() {
    unsafe { m_init() };
}

///Should be called after everything is done
pub fn close() {
    unsafe { m_close() };
}

///Stem once
pub fn stem(input: &str) -> String {
    let c_input = CString::new(input).unwrap().as_ptr();
    let c_output = unsafe {m_stem(c_input)};
    str::from_utf8(unsafe { CStr::from_ptr(c_output) }.to_bytes())
        .unwrap().to_owned()
}

///Stem as many times as something changes
///(For example tilings -> tiling -> tile)
pub fn full_stem(input: &str) -> String {
    let c_input = CString::new(input).unwrap().as_ptr();
    let c_output = unsafe {m_full_stem(c_input)};
    str::from_utf8(unsafe {CStr::from_ptr(c_output) }.to_bytes())
        .unwrap().to_owned()
}

#[test]
fn small_test() {
    assert_eq!(stem("tilings"), "tiling");
    assert_eq!(full_stem("tilings"), "tile");
    assert_eq!(full_stem("There are many words"), "there be many word");
    close();
}
