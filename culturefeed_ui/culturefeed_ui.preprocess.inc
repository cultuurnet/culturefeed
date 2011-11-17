<?php

/**
 * Preprocess variables for culturefeed-ui-profile.tpl.php.
 *
 * @see culturefeed-ui-profile.tpl.php
 */
function template_preprocess_culturefeed_ui_profile(&$variables) {
  $account = $variables['user'];
  $account instanceof CultureFeed_User;

  // nick
  $variables['nick'] = $account->nick;

  // name
  $name = array();

  if (!empty($account->givenName)) {
    $name[] = $account->givenName;
  }
  if (!empty($account->familyName)) {
    $name[] = $account->familyName;
  }

  $variables['name'] = !empty($name) ? implode(' ', $name) : '';

  // gender
  $variables['gender'] = '';
  if (!empty($account->gender)) {
    $variables['gender'] = $account->gender == CultureFeed_User::GENDER_MALE ? 'Man' : 'Vrouw';
  }

  // dob
  $variables['dob'] = !empty($account->dob) ? format_date($account->dob, 'custom', 'j/n/Y') : '';

  // bio
  $variables['bio'] = !empty($account->bio) ? $account->bio : '';

	// picture
	$variables['picture'] = $account->depiction ? theme('image', array('path' => $account->depiction . '?maxwidth=100')) : '';
	$variables['picture_change_link'] = '';
}

/**
 * Preprocess variables for culturefeed-ui-connect.tpl.php.
 *
 * @see culturefeed-ui-connect.tpl.php
 */
function template_preprocess_culturefeed_ui_connect(&$variables) {
  $variables['link_facebook'] = l('Facebook', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_twitter'] = l('Twitter', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_google'] = l('Google', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
}

/**
 * Preprocess variables for culturefeed-ui-mbox-confirmation-reminder.tpl.php.
 *
 * @see culturefeed-ui-mbox-confirmation-reminder.tpl.php
 */
function template_preprocess_culturefeed_ui_mbox_confirmation_reminder(&$variables) {
  $variables['mbox'] = $variables['account']->mbox;

  $variables['resend_link'] = l('Bevestiging opnieuw versturen', 'culturefeed/mboxverify/resend', array('query' => drupal_get_destination()));
  $variables['update_link'] = l('E-mailadres aanpassen', 'culturefeed/account/edit');
  $variables['info_link'] = l('Meer info over UiTID', '<front>');
}

/**
 * Preprocess variables for culturefeed-ui-activity-summary.tpl.php.
 *
 * @see culturefeed-ui-activity-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_profile_menu_item(&$variables) {
  $variables['link'] = l($variables['title'], $variables['url']);
}

/**
 * Preprocess variables for culturefeed-ui-online-account.tpl.php.
 *
 * @see culturefeed-ui-online-account.tpl.php
 */
function template_preprocess_culturefeed_ui_online_account(&$variables) {
  $account = $variables['account'];
  
  $variables['type'] = $variables['account_type'];
  
  $variables['name'] = !empty($account->accountName) ? $account->accountName: '';
  $variables['nick'] = !empty($account->accountNick) ? $account->accountNick : '';
  $variables['depiction'] = !empty($account->accountDepiction) ? $account->accountDepiction : '';
  
  $variables['picture'] = !empty($account->accountDepiction) ? theme('image', array('path' => $account->accountDepiction . '?maxwidth=50&maxheight=50&crop=auto')) : '';
}

/**
 * Preprocess variables for culturefeed-ui-activity-summary.tpl.php.
 *
 * @see culturefeed-ui-activity-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_activity_summary(&$variables) {
  $activity = $variables['activity'];
  $activity instanceof CultureFeed_Activity;

  $picture = theme('image', array('path' => $activity->depiction . '?maxwidth=50&maxheight=50&crop=auto'));
  $url = 'user/' . $variables['uid'];

  $variables['picture'] = l($picture, $url, array('html' => TRUE));
  $variables['date'] = format_interval(REQUEST_TIME - $activity->creationDate) . ' geleden.';

  $node = '';
  if ($activity->contentType == CultureFeed_Activity::CONTENT_TYPE_PAGE) {
    $node = l($activity->nodeTitle, 'node/' . $activity->nodeId);
  }
  else {
    $node = cnapi_url_dp2dul($activity->nodeTitle, array($activity->contentType => $activity->nodeId, 'title' => $activity->nodeTitle));
  }

  $type_prefix = '';
  $type_suffix = '';

  switch ($activity->type) {
    case CultureFeed_Activity::TYPE_VIEW:
      $type_prefix = 'heeft';
      $type_suffix = 'bekeken';
      break;
    case CultureFeed_Activity::TYPE_DETAIL:
      $type_prefix = 'heeft';
      $type_suffix = 'bekeken';
      break;
    case CultureFeed_Activity::TYPE_LIKE:
      $type_prefix = 'vindt';
      $type_suffix = 'leuk';
      break;
    case CultureFeed_Activity::TYPE_MAIL:
      $type_prefix = 'heeft';
      $type_suffix = 'gemaild';
      break;
    case CultureFeed_Activity::TYPE_PRINT:
      $type_prefix = 'heeft';
      $type_suffix = 'afgedrukt';
      break;
    case CultureFeed_Activity::TYPE_FACEBOOK:
      $type_prefix = 'heeft';
      $type_suffix = 'via Facebook gedeeld';
      break;
    case CultureFeed_Activity::TYPE_TWITTER:
      $type_prefix = 'heeft';
      $type_suffix = 'via Twitter gedeeld';
      break;
    case CultureFeed_Activity::TYPE_IK_GA:
      $type_prefix = 'gaat naar';
      $type_suffix = '';
      break;
  }

  $replace = array(
    '!nick' => l($activity->nick, $url),
    '!type_prefix' => $type_prefix,
    '!type_suffix' => $type_suffix,
    '!node' => $node,
  );

  $variables['text'] = rtrim(strtr('!nick !type_prefix !node !type_suffix', $replace), ' ') . '.';
}

/**
 * Preprocess variables for culturefeed-ui-ranked-user-summary.tpl.php.
 *
 * @see culturefeed-ui-ranked-user-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_ranked_user_summary(&$variables) {
  $user = $variables['user'];
  $user instanceof CultureFeed_User;

  template_preprocess_culturefeed_ui_user_summary($variables);
  $variables['sortValue'] = $user->sortValue;
}

/**
 * Preprocess variables for culturefeed-ui-ranked-user-summary.tpl.php.
 *
 * @see culturefeed-ui-user-summary.tpl.php
 */
function template_preprocess_culturefeed_ui_user_summary(&$variables) {
  $user = $variables['user'];
  $user instanceof CultureFeed_User;

  $picture = theme('image', array('path' => $user->depiction . '?maxwidth=50&maxheight=50&crop=auto', 'alt' => $user->nick));
  $url = 'user/' . $variables['uid'];
  $variables['picture'] = l($picture, $url, array('html' => TRUE, 'attributes' => array('title' => $user->nick)));
  $variables['nick'] = l($user->nick, $url);
}

/**
 * Preprocess variables for culturefeed-ui-profile-box.tpl.php.
 *
 * @see culturefeed-ui-profile-box.tpl.php
 */
function template_preprocess_culturefeed_ui_profile_box(&$variables) {
  $variables['is_logged_in'] = !user_is_anonymous();

  if (!user_is_anonymous()) {
    $url_profile = 'user/' . culturefeed_get_uid_for_cf_uid($variables['account']->id, $variables['account']->nick);

    $picture = $variables['account']->depiction . '?maxwidth=20&maxheight=20&crop=auto';
    $variables['picture'] = theme('image', array('path' => $picture));

    $variables['nick'] = l($variables['account']->nick, $url_profile);

    $variables['link_profile'] = l('Mijn UiTID', $url_profile);
  }

  $variables['link_logout'] = l('Afmelden', 'user/logout');
  $variables['link_login'] = l('Aanmelden', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_register'] = l('Registreren', 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
}

/**
 * Preprocess variables for culturefeed-ui-most-active-user.tpl.php.
 *
 * @see culturefeed-ui-most-active-user.tpl.php
 */
function template_preprocess_culturefeed_ui_most_active_user(&$variables) {
  $account = $variables['account'];
  $account instanceof CultureFeed_User;

  $url = 'user/' . culturefeed_get_uid_for_cf_uid($account->id, $account->nick);
  $picture = theme('image', array('path' => $account->depiction . '?maxwidth=93&maxheight=93&crop=auto'));

  $variables['picture'] = l($picture, $url, array('html' => TRUE));

  switch ($variables['sort']) {
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVITIES:
      $type_text = 'gedaan';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFLIKES:
      $type_text = 'leuk gevonden';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESFACEBOOK:
      $type_text = 'op Facebook gedeeld';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESTWITTER:
      $type_text = 'op Twitter gedeeld';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFATTENDS:
      $type_text = 'bijgewoond';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES:
      $type_text = 'gedaan';
      break;
  }

  $replace = array(
    '!nick' => l($account->nick, $url),
    '!sort_value' => $account->sortValue,
    '!type' => $type_text,
    '!uitslover' => l('UiTslover', 'culturefeed/users/mostactive'),
  );

  $variables['text'] = strtr('!nick heeft vorige week !sort_value activiteiten !type en is de !uitslover van de week. Proficiat !nick', $replace);
}

/**
 * Preprocess variables for culturefeed-ui-event-recommendation.tpl.php.
 *
 * @see culturefeed-ui-event-recommendation.tpl.php
 */
function template_preprocess_culturefeed_ui_event_recommendation(&$variables) {
  $item = $variables['event']->recommendationItem;

  // object_url
  $request_detail = array('event' => $item->id, 'title' => $item->title);
  $object_url = cnapi_url_dp2dua($request_detail);

  // id
  $variables['id'] = $variables['event']->itemid;

  // title
  $variables['title'] = l($item->title, $object_url['path'], $object_url['options']);

  // date
  $variables['date'] = !empty($item->from) ? $item->from : '';
  if (!empty($item->from) && !empty($item->from)) {
    $variables['date'] .= ' tot ' . $item->to;
  }
  
  // like
  $variables['like_link'] = !empty($variables['links']['like']) ? $variables['links']['like'] : '';
  
  // remove
  $variables['remove_link'] = !empty($variables['links']['remove']) ? $variables['links']['remove'] : '';

  // location
  $variables['location'] = !empty($item->location_simple) ? $item->location_simple : '';

  // more
  $variables['more'] = l('Meer info', $object_url['path'], $object_url['options']);
}

/**
 * Preprocess variables for culturefeed-ui-related-events.tpl.php.
 *
 * @see culturefeed-ui-related-events.tpl.php
 */
function template_preprocess_culturefeed_ui_related_events(&$variables) {
  // events
  $items = array();

  foreach ($variables['events'] as $event_info) {
    $items[] = theme('culturefeed_ui_event_recommendation', array('event' => $event_info, 'heading_level' => 3));
  }

  $variables['event_list'] = theme('item_list', array('items' => $items, 'attributes' => array('class' => 'event-recommendation-list')));

  // links
  $links = array();

  // links : headings
	if (!empty($variables['headings'])) {
  	foreach ($variables['headings'] as $heading) {
  	  $request = array('context' => 'event', 'query' => array('heading' => $heading['hid']));
  	  $links[] = cnapi_url_dp2dul('Meer ' . $heading['name'], $request);
  	}
	}

  // links : zip
	if (!empty($variables['cityid'])) {
		$cities = cnapi_get_cities();
		$city = $cities[$variables['cityid']];

		if ($city) {
  		$request = array('context' => 'event', 'query' => array('cityid' => $variables['cityid']));
  		$links[] = cnapi_url_dp2dul('Meer in ' . $city['zip'] . ' - ' . $city['name'], $request);
		}
	}

  $variables['link_list'] = theme('item_list', array('items' => $links));
}

/**
 * Preprocess variables for culturefeed-ui-related-events.tpl.php.
 *
 * @see culturefeed-ui-related-events.tpl.php
 */
function template_preprocess_culturefeed_ui_service_consumer_summary(&$variables) {
  $consumer = $variables['consumer'];
  $consumer instanceof CultureFeed_Consumer;

  $variables['logo'] = isset($consumer->logo) ? theme('image' , array('path' => $consumer->logo . '?maxwidth=100&maxheight=100&crop=auto')) : '';
  $variables['name'] = isset($consumer->name) ? $consumer->name : '';
  $variables['description'] = isset($consumer->description) ? $consumer->description : '';
  $variables['creation_date'] = 'Gekoppeld op ' . format_date($consumer->creationDate, 'custom', 'j F Y');
}