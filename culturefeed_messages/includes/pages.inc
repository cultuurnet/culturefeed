<?php
/**
 * @file
 * Page callbacks for culturefeed messages.
 */

/**
 * Show the form to add a new message.
 */
function culturefeed_messages_new_message_form($form, $form_state, $page = NULL) {

  $form = array();

  $recipient_options = array();

  // If we are on a page. Options are to members or to admins.
  if (!empty($page) && $page instanceof CultureFeed_Cdb_Item_Page) {

    try {

      $cf_pages = DrupalCultureFeed::getConsumerInstance()->pages();
      $user_list = $cf_pages->getUserList($page->getId(), array(CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN, CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER), FALSE);

      if (!empty($user_list->memberships)) {

        $member_count = 0;
        $admin_count = 0;

        foreach ($user_list->memberships as $user_list_membership) {
          if ($user_list_membership->role == CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN) {
            $admin_count++;
          }
          else {
            $member_count++;
          }
        }

      }

      $recipient_options[CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_ADMIN] = 'alle beheerders van mijn pagina (' . $admin_count . ')';
      $recipient_options[CultureFeed_Pages_Membership::MEMBERSHIP_ROLE_MEMBER] = 'alle leden van mijn pagina (' . $member_count . ')';

    }
    catch (Exception $e) {
      watchdog_exception('culturefeed_messages', $e);
    }

  }

  $form['recipient'] = array(
    '#type' => 'radios',
    '#title' => 'Stuur bericht naar:',
    '#options' => $recipient_options,
  );

  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => 'Onderwerp',
  );

  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => 'Berichttekst',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Verzenden',
  );

  return $form;

}