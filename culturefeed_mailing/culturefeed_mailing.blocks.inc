<?php
/**
 * @file
 * Block callback functions for culturefeed mailing.
 */

/**
 * Newsletter opt-in visibility callback.
 */
function culturefeed_mailing_block_newsletter_opt_in_is_visible() {
  return DrupalCultureFeed::isCultureFeedUser();
}

/**
 * Newsletter opt-in content callback.
 */
function culturefeed_mailing_block_newsletter_opt_in_view() {
  $block['subject'] = '';
  $block['content'] = theme('culturefeed_mailing_optin', array('mailing_id' => variable_get('culturefeed_mailing_list', '')));
  return $block;
}

/**
 * Show the newsletter form as block.
 */
function culturefeed_mailing_block_newsletter_form_view() {

  return array(
    'subject' => t('Newsletter'),
    'content' => drupal_get_form('culturefeed_mailing_newsletter_block_form'),
  );

}
