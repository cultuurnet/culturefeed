<?php

require 'common.inc';

$cf = new CultureFeed(CULTUREFEED_API_APPLICATION_KEY, CULTUREFEED_API_SHARED_SECRET);

$token = $cf->getRequestToken();

if (!$token) {
  return;
}

$callback_url = CULTUREFEED_API_SERVER . 'authorize.php?token=' . $token['oauth_token'] . '&token_secret=' . $token['oauth_token_secret'];

$auth_url = $cf->getUrlAuthorize($token, $callback_url);

setcookie('oauth_token', $token['oauth_token']);
setcookie('oauth_token_secret', $token['oauth_token_secret']);

Header("Location: $auth_url");
exit;