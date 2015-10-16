<?php

/**
 * Change the event before making the api call.
 *
 * @param CultureFeed_Cdb_Item_Event $event
 *   The event.
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function hook_culturefeed_entry_ui_event_pre_save_alter(CultureFeed_Cdb_Item_Event &$event, array $form, array $form_state) {
}

/**
 * Change the event before making the api call.
 *
 * @param CultureFeed_Cdb_Item_Event $event
 *   The event.
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function hook_culturefeed_entry_ui_event_post_save(CultureFeed_Cdb_Item_Event $event, array $form, array $form_state) {
}

/** Change the timeout url when event added and not synced.
 *
 * @param string $url
 *   The url.
 */
function hook_culturefeed_entry_ui_event_timeout_new_no_sync_redirect($url) {
}
