<?php
/**
 * @file
 * Theming functions for cultuurkuur.
 */

/**
 * Theme 1 fellow members block.
 * @param $variables
 */
function culturefeed_pages_preprocess_culturefeed_pages_fellow_members_block(&$variables) {

  $uids = culturefeed_get_uids_for_memberships($variables['members']);

  $colleagues = array();
  foreach ($variables['members'] as $membership) {

    // If user was not found in drupal, don't show it.
    if (!isset($uids[$membership->user->id])) {
      continue;
    }

    $colleague = array();
    $colleague['url'] = url('user/' . $uids[$membership->user->id]);
    $colleague['name'] = $membership->user->nick;
    $colleague['picture'] = $membership->user->depiction;

    $colleagues[] = $colleague;

  }

  $variables['colleagues'] = $colleagues;
  $variables['nick'] = $variables['account']->nick;
  $variables['title'] = $variables['page']->name;

  if (culturefeed_pages_is_user_member_of_page($variables['page']->id)) {
    $variables['is_member'] = TRUE;
  }
  else {
    $variables['is_member'] = FALSE;
    $variables['become_member_url'] = url('<front>');
  }

}

/**
 * Theme the overview of pages that a user follows in a block.
 */
function theme_culturefeed_pages_following_pages_block($variables) {

  $items = array();
  foreach ($variables['following'] as $following) {
    $items[] = culturefeed_search_detail_l('page', $following->page->id, $following->page->name);
  }

  return theme('item_list', array('items' => $items, 'attributes' => array('class' => array('pages-following'))));

}