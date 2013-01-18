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
  $variables['picture'] = !empty($account->depiction) ? theme('image', array('path' => $account->depiction . '?maxwidth=100')) : '';

  // city
  $variables['city'] = !empty($account->city) ? $account->city : '';

  // profile
  $variables['profile_edit_link'] = '';
  if (DrupalCultureFeed::getLoggedInUserId() == $account->id) {
    $variables['profile_edit_link'] = l('Beheer jouw profiel', 'culturefeed/profile/edit', array('attributes' => array('class' => array('profile-edit-link'))));
  }
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
  $variables['info_link'] = l('Meer info over UiTiD', 'http://www.uitinvlaanderen.be/over-uitid');
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

    $variables['link_profile'] = l('Mijn UiTiD', $url_profile);
  }

  $variables['link_logout'] = l('Afmelden', 'user/logout');
  $variables['link_login'] = l('Aanmelden', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $variables['link_register'] = l('Registreren', 'culturefeed/oauth/connect/register', array('attributes' => array('class' => array('culturefeedconnect')), 'query' => drupal_get_destination()));
  $query = drupal_get_destination();
  $query['via'] = 'facebook';
  $variables['link_login_facebook'] = l('Facebook', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-facebook')), 'query' => $query));
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
      $type_text = 'acties</strong> gedaan';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFLIKES:
      $type_text = 'pagina\'s</strong> leuk gevonden';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESFACEBOOK:
      $type_text = 'pagina\'s</strong> op Facebook gedeeld';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFSHARESTWITTER:
      $type_text = 'pagina\'s</strong> op Twitter gedeeld';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFATTENDS:
      $type_text = 'activiteiten</strong> gemarkeerd bij te zullen wonen';
      break;
    case CultureFeed_SearchUsersQuery::SORT_NUMBEROFACTIVEACTIVITIES:
      $type_text = 'acties</strong> gedaan';
      break;
  }

  $replace = array(
    '!nick' => l($account->nick, $url),
    '!sort_value' => $account->sortValue,
    '!type' => $type_text,
    '!uitslover' => l('UiTslover', 'culturefeed/users/mostactive'),
  );

  $variables['text'] = strtr('!nick heeft de afgelopen week <strong>!sort_value !type en is de !uitslover van de week. Proficiat!', $replace);
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
  $variables['date'] = $item->description_full;

  // thumbnail
  $variables['thumbnail'] = !empty($item->thumbnail) ? theme('image', array('path' => $item->thumbnail)) : '';

  // location
  $variables['location'] = !empty($item->location_simple) ? $item->location_simple : '';

  // address
  $variables['address'] = !empty($item->location_city) ? $item->location_city : '';

  // more
  $variables['more'] = l('Meer info', $object_url['path'], $object_url['options']);

  // like
  $variables['like_link'] = !empty($variables['links']['like']) ? $variables['links']['like'] : '';

  // remove
  $variables['remove_link'] = !empty($variables['links']['remove']) ? $variables['links']['remove'] : '';

  // algorithm
  $variables['algorithm'] = $variables['event']->algorithm;
  
  // algorithm description
  switch ($variables['event']->algorithm) {
    case "CBRecommender":
      $variables['algorithm_description'] = "Deze tip werd gegenereerd op basis van gelijkaardige items die je leuk vond. <span class=score>" .$variables['event']->score . "</span>";
        break;
    case "UserBasedCF":
      $variables['algorithm_description'] = "Deze tip werd gegenereerd op basis van gelijkaardige gebruikers die dit item leuk vonden. <span class=score>" .$variables['event']->score . "</span>";
        break;
    default:
      $variables['algorithm_description'] = "Deze tip werd gegenereerd op basis van je profiel (Fijnproever, Actiezoeker, Actieve ontspanner of Ontdekker).";
        break;
  }

}

/**
 * Preprocess variables for culturefeed-ui-top-event.tpl.php.
 *
 * @see culturefeed-ui-top-event.tpl.php
 */
function template_preprocess_culturefeed_ui_top_event(&$variables) {

  $event = $variables['event'];

  // object_url
  $request_detail = array('event' => $event['cdbid'], 'title' => $event['title']);
  $object_url = cnapi_url_dp2dua($request_detail);

  // id
  $variables['id'] = $event['cdbid'];

  // title
  $variables['title'] = l($event['title'], $object_url['path'], $object_url['options']);

  // date
  $variables['date'] = $event['calendarsummary'];

  // thumbnail
  $variables['thumbnail'] = !empty($event['thumbnail']) ? theme('image', array('path' => $event['thumbnail'])) : '';

  // location
  $variables['location'] = !empty($event['location']) ? $event['location'] : '';

  // address
  $variables['address'] = !empty($event['address']) ? $event['address'] : '';

  // city
  $variables['city'] = !empty($event['city']) ? $event['city'] : '';

  // more
  $variables['more'] = l('Meer info', $object_url['path'], $object_url['options']);

  // like
//  $variables['like_link'] = !empty($variables['links']['like']) ? $variables['links']['like'] : '';

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

  $variables['event_list'] = theme('item_list', array('items' => $items, 'attributes' => array('class' => 'event-recommendation-list event-teaser-list')));

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

  $variables['logo'] = isset($consumer->logo) ? theme('image' , array('path' => $consumer->logo . '?maxwidth=100&maxheight=100')) : '';
  $variables['name'] = isset($consumer->name) ? $consumer->name : '';
  $variables['description'] = isset($consumer->description) ? $consumer->description : '';
  $variables['creation_date'] = 'Gekoppeld op ' . format_date($consumer->creationDate, 'custom', 'j F Y');
}

/**
 * Preprocess variables for culturefeed-ui-my-uitid.tpl.php.
 */
function template_preprocess_culturefeed_ui_my_uitid(&$variables) {
  $account = $variables['account'];

  // contextual links
  $contextual_links = array(
    l('Bewerk profiel', 'culturefeed/profile/edit'),
    l('Mijn aanbevelingen', 'culturefeed/recommendations'),
    l('Beheer mijn acties', 'culturefeed/activities'),
    l('Bewerk account', 'culturefeed/account/edit'),
  );
  $item_list = array(
    '#theme' => 'item_list',
    '#items' => $contextual_links,
  );
  $variables['contextual_links'] = drupal_render($item_list);

  // nick
  $url_profile = 'user/' . culturefeed_get_uid_for_cf_uid($variables['account']->id, $variables['account']->nick);
  $variables['nick'] = l($account->nick, $url_profile);

  // name
  $name = array();

  if (!empty($account->givenName)) {
    $name[] = $account->givenName;
  }
  if (!empty($account->familyName)) {
    $name[] = $account->familyName;
  }

  $variables['name'] = !empty($name) ? l(implode(' ', $name), $url_profile) : '';

  // gender
  $variables['gender'] = '';
  if (!empty($account->gender)) {
    $variables['gender'] = $account->gender == CultureFeed_User::GENDER_MALE ? 'Man' : 'Vrouw';
  }

  // dob
  if (!empty($account->dob)) {
    $year = date('Y', $account->dob);
    $age = (date("md", date("U", $account->dob)) > date("md") ? ((date("Y") - $year) - 1) : (date("Y") - $year));
    $variables['age'] = $age;
  }
  else {
    $variables['age'] = '';
  }

  // picture
  $variables['picture'] = !empty($account->depiction) ? theme('image', array('path' => $account->depiction . '?width=50&height=50&crop=auto')) : '';

  // city
  $variables['city'] = !empty($account->city) ? $account->city : '';

  // profile
  $variables['profile_edit_link'] = NULL;
  if (!$account->lifestyleProfile) {
    $variables['profile_edit_link'] = l('Bereken je vrijetijdsprofiel', 'uitid/game', array('attributes' => array('class' => array('profile-edit-link cta'))));
  }
  else {
    // lifestyleProfile name
    switch ($account->lifestyleProfile) {
      case "AZ":
        $variables['profile_name'] = 'Actiezoeker';
          break;
      case "AO":
        $variables['profile_name'] = 'Actieve ontspanner';
          break;
      case "ONT":
        $variables['profile_name'] = 'Ontdekker';
          break;
      case "FP":
        $variables['profile_name'] = 'Fijnproever';
          break;
      default:
        $variables['profile_name'] = 'Profiel nog niet berekend';
          break;
    }    
    $variables['profile_refresh_link'] = l('Herbereken', 'uitid/game', array('attributes' => array('class' => array('profile-refresh-link'))));
  }

  // recommendations
  $variables['recommendations_link'] = l('Beoordeel nog enkele tips', 'culturefeed/recommendations', array('attributes' => array('class' => array('recommendations-link'))));

  // facebook toggle
  $variables['facebook_privacy_toggle'] = NULL;
  $facebook = _culturefeed_ui_facebook_privacy_toggle();
  if ($facebook) {
    $item_list = array(
      '#theme' => 'item_list',
      '#items' => array(
        array(
          'data' => $facebook,
          'id' => 'onlineaccount-facebook',
        ),
      ),
    );
    $facebook_privacy_toggle = array(
      '#markup' => drupal_render($item_list),
    );
    $variables['facebook_privacy_toggle'] = drupal_render($facebook_privacy_toggle);
  }
}

/**
 * Preprocess variables for culturefeed-ui-connect.tpl.php.
 *
 * @see culturefeed-ui-connect.tpl.php
 */
function template_preprocess_culturefeed_ui_connect(&$variables) {

  $query['destination'] = isset($variables['url']) ? $variables['url'] : $_GET['q'];
  $query['via'] = 'facebook';
  $variables['link_facebook'] = l('Facebook', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-facebook')), 'query' => $query));

  $query['via'] = 'twitter';
  $variables['link_twitter'] = l('Twitter', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-twitter')), 'query' => $query));

  $query['via'] = 'google';
  $variables['link_google'] = l('Google', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-google')), 'query' => $query));

  unset($query['via']);
  $variables['link_email'] = l('je e-mailadres', 'culturefeed/oauth/connect', array('attributes' => array('class' => array('culturefeedconnect connect-email')), 'query' => $query));

}

/**
 * Preprocess variables for culturefeed-ui-connect-hover.tpl.php.
 *
 * @see culturefeed-ui-connect-hover.tpl.php
 */
function template_preprocess_culturefeed_ui_connect_hover(&$variables) {
  template_preprocess_culturefeed_ui_connect($variables);
}

/**
 * Preprocess variables for culturefeed-ui-something-for-you.tpl.php.
 *
 * @see culturefeed-ui-something-for-you.tpl.php
 */
function template_preprocess_culturefeed_ui_something_for_you(&$variables) {
  template_preprocess_culturefeed_ui_connect(&$variables);
}
