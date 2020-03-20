use gtk::{
    prelude::*,
    Builder
};
use gio::prelude::*;
use crate::{
    get_obj,
    error::RdgResult,
    profiles::{
        Profile,
        Profiles
    },
    settings::Settings,
    connections::Connections
};
use std::sync::{Arc, Mutex, atomic::Ordering};
use padlock;

pub struct Rdg {
    connections: Arc<Connections>,
    count_update_trigger: glib::Sender<()>,
    profiles: Arc<Mutex<Profiles>>,
    settings: Arc<Mutex<Settings>>,
    // Main window, main view
    main_window: gtk::ApplicationWindow,
    header_bar: gtk::HeaderBar,
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

impl Rdg {

    pub fn build(app: &gtk::Application) -> RdgResult<Self> {

        let builder = Builder::new_from_resource("/net/olback/rdg/ui");
        let (tx, rx) = glib::MainContext::channel::<()>(glib::Priority::default());

        let inner = Self {
            connections: Arc::new(Connections::new()),
            count_update_trigger: tx,
            profiles: Arc::new(Mutex::new(Profiles::load()?)),
            settings: Arc::new(Mutex::new(Settings::load()?)),
            main_window: get_obj!(builder, "main_window"),
            header_bar: get_obj!(builder, "header_bar"),
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

        inner.setup();
        inner.connect_subtitle(rx);
        inner.connect_host_changed();
        inner.connect_connect_button();
        inner.connect_info_bar();
        inner.connect_menu(app.clone());
        inner.connect_settings();
        inner.connect_extra_args();

        inner.main_window.set_application(Some(app));
        inner.main_window.show();

        Ok(inner)

    }

    fn setup(&self) {

        let settings = padlock::mutex_lock(&self.settings, |lock| lock.clone());
        let profiles = padlock::mutex_lock(&self.profiles, |lock| lock.clone());

        // Keymap
        if settings.keymap_path.exists() {
            match std::fs::read_dir(&settings.keymap_path) {
                Ok(dirs) => {
                    self.keymap.remove_all();
                    for dir in dirs {
                        let d = dir.expect("error");
                        let t = d.file_type().expect("could not determine file type");
                        if t.is_file() {
                            self.keymap.append_text(d.file_name().to_str().expect("failed to convert filename to str"))
                        }
                    }
                    Self::cbt_set(&self.keymap, "en-us", "en-us");
                },
                Err(e) => {
                    self.show_error(&format!("Error reading keymap dir: {}", e));
                }
            }
        } else {
            self.show_error(&format!("Error: Keymap path not found"));
        }

        if !settings.rdesktop_path.exists() {
            self.show_error(&format!("Error: Could not find rdesktop binary"));
        }

        self.host.remove_all();
        for (k, _) in profiles.iter() {
            self.host.append_text(k.as_str());
        }

    }

    fn connect_subtitle(&self, rx: glib::Receiver<()>) {

        let header_bar = self.header_bar.clone();
        let count = Arc::clone(&self.connections.count());
        rx.attach(None, move |_| {

            let amount = count.load(Ordering::SeqCst);

            if amount == 0 {
                header_bar.set_subtitle(Some("Connect via RDP"));
            } else {
                header_bar.set_subtitle(Some(&format!("Active connections: {}", amount)));
            }

            glib::source::Continue(true)

        });

    }

    fn connect_host_changed(&self) {

        let port = self.port.clone();
        let username = self.username.clone();
        let domain = self.domain.clone();
        let width = self.width.clone();
        let height = self.height.clone();
        let keymap = self.keymap.clone();
        let experience = self.experience.clone();
        let color_depth = self.color_depth.clone();
        let fullscreen = self.fullscreen.clone();
        let disable_encryption = self.disable_encryption.clone();
        let compression = self.compression.clone();
        let cache_bitmaps = self.cache_bitmaps.clone();
        let numlock_sync = self.numlock_sync.clone();
        let extra_args = self.extra_args.clone();
        let profiles = Arc::clone(&self.profiles);
        self.host.connect_changed(move |val| {

            let profile = match val.get_active_text().map(|v| v.to_string()) {
                Some(addr) => {
                    match padlock::mutex_lock(&profiles, |lock| lock.get(addr).map(|v| v.clone())) {
                        Some(p) => p,
                        None => return
                    }
                },
                None => return
            };

            port.set_text(&format!("{}", profile.port));
            username.set_text(profile.username.as_str());
            domain.set_text(profile.domain.as_str());
            width.set_text(&format!("{}", profile.width));
            height.set_text(&format!("{}", profile.height));
            Self::cbt_set(&keymap, &profile.keymap, "en-us");
            Self::cbt_set(&experience, &profile.experience, "lan");
            Self::cbt_set(&color_depth, &format!("{}", profile.color_depth), "15");
            fullscreen.set_active(profile.fullscreen);
            disable_encryption.set_active(profile.disable_encryption);
            compression.set_active(profile.compression);
            cache_bitmaps.set_active(profile.cache_bitmaps);
            numlock_sync.set_active(profile.sync_numlock);
            extra_args.set_text(&profile.extra_args);

        });

    }

    fn connect_connect_button(&self) {

        let info_bar = self.info_bar.clone();
        let info_bar_label = self.info_bar_label.clone();
        let host_entry = self.host_entry.clone();
        let port = self.port.clone();
        let username = self.username.clone();
        let domain = self.domain.clone();
        let width = self.width.clone();
        let height = self.height.clone();
        let keymap = self.keymap.clone();
        let experience = self.experience.clone();
        let color_depth = self.color_depth.clone();
        let fullscreen = self.fullscreen.clone();
        let disable_encryption = self.disable_encryption.clone();
        let compression = self.compression.clone();
        let cache_bitmaps = self.cache_bitmaps.clone();
        let numlock_sync = self.numlock_sync.clone();
        let extra_args = self.extra_args.clone();
        let connections = Arc::clone(&self.connections);
        let profiles = Arc::clone(&self.profiles);
        let settings = Arc::clone(&self.settings);
        let count_update_trigger = self.count_update_trigger.clone();
        self.connect_button.connect_clicked(move |_| {

            let host = host_entry.get_text().map(|v| v.to_string()).unwrap_or("".into());
            if host.is_empty() {
                info_bar_label.set_text("Address may not be empty");
                info_bar.set_message_type(gtk::MessageType::Error);
                info_bar.set_visible(true);
                return
            }

            let port = match port.get_text().map(|v| v.to_string()).unwrap_or("3389".into()).parse::<u16>() {
                Ok(p) => p,
                Err(e) => {
                    info_bar_label.set_text(&format!("Error parsing port: {}", e));
                    info_bar.set_message_type(gtk::MessageType::Error);
                    info_bar.set_visible(true);
                    return
                }
            };

            let width = match width.get_text().map(|v| v.to_string()).unwrap_or("1366".into()).parse::<u32>() {
                Ok(w) => w,
                Err(e) => {
                    info_bar_label.set_text(&format!("Error parsing width: {}", e));
                    info_bar.set_message_type(gtk::MessageType::Error);
                    info_bar.set_visible(true);
                    return
                }
            };

            let height = match height.get_text().map(|v| v.to_string()).unwrap_or("768".into()).parse::<u32>() {
                Ok(w) => w,
                Err(e) => {
                    info_bar_label.set_text(&format!("Error parsing height: {}", e));
                    info_bar.set_message_type(gtk::MessageType::Error);
                    info_bar.set_visible(true);
                    return
                }
            };

            let color_depth = match color_depth.get_active_text().map(|v| v.to_string()).unwrap_or("15".into()).parse::<u8>() {
                Ok(w) => w,
                Err(e) => {
                    info_bar_label.set_text(&format!("Error parsing color depth: {}", e));
                    info_bar.set_message_type(gtk::MessageType::Error);
                    info_bar.set_visible(true);
                    return
                }
            };

            let profile = Profile {
                host: host.clone(),
                port: port,
                username: username.get_text().map(|v| v.to_string()).unwrap_or("".into()),
                domain: domain.get_text().map(|v| v.to_string()).unwrap_or("".into()),
                width: width,
                height: height,
                keymap: keymap.get_active_text().map(|v| v.to_string()).unwrap_or("en-us".into()),
                experience: experience.get_active_text().map(|v| v.to_string()).unwrap_or("lan".into()),
                color_depth: color_depth,
                fullscreen: fullscreen.get_active(),
                disable_encryption: disable_encryption.get_active(),
                compression: compression.get_active(),
                cache_bitmaps: cache_bitmaps.get_active(),
                sync_numlock: numlock_sync.get_active(),
                extra_args: extra_args.get_text().map(|v| v.to_string()).unwrap_or("".into())
            };

            let settings = padlock::mutex_lock(&settings, |lock| lock.clone());

            connections.connect(&profile, &settings, count_update_trigger.clone());

            padlock::mutex_lock(&profiles, move |lock| {
                drop(lock.insert(profile));
            });

            info_bar.set_visible(false);

            drop(count_update_trigger.send(()));

        });

    }

    fn connect_info_bar(&self) {

        self.info_bar.connect_response(|info_bar, _| {
            info_bar.set_visible(false);
        });

    }

    fn connect_menu(&self, app: gtk::Application) {

        let about_dialog = self.about_dialog.clone();
        self.open_about.connect_clicked(move |_| {
            match about_dialog.run() {
                _ => about_dialog.hide()
            }
        });

        let stack = self.stack.clone();
        self.open_settings.connect_clicked(move |_| {
            stack.set_visible_child_name("settings_view");
        });

        let extra_args_window = self.extra_args_window.clone();
        self.open_extra_args.connect_clicked(move |_| {
            extra_args_window.show();
        });

        self.quit.connect_clicked(move |_| {
            app.quit();
        });

    }

    fn connect_settings(&self) {

        let settings = padlock::mutex_lock(&self.settings, |lock| {
            lock.clone()
        });

        self.allow_untrusted_cert.set_active(settings.allow_untrusted_cert);
        self.rdesktop_path.set_filename(settings.rdesktop_path);
        self.keymap_path.set_filename(settings.keymap_path);

        let stack = self.stack.clone();
        self.settings_back.connect_clicked(move |_| {
            stack.set_visible_child_name("main_view");
        });

        let info_bar = self.info_bar.clone();
        let info_bar_label = self.info_bar_label.clone();
        let settings = Arc::clone(&self.settings);
        let allow_untrusted_cert = self.allow_untrusted_cert.clone();
        let rdesktop_path = self.rdesktop_path.clone();
        let keymap_path = self.keymap_path.clone();
        self.settings_save.connect_clicked(move |_| {

            let allow_untrusted_cert = allow_untrusted_cert.get_active();
            let rdesktop_path = rdesktop_path.get_filename();
            let keymap_path = keymap_path.get_filename();

            let new_settings = padlock::mutex_lock(&settings, move |lock| {
                lock.allow_untrusted_cert = allow_untrusted_cert;
                match rdesktop_path {
                    Some(v) => lock.rdesktop_path = v,
                    None => {}
                };
                match keymap_path {
                    Some(v) => lock.keymap_path = v,
                    None => {}
                };
                lock.clone()
            });

            match new_settings.save() {

                Ok(_) => {
                    info_bar_label.set_text("Settings saved");
                    info_bar.set_message_type(gtk::MessageType::Info);
                    info_bar.set_visible(true);
                },

                Err(e) => {
                    info_bar_label.set_text(&format!("Error saving: {}", e));
                    info_bar.set_message_type(gtk::MessageType::Error);
                    info_bar.set_visible(true);
                }

            }

        });

    }

    fn connect_extra_args(&self) {

        let extra_args_window = self.extra_args_window.clone();
        self.extra_args_button.connect_clicked(move |_| {
            extra_args_window.show();
        });

        let extra_args_window = self.extra_args_window.clone();
        self.extra_args_ok.connect_clicked(move |_| {
            extra_args_window.hide();
        });

        self.extra_args_window.connect_delete_event(|window, _| {
            window.hide();
            glib::signal::Inhibit(true)
        });

    }

    fn cbt_set(cbt: &gtk::ComboBoxText, search: &str, default: &str) {

        let model = cbt.get_model().expect("could not get model");
        let iter = model.get_iter_from_string("0:1").expect("failed to get iter from string");
        let mut index = 0;
        loop {
            let val: String = model.get_value(&iter, 0).get().expect("failed to get value").unwrap_or(String::from(default));
            if val.trim() == search.trim() {
                cbt.set_active(Some(index));
                break;
            }
            index += 1;
            if !model.iter_next(&iter) {
                break;
            }
            // In case something goes wrong, just crash instead of looping indefinitely
            if index > 100 {
                unreachable!("no match found")
            }
        }

    }

    fn show_error(&self, msg: &str) {

        self.info_bar_label.set_text(msg);
        self.info_bar.set_message_type(gtk::MessageType::Error);
        self.info_bar.set_visible(true);

    }

}
