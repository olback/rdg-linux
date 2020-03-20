mod error;
mod macros;
mod profiles;
mod settings;
mod connections;
mod rdg;
mod resources;

use gio::prelude::*;
use std::env::args;

fn main() -> error::RdgResult<()> {

    // Load resources
    resources::load();

    let application = gtk::Application::new(Some("net.olback.rdg"), Default::default())?;

    application.connect_activate(move |app| {

        let _ = rdg::Rdg::build(app).expect("failed to build app");

    });

    application.run(&args().collect::<Vec<String>>());

    Ok(())

}
