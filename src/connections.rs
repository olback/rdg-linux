use std::{
    sync::{
        mpsc,
        Arc,
        atomic::{
            AtomicU8,
            Ordering
        }
    },
    thread::{
        self,
        JoinHandle,
    },
    io::{
        Read,
        Write
    },
    process::{
        Command,
        Stdio,
        ExitStatus
    }
};
use crate::{
    settings::Settings,
    profiles::Profile
};

pub struct Connections {
    count: Arc<AtomicU8>
}

impl Connections {

    pub fn new() -> Self {

        Self {
            count: Arc::new(AtomicU8::new(0))
        }

    }

    pub fn connect(&self, profile: &Profile, settings: &Settings) -> JoinHandle<ExitStatus> {

        self.count.fetch_add(1, Ordering::SeqCst);

        let p_args = profile.get_connect_args(&settings);
        let allow_invalid = settings.allow_invalid_cert;
        let rdesktop_path = settings.rdesktop_path.clone();
        let count = Arc::clone(&self.count);

        thread::spawn(move || {

            let mut child = Command::new(rdesktop_path)
            .args(p_args)
            .stdin(Stdio::piped())
            .stdout(Stdio::piped())
            .stderr(Stdio::piped())
            .spawn()
            .unwrap();

            loop {

                // Certificate warning
                let mut buf = [0u8; 256];
                let stderr = child.stderr.as_mut().unwrap();
                stderr.read(&mut buf[..]).unwrap();

                println!("{}", String::from_utf8_lossy(&buf[..]));

                let cert_error = String::from_utf8_lossy(&buf[..]).contains("ATTENTION!");
                if cert_error && allow_invalid {
                    child.stdin.as_mut().unwrap().write(b"yes\n").expect("failed to write to stdin");
                } else if cert_error && !allow_invalid {
                    child.stdin.as_mut().unwrap().write(b"no\n").expect("failed to write to stdin");
                }

                match child.try_wait() {

                    Ok(v) => {

                        match v {

                            Some(status) => {
                                println!("Status: {}", status);
                                count.fetch_sub(1, Ordering::SeqCst);
                                return status;
                            },

                            None => {}

                        }

                    },

                    Err(e) => eprintln!("{}", e)

                }

                std::thread::sleep(std::time::Duration::from_secs(1));

            }

        })

    }

}
