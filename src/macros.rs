#[macro_export]
macro_rules! new_err {
    ($e:expr) => {
        $crate::error::RdgError::new(format!("{}", $e), std::file!(), std::line!())
    };
}

#[macro_export]
macro_rules! is_debug {
    () => {
        if cfg!(debug_assertions) {
            true
        } else {
            std::env::var("RDG_DEBUG").is_ok()
        }
    };
}

#[macro_export]
macro_rules! impl_from {
    ($t:ty) => {
        impl From<$t> for RdgError {
            fn from(err: $t) -> RdgError {
                // super::new_err!(format!("{}", err))
                $crate::error::RdgError::new_with_module(format!("{}", err), std::file!(), std::line!(), stringify!($t))
            }
        }
    };
}

#[macro_export]
macro_rules! get_obj {
    ($builder:expr, $id:expr) => {
        // Catch and panic manually to get useful file and line info
        match $builder.get_object($id) {
            Some(o) => o,
            None => panic!("could not get {}", $id)
        }
    };
}
