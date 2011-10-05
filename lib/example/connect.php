<?php

require 'common.php';

$cf = new CultureFeed($_COOKIE['key'], $_COOKIE['secret']);

$token = $cf->getRequestToken();

if (!$token) {
  return;
}

$base_url = 'http://' . $_SERVER['HTTP_HOST'] . rtrim(dirname($_SERVER['SCRIPT_NAME']), '\/') . '/';

$callback_url = $base_url . 'authorize.php?token=' . $token['oauth_token'] . '&token_secret=' . $token['oauth_token_secret'];

$auth_url = $cf->getUrlAuthorize($token, $callback_url);

setcookie('oauth_token', $token['oauth_token']);
setcookie('oauth_token_secret', $token['oauth_token_secret']);

Header("Location: $auth_url");
exit();