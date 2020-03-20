#!/bin/sh

if [[ $# -eq 1 && $1 == "uninstall" ]]; then

    # Uninstall
    rm /usr/share/applications/net.olback.rdg.desktop
    rm /usr/share/icons/hicolor/256x256/apps/net.olback.rdg.png
    rm /usr/bin/rdg

    echo "Uninstalled"

else

    cp target/release/rdg /usr/bin/rdg
    cp assets/net.olback.rdg.desktop /usr/share/applications/net.olback.rdg.desktop
    cp assets/net.olback.rdg.png /usr/share/icons/hicolor/256x256/apps/net.olback.rdg.png

    gtk-update-icon-cache /usr/share/icons/hicolor

    echo "Installed"

fi
