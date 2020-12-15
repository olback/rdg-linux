#!/bin/sh

if [[ $# -eq 1 && $1 == "uninstall" ]]; then

    # Uninstall
    rm /usr/share/applications/net.olback.rdg.desktop
    rm /usr/share/icons/hicolor/256x256/apps/net.olback.rdg.png
    rm /usr/bin/rdg

    echo "Uninstalled"

else
    if [[ -n $CARGO_TARGET_DIR ]]; then
        cp $CARGO_TARGET_DIR/release/rdg /usr/bin/rdg
    else
        cp target/release/rdg /usr/bin/rdg
    fi
    cp assets/net.olback.rdg.desktop /usr/share/applications/net.olback.rdg.desktop
    cp assets/net.olback.rdg.png /usr/share/icons/hicolor/256x256/apps/net.olback.rdg.png

    gtk-update-icon-cache /usr/share/icons/hicolor

    echo "Installed"

fi
