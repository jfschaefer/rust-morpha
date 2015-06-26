fn main() {
    let out_dir = std::env::var("OUT_DIR").unwrap();
    let out_path = |f : &str| format!("{}/{}", out_dir, f);
    std::process::Command::new("gcc")
        .args(&["src/morpha_interface.c", "-c", "-fPIC", "-o",
                &out_path("morpha_interface.o")])
        .status().unwrap();
    std::process::Command::new("ar")
        .args(&["-crs", &out_path("/libmorpha_interface.a"),
                &out_path("/morpha_interface.o")])
        .status().unwrap();
    println!("cargo:rustc-link-search=native={}", out_dir);
    println!("cargo:rustc-link-lib=static=morpha_interface");
}
