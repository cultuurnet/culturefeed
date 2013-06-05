<?php
/**
 * @file
 * Theming / preprocess functions for culturefeed userpoints ui.
 */


/**
 * Preprocess the culturefeed userpoints my balance block.
 * @see culturefeed-userpoints-ui-my-balance.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_my_balance(&$variables) {

  $variables['points'] = culturefeed_userpoints_ui_real_points($variables['points']);

}

/**
 * Preprocess the culturefeed userpoints my exchange page.
 * @see culturefeed-userpoints-ui-exchange-page.tpl.php
 */
function culturefeed_userpoints_ui_preprocess_culturefeed_userpoints_ui_exchange_page(&$variables) {

  $variables['points'] = culturefeed_userpoints_ui_real_points($variables['points']);

}
