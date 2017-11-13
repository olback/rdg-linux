/**
 *  olback.net
 *  github.com/olback/rdg-linux
 */

void setKeymap() {
    sprintf(keymap, "%s", gtk_combo_box_text_get_active_text (keymapInput));
    printf("Keymap set to %s\n", keymap);
}