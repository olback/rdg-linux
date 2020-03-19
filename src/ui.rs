use gtk::{
    prelude::*,
    Builder
};
use crate::get_obj;

pub struct Ui {
    // Main window, main view
    main_window: gtk::ApplicationWindow,
    info_bar: gtk::InfoBar,
    info_bar_label: gtk::Label,
    stack: gtk::Stack,
    // Main window, main view
    host: gtk::ComboBoxText,
    host_entry: gtk::Entry,
    port: gtk::Entry,
    username: gtk::Entry,
    domain: gtk::Entry,
    width: gtk::Entry,
    height: gtk::Entry,
    keymap: gtk::ComboBoxText,
    extra_args_button: gtk::Button,
    connect_button: gtk::Button,
    // Main window, settings view
    settings_back: gtk::Button,
    allow_untrusted_cert: gtk::Switch,
    rdesktop_path: gtk::FileChooserButton,
    keymap_path: gtk::FileChooserButton,
    settings_save: gtk::Button,
    // Extra args window
    extra_args_window: gtk::Window,
    experience: gtk::ComboBoxText,
    color_depth: gtk::ComboBoxText,
    fullscreen: gtk::Switch,
    disable_encryption: gtk::Switch,
    compression: gtk::Switch,
    cache_bitmaps: gtk::Switch,
    numlock_sync: gtk::Switch,
    extra_args: gtk::Entry,
    extra_args_ok: gtk::Button,
    // About dialog
    about_dialog: gtk::AboutDialog,
    // Main menu items
    open_about: gtk::Button,
    open_settings: gtk::Button,
    open_extra_args: gtk::Button,
    quit: gtk::Button
}

impl Ui {

    pub fn build(app: &gtk::Application) -> Self {

        let builder = Builder::new_from_resource("/net/olback/rdg/ui");

        let inner = Self {
            main_window: get_obj!(builder, "main_window"),
            info_bar: get_obj!(builder, "info_bar"),
            info_bar_label: get_obj!(builder, "info_bar_label"),
            stack: get_obj!(builder, "stack0"),
            // Main window, main view
            host: get_obj!(builder, "host"),
            host_entry: get_obj!(builder, "host_entry"),
            port: get_obj!(builder, "port"),
            username: get_obj!(builder, "username"),
            domain: get_obj!(builder, "domain"),
            width: get_obj!(builder, "width"),
            height: get_obj!(builder, "height"),
            keymap: get_obj!(builder, "keymap"),
            extra_args_button: get_obj!(builder, "more_options"),
            connect_button: get_obj!(builder, "connect"),
            // Main window, settings view
            settings_back: get_obj!(builder, "settings_back"),
            allow_untrusted_cert: get_obj!(builder, "allow_untrusted_cert"),
            rdesktop_path: get_obj!(builder, "rdesktop_path"),
            keymap_path: get_obj!(builder, "keymap_path"),
            settings_save: get_obj!(builder, "save_settings"),
            // Extra args window
            extra_args_window: get_obj!(builder, "extra_args_window"),
            experience: get_obj!(builder, "experience"),
            color_depth: get_obj!(builder, "color_depth"),
            fullscreen: get_obj!(builder, "fullscreen"),
            disable_encryption: get_obj!(builder, "disable_encryption"),
            compression: get_obj!(builder, "compression"),
            cache_bitmaps: get_obj!(builder, "cache_bitmaps"),
            numlock_sync: get_obj!(builder, "numlock_sync"),
            extra_args: get_obj!(builder, "extra_args"),
            extra_args_ok: get_obj!(builder, "extra_args_ok"),
            // About dialog
            about_dialog: get_obj!(builder, "about_dialog"),
            // Main menu items
            open_about: get_obj!(builder, "open_about"),
            open_settings: get_obj!(builder, "open_settings"),
            open_extra_args: get_obj!(builder, "open_extra_args"),
            quit: get_obj!(builder, "quit")
        };

        unimplemented!()

    }

}
