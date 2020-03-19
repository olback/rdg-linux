mod out;
mod resources;
mod glade;
mod version;

fn main() {

    println!("cargo:rerun-if-changed=.git/HEAD");
    println!("cargo:rerun-if-changed=assets/*");

    let version = version::get_version();

    out::output_dir();
    version::write_version(&version);
    glade::process(&version);
    resources::generate_resources();

}
