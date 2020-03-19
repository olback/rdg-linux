use crate::{impl_from, is_debug};

pub type RdgResult<T> = Result<T, RdgError>;

#[derive(Debug)]
pub struct RdgError {
    cause: String,
    file: String,
    line: u32,
    from: String
}

impl RdgError {

    #[allow(dead_code)]
    pub fn new<C: Into<String>>(cause: C, file: &str, line: u32) -> Self {

        Self {
            cause: cause.into(),
            file: String::from(file),
            line: line,
            from: String::from("error::RdgError")
        }

    }

    pub fn new_with_module<C: Into<String>>(cause: C, file: &str, line: u32, module: &str) -> Self {

        Self {
            cause: cause.into(),
            file: String::from(file),
            line: line,
            from: String::from(module)
        }

    }

}

impl std::fmt::Display for RdgError {

    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {

        if is_debug!() {
            write!(f, "{}#{}: {}", self.file, self.line, self.cause)
        } else {
            write!(f, "{}", self.cause)
        }

    }

}

impl_from!(std::io::Error);
impl_from!(serde_json::Error);
impl_from!(glib::error::BoolError);
