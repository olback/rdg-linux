mod error;
mod macros;
mod profiles;
mod settings;
mod connections;
mod ui;

use gtk::prelude::*;
use gio::prelude::*;

fn main() -> error::RdgResult<()> {

    // let p = profiles::Profiles::load().unwrap();
    // println!("{:#?}", p);

    // let s = settings::Settings::load().unwrap();
    // println!("{:#?}", s);

    // Load resources
    let glib_resource_bytes = glib::Bytes::from_static(include_bytes!("../out/barium.gresource"));
    let resources = gio::Resource::new_from_data(&glib_resource_bytes).expect("Failed to load resources");
    gio::resources_register(&resources);

    let application = gtk::Application::new(Some("net.olback.rdg"), Default::default())?;

    application.connect_activate(move |app| {

        let ui = ui::Ui::build(app);

    });

    Ok(())

}
